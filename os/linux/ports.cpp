/*
 *   Copyright (C) 2016 Grzegorz Sygieda
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <thread>
#include <cassert>
#include <map>
#include <utility>
#include <initializer_list>
#include <functional>

#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/poll.h"
#include <termios.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "egos.hpp"
#include "ports.hpp"

namespace osapi {

struct port_ctx {
    int trxfd;
    int exitfds[2];
    std::thread *handler;
    bool exit;
};

static port_ctx* port_spawn(int fd,
    std::function<void(int fd)> received,
    std::function<void()> closed)
{
    port_ctx *ctx;

    ctx = new port_ctx;
    assert(ctx);
    ctx->trxfd = fd;
    ctx->exit = false;
    assert(!pipe(ctx->exitfds));

    ctx->handler = new std::thread([ctx, received, closed]() {
            struct pollfd pfds[2];

            while(!ctx->exit) {
                pfds[0].fd = ctx->trxfd;
                pfds[0].events = POLLIN;
                pfds[1].fd = ctx->exitfds[1];
                pfds[1].events = POLLHUP;

                poll(pfds, 2, -1);
                if (pfds[0].revents & POLLIN) {
                    assert(received);
                    received(ctx->trxfd);
                } else if (pfds[1].revents & POLLHUP) {
                    close(ctx->exitfds[1]);
                    assert(closed);
                    closed();
                    continue;
                }
            }
        });

    return ctx;
}

static void port_cleanup(port_ctx *ctx)
{
    ctx->exit = true;
    close(ctx->exitfds[0]);
    ctx->handler->join();
    close(ctx->trxfd);

    delete ctx->handler;
    delete ctx;
}

bool serial_port::init()
{
    std::map<int, int> baudrates {
        std::make_pair(9600, B9600),
        std::make_pair(19200, B19200),
        std::make_pair(38400, B38400),
        std::make_pair(57600, B57600),
        std::make_pair(115200, B115200)
    };
    int fd;
    port_ctx *sp;

    egos::prints("serial init: %s baudrate: %u\n",
        _name.c_str(), _baudrate);
    if ((fd = open(_name.c_str(), O_RDWR)) < 0) {
        return false;
    }

    int br = B9600;
    auto it = baudrates.find(_baudrate);
    if (it != baudrates.end())
        br = it->second;
    else
        egos::prints("provided value not known, using default baudrate!\n");

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, br);
    cfsetospeed(&options, br);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag |= CSTOPB;
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */
    tcsetattr(fd, TCSANOW, &options);

    sp = port_spawn(fd,
        [this] (int fd) {
            char buf[1024];

            int ret = read(fd, buf, sizeof(buf));
            egos::prints("serial received (%d) data\n", ret);

            SER_FRAME sf;
            for(int i = 0; i < ret; i++) {
                sf._data = buf[i];
                this->_rcv->frame_received(this, sf);
            }
        },
        [] () {
            egos::prints("closing serial port\n");
        });

    assert(sp);
    _priv_data = static_cast<void *>(sp);

    return true;
}

bool serial_port::send_frame(frame &f)
{
    port_ctx *sp = static_cast<port_ctx *>(_priv_data);
    SER_FRAME &sf = static_cast<SER_FRAME&>(f);

    int ret = write(sp->trxfd,
        (const void *)(&sf._data), sizeof(sf._data));
    egos::prints("serial port send (%d)\n", ret);

    return true;
}

void serial_port::deinit()
{
    egos::prints("serial deinit\n");
    port_cleanup(static_cast<port_ctx *>(_priv_data));
    egos::prints("serial closed\n");
    _priv_data = static_cast<void *>(nullptr);
}

bool can_port::init()
{
    struct ifreq ifr;
    struct sockaddr_can addr;
    int fd;
    port_ctx *cp;

    egos::prints("can init: %s baudrate: %u\n",
        _name.c_str(), _baudrate);
    /* open socket */
    if ((fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        return false;
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, _name.c_str());

    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
        return false;
    }

    addr.can_ifindex = ifr.ifr_ifindex;
    fcntl(fd, F_SETFL, O_NONBLOCK);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return false;
    }

    // TODO: configure baudrate ?

    cp = port_spawn(fd,
        [this] (int fd) {
            struct can_frame hwf;

            int ret = read(fd, &hwf, sizeof(hwf));
            assert(ret == sizeof(hwf));
            egos::prints("can received (%d) data\n", ret);

            CAN_FRAME cf;
            cf._id = hwf.can_id;
            for(int i = 0; i < hwf.can_dlc; i++) {
                cf._data.push_back(hwf.data[i]);
            }
            this->_rcv->frame_received(this, cf);
        },
        [] () {
            egos::prints("closing can port\n");
        });

    assert(cp);
    _priv_data = static_cast<void *>(cp);

    return true;
}

bool can_port::send_frame(frame &f)
{
    port_ctx *cp = static_cast<port_ctx *>(_priv_data);
    CAN_FRAME &cf = static_cast<CAN_FRAME&>(f);
    struct can_frame hwf;

    hwf.can_id = cf._id;
    for(int i = 0; i < cf._data.size(); i++) {
        hwf.data[i] = cf._data[i];
    }
    hwf.can_dlc = cf._data.size();
    int ret = write(cp->trxfd,
        (const void *)(&hwf), sizeof(hwf));
    assert(ret == sizeof(hwf));
    egos::prints("can port send (%d)\n", ret);

    return true;
}

void can_port::deinit()
{
    egos::prints("can deinit\n");
    port_cleanup(static_cast<port_ctx *>(_priv_data));
    egos::prints("can closed\n");
    _priv_data = static_cast<void *>(nullptr);
}

}

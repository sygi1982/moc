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

#include "unistd.h"
#include "fcntl.h"
#include "sys/poll.h"
#include <termios.h>

#include "egos.hpp"
#include "ports.hpp"

namespace osapi {

struct serp {
    int trxfd;
    int exitfds[2];
    std::thread *handler;
    bool exit;
};

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
    serp *sp;

    egos::prints("serial init: %s\n", _name.c_str());
    if ((fd = open(_name.c_str(), O_RDWR)) < 0) {
        assert(false);
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

    sp = new serp;
    assert(sp);
    sp->trxfd = fd;
    sp->exit = false;
    assert(!pipe(sp->exitfds));

    sp->handler = new std::thread([this, sp]() {
            struct pollfd pfds[2];

            while(!sp->exit) {
                pfds[0].fd = sp->trxfd;
                pfds[0].events = POLLIN;
                pfds[1].fd = sp->exitfds[1];
                pfds[1].events = POLLHUP;

                poll(pfds, 2, -1);
                if (pfds[0].revents & POLLIN) {
                    char buf[1024];

                    int ret = read(sp->trxfd, buf, sizeof(buf));
                    egos::prints("received (%d) data\n", ret);

                    SER_FRAME sf;
                    for(int i = 0; i <= ret; i++) {
                        sf._data = buf[i];
                        this->_rcv->frame_received(this, sf);
                    }
                } else if (pfds[1].revents & POLLHUP) {
                    close(sp->exitfds[1]);
                    egos::prints("closing serial port\n");
                    continue;
                }
            }
        });

    _priv_data = static_cast<void *>(sp);

    return true;
}

bool serial_port::send_frame(frame &f)
{
    serp *sp = static_cast<serp *>(_priv_data);
    SER_FRAME &sf = static_cast<SER_FRAME&>(f);

    int ret = write(sp->trxfd,
        (const void *)(&sf._data), sizeof(sf._data));
    egos::prints("serial port send (%d)\n", ret);

    return true;
}

void serial_port::deinit()
{
    serp *sp = static_cast<serp *>(_priv_data);

    egos::prints("serial deinit\n");
    sp->exit = true;
    close(sp->exitfds[0]);
    sp->handler->join();
    close(sp->trxfd);
    egos::prints("serial closed\n");

    delete sp->handler;
    delete sp;
    _priv_data = static_cast<void *>(nullptr);
}

bool can_port::init()
{
    // TODO: attach to socketcan port
    egos::prints("can init\n");
    return true;
}

bool can_port::send_frame(frame &f)
{
    // TODO: use socketcan like api
    egos::prints("can port send\n");

    return true;
}

void can_port::deinit()
{
    // TODO: detach from socketcan port
    egos::prints("can deinit\n");
}

}

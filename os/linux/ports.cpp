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

#include "unistd.h"
#include "fcntl.h"
#include "sys/poll.h"

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
    egos::prints("serial init: %s\n", _name.c_str());
    int fd;
    serp *sp;

    if ((fd = open(_name.c_str(), O_RDWR)) < 0) {
        assert(false);
        return false;
    }

    // TODO: configure serial

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

                    for(int i = 0; i <= ret; i++) {
                        SER_FRAME sf;
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
    egos::prints("can init\n");
    return true;
}

bool can_port::send_frame(frame &f)
{
    // TODO: use write
    egos::prints("can port send\n");

    return true;
}

void can_port::deinit()
{
    egos::prints("can deinit\n");
}

}

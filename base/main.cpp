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
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <exception>
#include <stdio.h>

#include "egos.hpp"
#include "timerpool.hpp"
#include "ports.hpp"
#include "utils.hpp"

using namespace osapi;

class delayed_work : public workitem {
public:
    delayed_work(int id) : workitem(id) {
        egos::prints("delayed_work %u\n", workitem::get_id());
    }

    ~delayed_work() {
        egos::prints(" ~delayed_work %u\n", workitem::get_id());
    }

    bool utilize() {
        workitem::utilize();
        egos::prints(" delayed_work %u utilize\n", workitem::get_id());
        /*egos *os_api = &egos::get_instance();
        auto canp = os_api->get_port(comm_ports::CAN_PORT);
        CAN_FRAME cf;
        cf._id = 0x11111111;
        cf._timestamp = 0x22222222;
        canp->send_frame(cf);
        auto serp = os_api->get_port(comm_ports::SERIAL_PORT);
        SER_FRAME sf;
        sf._data = 0x55;
        serp->send_frame(sf);*/
        return true;
    }
};

int main(int argc, char **argv)
{
    egos *os_api = &egos::get_instance();

    try {
        os_api->initialize(argc, argv, 1000);
    } catch (egos::exception &e) {
        int err = e.get_code();

        if (!err)
            std::exit(0);
        else
            egos::prints("Unexpected error -> %u\n", err);
    }

    os_api->introduce_self();

    auto serp = os_api->get_port(comm_ports::SERIAL_PORT);
    assert(serp->is_ready());
    auto ser_handler = [&os_api] (frame &f) {
        egos::prints(" serial frame received %d!\n", static_cast<SER_FRAME *>(&f)->_data);
        autoitem item(new delayed_work(1));
        os_api->process_delayed(item, 1000);
    };
    serp->set_handler(ser_handler);
    SER_FRAME sf;
    sf._data = 0xAA;
    serp->send_frame(sf);

    auto canp = os_api->get_port(comm_ports::CAN_PORT);
    assert(canp->is_ready());
    auto can_handler = [&os_api] (frame &f) {
        egos::prints(" can frame received %d!\n", static_cast<CAN_FRAME *>(&f)->_id);
        autoitem item(new delayed_work(2));
        os_api->process_delayed(item, 1000);
    };
    canp->set_handler(can_handler);
    CAN_FRAME cf;
    cf._id = 0x11111111;
    cf._timestamp = 0x22222222;
    canp->send_frame(cf);

    try {
        autoitem item(new delayed_work(0));
        item.get_raw();
        os_api->process_delayed(item, 2000);
    } catch (std::bad_alloc &e) {
        egos::prints("\nBad alloc error\n");
    }

    //TODO: add a heartbeat task
    os_api->start();

    egos::prints("Finished!\n");
}

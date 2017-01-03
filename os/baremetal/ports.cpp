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

#include "egos.hpp"
#include "ports.hpp"

namespace osapi {

void generic_port_watcher::frame_received(port *owner, frame &f) const
{
    if (typeid(*owner) == typeid(serial_port)) {
        SER_FRAME &sf = static_cast<SER_FRAME&>(f);
        egos::prints(" serial frame handled 0x%x!\n", sf._data);
    } else if (typeid(*owner) == typeid(can_port)) {
        CAN_FRAME &cf = static_cast<CAN_FRAME&>(f);
        egos::prints(" can frame handled 0x%x!\n", cf._id);
    }

    // TODO: filter ?

    owner->call_handler(f);
}

bool serial_port::init()
{
    egos::prints("serial init\n");
    return true;
}

void serial_port::deinit()
{
    egos::prints("serial deinit\n");
}

bool can_port::init()
{
    egos::prints("can init\n");
    return true;
}

void can_port::deinit()
{
    egos::prints("can deinit\n");
}


bool serial_port::send_frame(frame &f)
{
    // TODO: this is for test only
    (_rcv)->frame_received(this, f);
    // TODO: use serial specific stuff

    return true;
}

bool can_port::send_frame(frame &f)
{
    // TODO: this is for test only
    (_rcv)->frame_received(this, f);
    // TODO: use can specific stuff

    return true;
}


}

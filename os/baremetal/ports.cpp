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
#include "hwcan.hpp"
#include "hwser.hpp"

namespace osapi {

using namespace halapi;

void generic_port_watcher::frame_received(port *owner, frame &f) const
{
    if (typeid(*owner) == typeid(serial_port)) {
        SER_FRAME &sf = static_cast<SER_FRAME&>(f);
        egos::prints(" serial frame handled 0x%x!\n", sf._data);
    } else if (typeid(*owner) == typeid(can_port)) {
        CAN_FRAME &cf = static_cast<CAN_FRAME&>(f);
        egos::prints(" can frame handled 0x%x!\n", cf._id);
    }

    // TODO: filter out ?
    owner->call_handler(f);
}

bool serial_port::init()
{
    egos::prints("serial init\n");
    _priv_data = static_cast<void *>(new hwser());
    assert(_priv_data);
    return _priv_data != nullptr;
}

void serial_port::deinit()
{
    egos::prints("serial deinit\n");
    hwser *ser = static_cast<hwser *>(_priv_data);
    if (ser)
        delete ser;
}

bool can_port::init()
{
    egos::prints("can init\n");
    _priv_data = static_cast<void *>(new hwcan());
    assert(_priv_data);
    return _priv_data != nullptr;
}

void can_port::deinit()
{
    egos::prints("can deinit\n");
    hwcan *can = static_cast<hwcan *>(_priv_data);
    if (can)
        delete can;
}

bool serial_port::send_frame(frame &f)
{
    SER_FRAME &sf = static_cast<SER_FRAME&>(f);
    hwser *ser = static_cast<hwser *>(_priv_data);
    // TODO: this is for test only
    //(_rcv)->frame_received(this, f);
    // TODO: use serial specific stuff
    ser->send();

    return true;
}

bool can_port::send_frame(frame &f)
{
    CAN_FRAME &cf = static_cast<CAN_FRAME&>(f);
    hwcan *can = static_cast<hwcan *>(_priv_data);
    // TODO: this is for test only
    //(_rcv)->frame_received(this, f);
    // TODO: use can specific stuff
    can->send();

    return true;
}


}

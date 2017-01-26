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

bool serial_port::init()
{
    egos::prints("serial init\n");

    auto ser_handler = [this] (HWSER_DAT &hwf) {
        SER_FRAME sf;
        sf._data = hwf.data;
        this->_rcv->frame_received(this, sf);
    };
    _priv_data = static_cast<void *>(new hwser(_baudrate, ser_handler));
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

    auto can_handler = [this] (HWCAN_DAT &hwf) {
        CAN_FRAME cf;
        cf._id = hwf.id;
        for (int i = 0; i < hwf.len; i++)
            cf._data.push_back(hwf.data[i]);
        cf._flags = hwf.flags;
        this->_rcv->frame_received(this, cf);
    };
    _priv_data = static_cast<void *>(new hwcan(_baudrate, can_handler));
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
    HWSER_DAT hwf;
    hwf.data = sf._data;
    ser->send(hwf);

    return true;
}

bool can_port::send_frame(frame &f)
{
    CAN_FRAME &cf = static_cast<CAN_FRAME&>(f);
    hwcan *can = static_cast<hwcan *>(_priv_data);
    HWCAN_DAT hwf;
    hwf.id = cf._id;
    for (int i = 0; i < cf._data.size(); i++)
        hwf.data[i] = cf._data[i];
    hwf.len = cf._data.size();
    hwf.flags = cf._flags;
    can->send(hwf);

    return true;
}

}

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
#ifndef __HWCAN_HPP__
#define __HWCAN_HPP__

#include <functional>

#include "irqmgr.hpp"

namespace halapi {

struct HWCAN_DAT : irqdat {
    unsigned int id;
    unsigned char data[8];
    unsigned char len;
    unsigned char flags;
};

class hwcan {

public:
    hwcan(int baudrate, std::function<void(HWCAN_DAT &d)> handler);

    ~hwcan();

    void send(HWCAN_DAT &d);

};

}

#endif

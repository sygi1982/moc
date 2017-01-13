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

namespace halapi {

typedef unsigned char HWCAN_DAT;

class hwcan {

public:
    hwcan(std::function<void(HWCAN_DAT &d)> handler);

    ~hwcan();

    void send(HWCAN_DAT &d);

};

}

#endif

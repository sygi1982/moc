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
#ifndef __IRQMGR_HPP__
#define __IRQMGR_HPP__

#include <functional>
#include <map>

#include "utils.hpp"

namespace halapi {

using namespace utils;

enum class irqsrc : int {
    UART0 = 0,
    CAN = 1,
    TIMER0 = 10,
    TIMER1 = 14,
    TIMER2 = 18,
    TIMER3 = 22
};

struct irqdat {
    int dat[0];
};

class irqmgr : public singleton<irqmgr> {
    std::map<int, std::function<void(irqdat&)>> _handlers;

    void handle_int(int num, irqdat& data);

    static const int MAX_GROUPS = 4;

    int _group_ref[MAX_GROUPS]; /* Used for managing hw specific stuff */

public:
    inline void initialize() {
        _handlers.clear();
        for (int i = 0; i < MAX_GROUPS; i++)
            _group_ref[i] = 0;
    }

    void register_int(const int num, std::function<void(irqdat&)> handler);

    void update_int(const int num, std::function<void(irqdat&)> handler);

    void unregister_int(const int num);

    void ints_ena();

    void ints_dis();

    void wfi();

    friend void raise_int(int num, irqdat& data);

};

}

#endif

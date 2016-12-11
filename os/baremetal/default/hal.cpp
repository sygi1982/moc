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
#include <cassert>

#include "irqmgr.hpp"
#include "hwtmr.hpp"

namespace halapi {

/* Hardware timers */
void hwtmr::start(int msecs, std::function<void()> delegate)
{
    irqmgr::get_instance().register_int(_id, delegate);
};

void hwtmr::stop()
{
    irqmgr::get_instance().unregister_int(_id);
};

/* Interrupt controller */
void irqmgr::register_int(const int num, std::function<void()> handler)
{
    assert(handler);
    _handlers.insert(std::pair<int, std::function<void()>>(num, handler));
}

void irqmgr::unregister_int(const int num)
{
    auto it = _handlers.find(num);
    _handlers.erase(it);
}

void irqmgr::handle_int(int num)
{
    auto handler = _handlers.at(num);
    assert(handler);
    handler();
}

void irqmgr::ints_ena()
{
}

void irqmgr::ints_dis()
{
}

void irqmgr::wfi()
{
}

/* This is irqmgr proxy function */
void raise_int(int num)
{
    irqmgr::get_instance().handle_int(num);
}

}

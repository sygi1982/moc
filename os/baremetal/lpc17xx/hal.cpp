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

#include "lpc17xx_can.h"

namespace halapi {

/* Hardware timers */
void hwtmr::start(int msecs, std::function<void()> delegate)
{
    irqmgr::get_instance().register_int(_id, delegate);
    //TODO: call plat C function
    CAN_Init(LPC_CAN1, 125000);
};

void hwtmr::stop()
{
    irqmgr::get_instance().unregister_int(_id);
    //TODO: call plat C function
};

/* Interrupt controller */
void irqmgr::register_int(const int num, std::function<void()> handler)
{
    assert(handler);
    _handlers.insert(std::pair<int, std::function<void()>>(num, handler));
    //TODO: call plat C function
}

void irqmgr::unregister_int(const int num)
{
    auto it = _handlers.find(num);
    _handlers.erase(it);
    //TODO: call plat C function
}

void irqmgr::handle_int(int num)
{
    auto handler = _handlers.at(num);
    assert(handler);
    handler();
}

void irqmgr::ints_ena()
{
    //TODO: call plat C function
}

void irqmgr::ints_dis()
{
    //TODO: call plat C function
}

void irqmgr::wfi()
{
    //TODO: call plat C function
}

/* This is irqmgr proxy function */
void raise_int(int num)
{
    irqmgr::get_instance().handle_int(num);
}

extern "C" void TIMER0_IRQHandler(void)
{
    raise_int(0);
}

extern "C" void CAN_IRQHandler(void)
{
    raise_int(1);
}

}

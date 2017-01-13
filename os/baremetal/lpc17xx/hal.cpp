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
#include "hwcan.hpp"
#include "hwser.hpp"
#include "LPC17xx.h"
#include "core_cm3.h"
#include "core_cmInstr.h"
#include "core_cmFunc.h"
#include "lpc17xx_can.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

namespace halapi {

/* Interrupt controller */
void irqmgr::register_int(const int num, std::function<void()> handler)
{
    assert(handler);
    _handlers.insert(std::pair<int, std::function<void()>>(num, handler));
    //TODO: call plat C function ???
}

void irqmgr::unregister_int(const int num)
{
    auto it = _handlers.find(num);
    _handlers.erase(it);
    //TODO: call plat C function ???
}

void irqmgr::handle_int(int num)
{
    auto handler = _handlers.at(num);
    assert(handler);
    handler();
}

void irqmgr::ints_ena()
{
    __enable_irq();
}

void irqmgr::ints_dis()
{
    __disable_irq();
}

void irqmgr::wfi()
{
    __WFI();
}

/* This is irqmgr proxy function */
void raise_int(int num)
{
    irqmgr::get_instance().handle_int(num);
}

extern "C" void TIMER0_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::TIMER0));
}

extern "C" void TIMER1_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::TIMER1));
}

extern "C" void TIMER2_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::TIMER2));
}

extern "C" void TIMER3_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::TIMER3));
}

extern "C" void CAN_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::CAN));
}

extern "C" void UART0_IRQHandler(void)
{
    raise_int(static_cast<int>(irqsrc::UART0));
}

/* Hardware timers */
void hwtmr::start(int msecs, std::function<void()> delegate)
{
    int irq = static_cast<int>(irqsrc::TIMER0) + _id;
    irqmgr::get_instance().register_int(irq, delegate);
    //TODO: call plat C function
};

void hwtmr::stop()
{
    int irq = static_cast<int>(irqsrc::TIMER0) + _id;
    irqmgr::get_instance().unregister_int(irq);
    //TODO: call plat C function
};

hwcan::hwcan(std::function<void(HWCAN_DAT &d)> handler)
{
    auto irq_routine = [this, handler] () {
        //TODO: call plat C function, read registers
        HWCAN_DAT d;
        handler(d);
    };
    irqmgr::get_instance().register_int(static_cast<int>(irqsrc::CAN), irq_routine);
}

hwcan::~hwcan()
{
    irqmgr::get_instance().unregister_int(static_cast<int>(irqsrc::CAN));
}

void hwcan::send(HWCAN_DAT &d)
{
    //TODO: call plat C function
}

hwser::hwser(std::function<void(HWSER_DAT &d)> handler)
{
    auto irq_routine = [this, handler] () {
        //TODO: call plat C function, read registers
        HWSER_DAT d;
        handler(d);
    };
    irqmgr::get_instance().register_int(static_cast<int>(irqsrc::UART0), irq_routine);
}

hwser::~hwser()
{
    irqmgr::get_instance().unregister_int(static_cast<int>(irqsrc::UART0));
}

void hwser::send(HWSER_DAT &d)
{
    //TODO: call plat C function
}

}

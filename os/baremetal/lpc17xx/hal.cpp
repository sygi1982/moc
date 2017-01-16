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
    _handlers.insert(std::pair<int,
        std::function<void()>>(num, handler));

    switch(static_cast<irqsrc>(num)) {
        case irqsrc::TIMER0:
            /* preemption = 1, sub-priority = 1 */
            NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
            /* Enable interrupt for timer 0 */
            NVIC_EnableIRQ(TIMER0_IRQn);
        default:
            assert(false);
    }
}

void irqmgr::update_int(const int num, std::function<void()> handler)
{
    auto it = _handlers.find(num);

    if (it != _handlers.end()) {
        assert(handler);
        it->second = handler;
    } else {
        assert(false);
    }
}

void irqmgr::unregister_int(const int num)
{
    auto it = _handlers.find(num);
    _handlers.erase(it);

    switch(static_cast<irqsrc>(num)) {
        case irqsrc::TIMER0:
            /* Disable interrupt for timer 0 */
            NVIC_DisableIRQ(TIMER0_IRQn);
        default:
            assert(false);
    }
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
    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)== SET) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        raise_int(static_cast<int>(irqsrc::TIMER0));
    } else {
        assert(false);  // spurious
    }
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
TIM_TIMERCFG_Type gTIM_ConfigStruct = {
    // prescale count time of 100us
    PrescaleOption : TIM_PRESCALE_USVAL,
    { 0, 0, 0}, // rsvd
    PrescaleValue : 100
};

TIM_MATCHCFG_Type gTIM_MatchConfigStruct = {
    MatchChannel : 0,
    IntOnMatch : TRUE,
    StopOnMatch : TRUE,
    ResetOnMatch : TRUE,
    ExtMatchOutputType : FALSE,
    { 0, 0, 0}, // rsvd
    MatchValue : 10
};

hwtmr::hwtmr(int &id, std::function<void()> delegate) : _id(id)
{
    int irq = static_cast<int>(irqsrc::TIMER0) + id;
    irqmgr::get_instance().register_int(irq, delegate);
}

hwtmr::~hwtmr()
{
    int irq = static_cast<int>(irqsrc::TIMER0) + _id;
    irqmgr::get_instance().unregister_int(irq);
}

void hwtmr::start(int msecs, std::function<void()> delegate)
{
    int irq = static_cast<int>(irqsrc::TIMER0) + _id;
    irqmgr::get_instance().update_int(irq, delegate);

    // use channel 0, MR0
    gTIM_MatchConfigStruct.MatchChannel = 0;
    // Set Match value, count value of base 10 (10 * 100uS = 1000 = 1ms --> 1 kHz)
    gTIM_MatchConfigStruct.MatchValue = 10 * msecs;
    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &gTIM_ConfigStruct);
    TIM_ConfigMatch(LPC_TIM0, &gTIM_MatchConfigStruct);
    // To start timer 0
    TIM_Cmd(LPC_TIM0, ENABLE);
};

void hwtmr::stop()
{
    TIM_Cmd(LPC_TIM0, DISABLE);
};

hwcan::hwcan(std::function<void(HWCAN_DAT &d)> handler)
{
    auto irq_routine = [this, handler] () {
        //TODO: call plat C function, read registers
        HWCAN_DAT d;
        handler(d);
    };

    irqmgr::get_instance().register_int(
        static_cast<int>(irqsrc::CAN), irq_routine);
}

hwcan::~hwcan()
{
    irqmgr::get_instance().unregister_int(
        static_cast<int>(irqsrc::CAN));
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

    irqmgr::get_instance().register_int(
        static_cast<int>(irqsrc::UART0), irq_routine);
}

hwser::~hwser()
{
    irqmgr::get_instance().unregister_int(
        static_cast<int>(irqsrc::UART0));
}

void hwser::send(HWSER_DAT &d)
{
    //TODO: call plat C function
}

}

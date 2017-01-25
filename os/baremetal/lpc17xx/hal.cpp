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
#include "lpc17xx_pinsel.h"

namespace halapi {

/* Interrupt controller */
void irqmgr::register_int(const int num, std::function<void(irqdat&)> handler)
{
    IRQn_Type irq = WDT_IRQn;

    assert(handler);
    _handlers.insert(std::pair<int,
        std::function<void(irqdat&)>>(num, handler));
    int status = -1;

    irqsrc src = static_cast<irqsrc>(num);
    if (src == irqsrc::UART0)
        irq = UART0_IRQn;
    else if (src == irqsrc::CAN)
        irq = CAN_IRQn;
    else if (src >= irqsrc::TIMER0 && src < irqsrc::TIMER1) {
        irq = TIMER0_IRQn;
        status = _group_ref[0]++;
    } else if (src >= irqsrc::TIMER1 && src < irqsrc::TIMER2) {
        irq = TIMER1_IRQn;
        status = _group_ref[1]++;
    } else if (src >= irqsrc::TIMER2 && src < irqsrc::TIMER3) {
        irq = TIMER2_IRQn;
        status = _group_ref[2]++;
    } else if (src >= irqsrc::TIMER3) {
        irq = TIMER3_IRQn;
        status = _group_ref[3]++;
    } else
        assert(false);

    if (status <= 0) {
        /* preemption = 1, sub-priority = 1 */
        NVIC_SetPriority(irq, ((0x01<<3)|0x01));
        /* Enable interrupt */
        NVIC_EnableIRQ(irq);
    }
}

void irqmgr::update_int(const int num, std::function<void(irqdat&)> handler)
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
    IRQn_Type irq = WDT_IRQn;
    auto it = _handlers.find(num);
    _handlers.erase(it);
    int status = -1;

    irqsrc src = static_cast<irqsrc>(num);
    if (src == irqsrc::UART0)
        irq = UART0_IRQn;
    else if (src == irqsrc::CAN)
        irq = CAN_IRQn;
    else if (src >= irqsrc::TIMER0 && src < irqsrc::TIMER1) {
        irq = TIMER0_IRQn;
        status = --_group_ref[0];
    } else if (src >= irqsrc::TIMER1 && src < irqsrc::TIMER2) {
        irq = TIMER1_IRQn;
        status = --_group_ref[1];
    } else if (src >= irqsrc::TIMER2 && src < irqsrc::TIMER3) {
        irq = TIMER2_IRQn;
        status = --_group_ref[2];
    } else if (src >= irqsrc::TIMER3) {
        irq = TIMER3_IRQn;
        status = --_group_ref[3];
    } else
        assert(false);

    if (status <= 0)
        /* Disable interrupt */
        NVIC_DisableIRQ(irq);
}

void irqmgr::handle_int(int num, irqdat& data)
{
    auto handler = _handlers.at(num);
    assert(handler);
    handler(data);
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
void raise_int(int num, irqdat& data)
{
    irqmgr::get_instance().handle_int(num, data);
}

extern "C" void TIMER0_IRQHandler(void)
{
    irqdat data;

    if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        raise_int(static_cast<int>(irqsrc::TIMER0), data);
    } else if (TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
        raise_int(static_cast<int>(irqsrc::TIMER0) + 1, data);
    } else if (TIM_GetIntStatus(LPC_TIM0, TIM_MR2_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR2_INT);
        raise_int(static_cast<int>(irqsrc::TIMER0) + 2, data);
    } else if (TIM_GetIntStatus(LPC_TIM0, TIM_MR3_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM0, TIM_MR3_INT);
        raise_int(static_cast<int>(irqsrc::TIMER0) + 3, data);
    } else {
        assert(false);  // spurious
    }
}

extern "C" void TIMER1_IRQHandler(void)
{
    irqdat data;

    if (TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
        raise_int(static_cast<int>(irqsrc::TIMER1), data);
    } else if (TIM_GetIntStatus(LPC_TIM1, TIM_MR1_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM1, TIM_MR1_INT);
        raise_int(static_cast<int>(irqsrc::TIMER1) + 1, data);
    } else if (TIM_GetIntStatus(LPC_TIM1, TIM_MR2_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM1, TIM_MR2_INT);
        raise_int(static_cast<int>(irqsrc::TIMER1) + 2, data);
    } else if (TIM_GetIntStatus(LPC_TIM1, TIM_MR3_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM1, TIM_MR3_INT);
        raise_int(static_cast<int>(irqsrc::TIMER1) + 3, data);
    } else {
        assert(false);  // spurious
    }
}

extern "C" void TIMER2_IRQHandler(void)
{
    irqdat data;

    if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);
        raise_int(static_cast<int>(irqsrc::TIMER2), data);
    } else if (TIM_GetIntStatus(LPC_TIM2, TIM_MR1_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR1_INT);
        raise_int(static_cast<int>(irqsrc::TIMER2) + 1, data);
    } else if (TIM_GetIntStatus(LPC_TIM2, TIM_MR2_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR2_INT);
        raise_int(static_cast<int>(irqsrc::TIMER2) + 2, data);
    } else if (TIM_GetIntStatus(LPC_TIM2, TIM_MR3_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR3_INT);
        raise_int(static_cast<int>(irqsrc::TIMER2) + 3, data);
    } else {
        assert(false);  // spurious
    }
}

extern "C" void TIMER3_IRQHandler(void)
{
    irqdat data;

    if (TIM_GetIntStatus(LPC_TIM3, TIM_MR0_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM3, TIM_MR0_INT);
        raise_int(static_cast<int>(irqsrc::TIMER3), data);
    } else if (TIM_GetIntStatus(LPC_TIM3, TIM_MR1_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM3, TIM_MR1_INT);
        raise_int(static_cast<int>(irqsrc::TIMER3) + 1, data);
    } else if (TIM_GetIntStatus(LPC_TIM3, TIM_MR2_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM3, TIM_MR2_INT);
        raise_int(static_cast<int>(irqsrc::TIMER3) + 2, data);
    } else if (TIM_GetIntStatus(LPC_TIM3, TIM_MR3_INT) == SET) {
        TIM_ClearIntPending(LPC_TIM3, TIM_MR3_INT);
        raise_int(static_cast<int>(irqsrc::TIMER3) + 3, data);
    } else {
        assert(false);  // spurious
    }
}

extern "C" void CAN_IRQHandler(void)
{
    /* Get CAN status */
    int status = CAN_GetCTRLStatus(LPC_CAN1, CANCTRL_STS);
    // check receive buffer status
    if (status & 0x01) {
        CAN_MSG_Type frame;
        CAN_ReceiveMsg(LPC_CAN1, &frame);
        HWCAN_DAT hwf;
        hwf.id = frame.id;
        hwf.len = frame.len;
        hwf.data[0] = frame.dataA[0];
        hwf.data[1] = frame.dataA[1];
        hwf.data[2] = frame.dataA[2];
        hwf.data[3] = frame.dataA[3];
        hwf.data[4] = frame.dataB[0];
        hwf.data[5] = frame.dataB[1];
        hwf.data[6] = frame.dataB[2];
        hwf.data[7] = frame.dataB[3];
        hwf.flags = 0; // TODO: flags
        raise_int(static_cast<int>(irqsrc::CAN), hwf);
    }
}

extern "C" void UART0_IRQHandler(void)
{
    int status, tmp, tmp1;
    HWSER_DAT hwf;

    /* Determine the interrupt source */
    status = UART_GetIntId(LPC_UART0);
    tmp = status & UART_IIR_INTID_MASK;

    // Receive Line Status
    if (tmp == UART_IIR_INTID_RLS) {
        // Check line status
        tmp1 = UART_GetLineStatus(LPC_UART0);
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1) {
            assert(false);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)) {
        int len = UART_Receive((LPC_UART_TypeDef *)LPC_UART0,
            &hwf.data, 1, NONE_BLOCKING);
        raise_int(static_cast<int>(irqsrc::UART0), hwf);
    }

    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE) {
        // TODO: tx irq ?
    }
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

hwtmr::hwtmr(int &id) : _id(id)
{
}

hwtmr::~hwtmr()
{
}

void hwtmr::start(int msecs, std::function<void()> delegate)
{
    LPC_TIM_TypeDef *tim = nullptr;
    int irq = static_cast<int>(irqsrc::TIMER0) + _id;
    auto tmr_handler = [delegate] (irqdat& data) {
        /* irqdat is empty */
        delegate();
    };
    irqmgr::get_instance().update_int(irq, tmr_handler);

    // Set Match value, count value of base 10 (10 * 100uS = 1000 = 1ms --> 1 kHz)
    gTIM_MatchConfigStruct.MatchValue = 10 * msecs;
    // select channel MRx
    gTIM_MatchConfigStruct.MatchChannel = _id % 4;

    switch(_id / 4) {
        case 0:  // TIMER0 MR0-3
            tim = LPC_TIM0;
            break;
        case 1:  // TIMER1 MR0-3
            tim = LPC_TIM1;
            break;
        case 2:  // TIMER2 MR0-3
            tim = LPC_TIM2;
            break;
        case 3:  // TIMER3 MR0-3
            tim = LPC_TIM3;
            break;
        default:
            assert(false);
            break;
    }

    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(tim, TIM_TIMER_MODE, &gTIM_ConfigStruct);
    TIM_ConfigMatch(tim, &gTIM_MatchConfigStruct);
    // To start timer
    TIM_Cmd(tim, ENABLE);
};

void hwtmr::stop(bool force)
{
    /* No need to disable the timer as it auto disables
       when expired, but in some cases C level interrupt may be raised,
       so unregister the handler to avoid signal propagation */
    if (force) {
        int irq = static_cast<int>(irqsrc::TIMER0) + _id;
        irqmgr::get_instance().unregister_int(irq);
    }
};

hwcan::hwcan(std::function<void(HWCAN_DAT &d)> handler)
{
    auto irq_routine = [this, handler] (irqdat& data) {
        HWCAN_DAT hwf = static_cast<HWCAN_DAT&>(data);
        handler(hwf);
    };

    // TODO: baudrate, mode
    CAN_Init(LPC_CAN1, 125000);

    // Enable oper mode
    CAN_ModeConfig(LPC_CAN1, CAN_OPERATING_MODE, ENABLE);

    // Enable Interrupts
    CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);
    CAN_IRQCmd(LPC_CAN1, CANINT_TIE1, ENABLE);

    CAN_SetAFMode(LPC_CANAF, CAN_Normal);

    irqmgr::get_instance().register_int(static_cast<int>(irqsrc::CAN),
        irq_routine);
}

hwcan::~hwcan()
{
    irqmgr::get_instance().unregister_int(
        static_cast<int>(irqsrc::CAN));

    // Disable oper mode
    CAN_ModeConfig(LPC_CAN1, CAN_OPERATING_MODE, ENABLE);
    // Disable Interrupts
    CAN_IRQCmd(LPC_CAN1, CANINT_RIE, DISABLE);
    CAN_IRQCmd(LPC_CAN1, CANINT_TIE1, DISABLE);
}

void hwcan::send(HWCAN_DAT &hwf)
{
    CAN_MSG_Type frame;

    // TODO: format from flags
    frame.format = EXT_ID_FORMAT;
    frame.id = hwf.id;
    frame.len = hwf.len;
    frame.type = DATA_FRAME;
    frame.dataA[0] = hwf.data[0];
    frame.dataA[1] = hwf.data[1];
    frame.dataA[2] = hwf.data[2];
    frame.dataA[3] = hwf.data[3];
    frame.dataB[0] = hwf.data[4];
    frame.dataB[1] = hwf.data[5];
    frame.dataB[2] = hwf.data[6];
    frame.dataB[3] = hwf.data[7];

    CAN_SendMsg(LPC_CAN1, &frame);
}

hwser::hwser(std::function<void(HWSER_DAT &d)> handler)
{
    auto irq_routine = [this, handler] (irqdat& data) {
        HWSER_DAT hwf = static_cast<HWSER_DAT&>(data);
        handler(hwf);
    };

    // UART Configuration structure variable
    UART_CFG_Type UARTConfigStruct;
    // UART FIFO configuration Struct variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    // Pin configuration for UART0
    PINSEL_CFG_Type PinCfg;
    /*
     * Initialize UART0 pin connect
     */
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 2;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);
    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    // TODO: baudrate, mode
    UART_ConfigStructInit(&UARTConfigStruct);

    // Initialize UART0 peripheral with given to corresponding parameter
    UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     *                 - FIFO_DMAMode = DISABLE
     *                 - FIFO_Level = UART_FIFO_TRGLEV0
     *                 - FIFO_ResetRxBuf = ENABLE
     *                 - FIFO_ResetTxBuf = ENABLE
     *                 - FIFO_State = ENABLE
     */
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    // Initialize FIFO for UART0 peripheral
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);

    // Enable UART Transmit
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);

    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RLS, ENABLE);

    irqmgr::get_instance().register_int(
        static_cast<int>(irqsrc::UART0), irq_routine);
}

hwser::~hwser()
{
    irqmgr::get_instance().unregister_int(
        static_cast<int>(irqsrc::UART0));

    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART0));
    // DeInitialize UART0 peripheral
    UART_DeInit((LPC_UART_TypeDef *)LPC_UART0);
}

void hwser::send(HWSER_DAT &hwf)
{
    /* Temporarily lock out UART transmit interrupts during this
       read so the UART transmit interrupt won't cause problems
       with the index values */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0,
        UART_INTCFG_THRE, DISABLE);
    UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&hwf.data,
        1, NONE_BLOCKING);
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0,
        UART_INTCFG_THRE, ENABLE);
}

}

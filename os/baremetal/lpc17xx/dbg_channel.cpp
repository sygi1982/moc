/*
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

#include "LPC17xx.h"
#include "core_cm3.h"
#include "core_cmInstr.h"
#include "core_cmFunc.h"
#include "lpc17xx_gpio.h"

extern "C" {

#define FT245_TXE  (1<<28)  //P1
#define FT245_RXF  (1<<29)  //P1
#define FT245_WR   (1<<26)  //P3
#define FT245_RD   (1<<25)  //P3
#define FT245_DATA (0x03FC0000) // P1.18-P1.25  D3,D6,D5,D7,D1,D2,D4,D0
#define FT245_DATA_SHIFT (18)

static bool ft245_write(unsigned int data)
{
    unsigned int temp = 0;

    if (FIO_ReadValue(1) & FT245_TXE) { //check TXE# line
        return false;
    }

    if (data&0x01) temp =  0x2000000;  // 7
    if (data&0x02) temp |= 0x0400000;  // 4
    if (data&0x04) temp |= 0x0800000;  // 5
    if (data&0x08) temp |= 0x0040000;  // 0
    if (data&0x10) temp |= 0x1000000;  // 6
    if (data&0x20) temp |= 0x0100000;  // 2
    if (data&0x40) temp |= 0x0080000;  // 1
    if (data&0x80) temp |= 0x0200000;  // 3

    FIO_SetDir(1, FT245_DATA, 1);   //configure data to outputs to send data to FTDI inputs
    __NOP();
    __NOP();
    __NOP();
    FIO_ClearValue(1, FT245_DATA);  //clear data pins
    __NOP();
    __NOP();
    __NOP();
    FIO_SetValue(1, temp);   //write data (already shifted by FT245_DATA_SHIFT)
    __NOP();
    __NOP();
    __NOP();
    FIO_SetValue(3, FT245_WR);      //set WR to high
    __NOP();
    __NOP();
    __NOP();
    FIO_ClearValue(3, FT245_WR);    //set WR from high to low
    __NOP();
    __NOP();
    __NOP();

    return true;
}

static bool ft245_read(unsigned char *pdata)
{
    unsigned char temp = 0;

    if (FIO_ReadValue(1) & FT245_RXF) {  //check RXF# line if data avialable
        return false;
    }

    *pdata = 0;
    __NOP();
    __NOP();
    __NOP();

    FIO_SetDir(1, FT245_DATA, 0);     //configure data to inputs to receive data from FTDI outputs
    FIO_ClearValue(3, FT245_RD);      //clear RD#
    __NOP();
    __NOP();
    __NOP();

    temp = (FIO_ReadValue(1)>>FT245_DATA_SHIFT)&0xFF;    //read data
    __NOP();
    __NOP();
    __NOP();

    FIO_SetValue(3, FT245_RD);               //set RD#
    __NOP();
    __NOP();
    __NOP();


    // P1.18-P1.25  D3,D6,D5,D7,D1,D2,D4,D0
    *pdata |= ((temp>>7)&0x01) ? 1<<0 : 0<<0;
    *pdata |= ((temp>>4)&0x01) ? 1<<1 : 0<<1;
    *pdata |= ((temp>>5)&0x01) ? 1<<2 : 0<<2;
    *pdata |= ((temp>>0)&0x01) ? 1<<3 : 0<<3;
    *pdata |= ((temp>>6)&0x01) ? 1<<4 : 0<<4;
    *pdata |= ((temp>>2)&0x01) ? 1<<5 : 0<<5;
    *pdata |= ((temp>>1)&0x01) ? 1<<6 : 0<<6;
    *pdata |= ((temp>>3)&0x01) ? 1<<7 : 0<<7;

    return true;
}

static void ft245_init(void)
{
    FIO_SetDir(1, FT245_DATA, 1);  // data as output by default
    FIO_ClearValue(1, FT245_DATA);

    FIO_SetDir(1, FT245_TXE|FT245_RXF, 0); // as input

    FIO_SetDir(3, FT245_WR|FT245_RD, 1);
    FIO_SetValue(3, FT245_WR|FT245_RD);

/*
 PWREN#-Output Goes low after the device is con.gured by USB, then high during USB suspend.
 RD#  - Input. Enables the current FIFO data byte on D0...D7 when low.
        Fetched the next FIFO data byte (if available) from the receive
        FIFO buffer when RD# goes from high to low.
 WR   - Input Writes the data byte on the D0...D7 pins into the transmit FIFO
        buffer when WR goes from high to low.
 TXE# - Output When high, do not write data into the FIFO.
         When low, data can be written into the FIFO by strobing WR high,then low.
 RXF# -  Output When high, do not read data from the FIFO. When low, there is data available
         in the FIFO which can be read by strobing RD# low, then high again.
 NOTE: Input/Output is from the FT245 side of view
  */
}

void dbg_channel_init()
{
    ft245_init();
}

int dbg_channel_write(const char *ptr, int len)
{
    int i = 0;

    while (i < len) {
        char val = ptr[i];
        if (ft245_write(val) == true)
            i++;
        else
            continue;
    }

    return true;
}

int dbg_channel_read(char *ptr, int len)
{
    int i = 0;

    while (i < len) {
        char *pval = &ptr[i];
        if (ft245_read((unsigned char *)pval) == true)
            i++;
        else
            continue;
    }

    return true;
}

}

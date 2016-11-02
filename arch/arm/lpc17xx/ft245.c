#include "ft245.h"
#include "compiler.h"
#include <NXP/LPC17xx/LPC17xx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __nop(x) { int i=0; while(i++ < x); }

unsigned char ft245_write(unsigned int data)
{
    unsigned int temp = 0;

    if((FT245_TXE)) { //check TXE# line
        return FALSE;
    }

    if(data&0x01)temp =  0x2000000;  // 7
    if(data&0x02)temp |= 0x0400000;  // 4
    if(data&0x04)temp |= 0x0800000;  // 5
    if(data&0x08)temp |= 0x0040000;  // 0
    if(data&0x10)temp |= 0x1000000;  // 6
    if(data&0x20)temp |= 0x0100000;  // 2
    if(data&0x40)temp |= 0x0080000;  // 1
    if(data&0x80)temp |= 0x0200000;  // 3

    FIO1DIR |= 0x03FC0000;   //configure data to outputs to send data to FTDI inputs
    __nop(1);
    __nop(1);
    __nop(1);
    FIO1CLR = 0x03FC0000;   //clear data pins
    __nop(1);
    __nop(1);
    __nop(1);
    FIO1SET = temp; //write data //(temp << 18)
    __nop(1);
    __nop(1);
    __nop(1);
    FIO3SET = FT245_WR;      //set WR to high
    __nop(1);
    __nop(1);
    __nop(1);
    FIO3CLR = FT245_WR;    //set WR from high to low
    __nop(1);
    __nop(1);
    __nop(1);

    return TRUE;
}

unsigned char ft245_read(unsigned char *pdata)
{
    unsigned char temp = 0;

    if((FT245_RXF))           //check RXF# line    if data avialable
        return FALSE;

    *pdata = 0;
    __nop(1);
    __nop(1);
    __nop(1);

    FIO1DIR &= ~(0x03FC0000);              //configure data to inputs to receive data from FTDI outputs
    FIO3CLR = FT245_RD;               //clear RD#
    __nop(1);
    __nop(1);
    __nop(1);

    temp = (FIO1PIN>>18)&0xFF;          //read data     //important IOPIN (FIO1PIN>>18)&0xFF;
    __nop(1);
    __nop(1);
    __nop(1);


    FIO3SET = FT245_RD;               //set RD#
    __nop(1);
    __nop(1);
    __nop(1);


    // P1.18-P1.25  D3,D6,D5,D7,D1,D2,D4,D0
    *pdata |= ((temp>>7)&0x01) ? 1<<0 : 0<<0;
    *pdata |= ((temp>>4)&0x01) ? 1<<1 : 0<<1;
    *pdata |= ((temp>>5)&0x01) ? 1<<2 : 0<<2;
    *pdata |= ((temp>>0)&0x01) ? 1<<3 : 0<<3;
    *pdata |= ((temp>>6)&0x01) ? 1<<4 : 0<<4;
    *pdata |= ((temp>>2)&0x01) ? 1<<5 : 0<<5;
    *pdata |= ((temp>>1)&0x01) ? 1<<6 : 0<<6;
    *pdata |= ((temp>>3)&0x01) ? 1<<7 : 0<<7;

    return TRUE;
}

void ft245_init(void)
{
    FIO1DIR = 0x03FC0000; //set output for data
    FIO1CLR |= 0x03FC0000;      // P1.18-P1.25  D3,D6,D5,D7,D1,D2,D4,D0

    FIO1DIR &= ~(1<<28 | 1<<29);    // as input

    FIO3DIR |=  FT245_WR|FT245_RD;
    FIO3SET |=  FT245_WR;    //
    FIO3SET |=  FT245_RD;    //

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

#ifdef __cplusplus
}
#endif

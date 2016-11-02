#include <NXP/LPC17xx/LPC17xx.h>
#include <stdio.h>
#include "compiler.h"
#include "target.h"

void target_init(void)
{
    /* Add your codes here */
    //PCLKSEL1 |= 0x00001000;    // PCLK_TIMER2 = CCLK

    //TODO: check this for UART variant - really needed ?????
    PCLKSEL0 &= ~(0xFC000000);    // CAN
    PCLKSEL0 |= 0xFC000000;

    /* Reset all GPIO pins to default: primary function */
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;
    PINSEL2 = 0x00000000;
    PINSEL3 = 0x00000000;
    PINSEL4 = 0x00000000;
    PINSEL5 = 0x00000000;
    PINSEL6 = 0x00000000;
    PINSEL7 = 0x00000000;
    PINSEL8 = 0x00000000;
    PINSEL9 = 0x00000000;
    PINSEL10 = 0x00000000;

    return;
}

void target_configure( void )
{
    DWORD MValue, NValue;

    if ( PLL0STAT & (1 << 25) )
    {
        PLL0CON = 1;            /* Enable PLL, disconnected */
        PLL0FEED = 0xaa;
        PLL0FEED = 0x55;
    }

    PLL0CON = 0;                /* Disable PLL, disconnected */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;

    SCS |= 0x20;            /* Enable main OSC */
    while( !(SCS & 0x40) );    /* Wait until main OSC is usable */

    CLKSRCSEL = 0x1;        /* select main OSC, 12MHz, as the PLL clock source */

    PLL0CFG = PLL0_MValue | (PLL0_NValue << 16);
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;

    PLL0CON = 1;                /* Enable PLL, disconnected */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;

    CCLKCFG = CCLKDivValue;    /* Set clock divider */

    while ( ((PLL0STAT & (1 << 26)) == 0) );    /* Check lock bit status */

    MValue = PLL0STAT & 0x00007FFF;
    NValue = (PLL0STAT & 0x00FF0000) >> 16;
    while ((MValue != PLL0_MValue) && ( NValue != PLL0_NValue) );

    PLL0CON = 3;                /* enable and connect */
    PLL0FEED = 0xaa;
    PLL0FEED = 0x55;
    while ( ((PLL0STAT & (1 << 25)) == 0) );    /* Check connect bit status */

    return;
}

extern char __heap_base__; /* Defined by the linker */
extern char __heap_limit__; /* Defined by the linker */

void target_reset(void)
{
    /* Configure PLL, switch from IRC to Main OSC */
    target_configure();

    /* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
    PCLKSEL0 = 0x00000000;    /* PCLK is 1/4 CCLK */
    PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    PCLKSEL0 = 0xAAAAAAAA;    /* PCLK is 1/2 CCLK */
    PCLKSEL1 = 0xAAAAAAAA;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    PCLKSEL0 = 0x55555555;    /* PCLK is the same as CCLK */
    PCLKSEL1 = 0x55555555;
#endif

    /* Set memory accelerate module*/
    //FLASHCTRL &= ~(1 << 12);
    //FLASHCTRL |= (1 << 13);

    //FLASHCTRL = 0;
    //FLASHTIM = 0x04; // 4 CCLK for flash instruction fetch
    //FLASHCTRL = 2;         // fully enabled

    target_init();

    printf("\nHeap start %p end %p  size %d\n",
           &__heap_base__, &__heap_limit__,
           (int)(&__heap_limit__ - &__heap_base__));

    return;
}

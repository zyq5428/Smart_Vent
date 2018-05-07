/*
 * led.c
 *
 *  Created on: 2018Äê5ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#include <msp430.h>

        /* P4.3 is Three-color light R pin,low on,height off */
        /* P4.4 is Three-color light B pin,low on,height off */
        /* P4.5 is Three-color light G pin,low on,height off */

void led_off(void)
{
    P4OUT |= (BIT3 | BIT4 | BIT5);
}

void red_on(void)
{
    P4OUT &= ~BIT3;
}

void red_off(void)
{
    P4OUT |= BIT3;
}

void green_on(void)
{
    P4OUT &= ~BIT5;
}

void green_off(void)
{
    P4OUT |= BIT5;
}

void blue_on(void)
{
    P4OUT &= ~BIT4;
}

void blue_off(void)
{
    P4OUT |= BIT4;
}

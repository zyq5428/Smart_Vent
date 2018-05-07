/*
 * rtc.c
 *
 *  Created on: 2018Äê5ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#include <msp430.h>

void rtc_init(int second)
{
    int n = second;
    // Initialize RTC
    // RTC count re-load compare value at 32.
    // 1024/32768 * 32 * n = 1 * n sec.
    RTCMOD = (n * 32) - 1;
    // Source = 32kHz crystal, divided by 1024
    RTCCTL = RTCSS__XT1CLK | RTCSR | RTCPS__1024 | RTCIE;
}

// RTC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(RTCIV,RTCIV_RTCIF))
    {
        case  RTCIV_NONE:   break;          // No interrupt
        case  RTCIV_RTCIF:                  // RTC Overflow
            P4OUT ^= BIT5;
            break;
        default: break;
    }
}

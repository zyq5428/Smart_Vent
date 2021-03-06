/*
 * rtc.c
 *
 *  Created on: 2018��5��7��
 *      Author: johnsonzhou
 */

#include "rtc.h"
#include "led.h"
#include "i2c_hw.h"
#include "hp203b.h"
#include "bat_detect.h"
#include "main.h"
#include "angle.h"

extern unsigned int Global_Flag;

int wake_num = 0;

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

void rtc_int_isr(void)
{
    switch(wake_num) {
    case 0:

        //green_on();
        bat_detect();
        hp203b_init();
        angle_test();
        wake_num++;
        led_off();
        break;
    case 1:

        wake_num = 0;
        bat_detect();
        hp203b_init();
        angle_test();
        led_off();
        break;
    default:
        break;
    }
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
            Global_Flag |= RTC_INT_Flag;    //set flag
            __bic_SR_register_on_exit(LPM3_bits);            // Clear CPUOFF bit from LPM0
            break;
        default: break;
    }
}

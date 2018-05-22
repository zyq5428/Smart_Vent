/*
 * timer_delay.c
 *
 *  Created on: 2018Äê5ÔÂ8ÈÕ
 *      Author: johnsonzhou
 */
#include <msp430.h>
#include "power_on.h"

extern struct vent_info vent;

void timer_start(void)
{
    TA1CCR0 = 0xffff-1;                             // Time is 1ms
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;  // ACLK, upmode, clear TAR, enable interrupt
}

void timer_end(void)
{
    unsigned int temp = 0;
    temp = TA1R;                             // Time is temp / 0x21 (ms)
    temp = temp / 0x21;
    if (temp == 0)
        temp = 1;
    vent.MAX_Time = temp;
    TA1CTL = TASSEL__ACLK | MC_0 | TACLR | TAIE;
}

void delay_hw_ms(int delay_ms)
{
    int n;
    for (n=delay_ms; n > 0; n--) {
        TA1CCR0 = 0x21;                             // Time is 1ms
        TA1CTL = TASSEL__ACLK | MC__UP | TACLR | TAIE;  // ACLK, upmode, clear TAR, enable interrupt
          __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, enable interrupts
    }
}

void delay_hw_s(int delay_s)
{
    int n;
    for (n=delay_s; n > 0; n--) {
        TA1CCR0 = 0x7FFF;                               // Time is 1s
        TA1CTL = TASSEL__ACLK | MC__UP | TACLR | TAIE;  // ACLK, upmode, clear TAR, enable interrupt
          __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, enable interrupts
    }
}

// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA1IV, TA1IV_TAIFG))
  {
    case TA1IV_NONE:   break;               // No interrupt
    case TA1IV_TACCR1: break;               // CCR1 not used
    case TA1IV_TACCR2: break;               // CCR2 not used
    case TA1IV_3:      break;               // reserved
    case TA1IV_4:      break;               // reserved
    case TA1IV_5:      break;               // reserved
    case TA1IV_6:      break;               // reserved
    case TA1IV_TAIFG:                       // overflow
      TA1CTL = TASSEL__ACLK | MC_0 | TACLR | TAIE;
      __bic_SR_register_on_exit(LPM3_bits);
      break;
    default: break;
  }
}




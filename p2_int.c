/*
 * p2_int.c
 *
 *  Created on: 2018Äê5ÔÂ17ÈÕ
 *      Author: johnsonzhou
 */

#include "p2_int.h"
#include "led.h"
#include "motor.h"
#include "power_on.h"
#include "angle.h"

extern struct angle_info angle;
extern struct vent_info vent;

void p2_int_init(void)
{
    P2IFG = 0x0;    //clear P2IFGs
    P2IE = 0x30;
}

// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    switch (P2IFG) {
    case BIT0:
        P2IFG &= ~BIT0;
        break;
    case BIT1:
        P2IFG &= ~BIT1;
        break;
    case BIT2:
        P2IFG &= ~BIT2;
        break;
    case BIT3:
        P2IFG &= ~BIT3;
        break;
    case BIT4:      /* P2.4 is LIMIT1 (Vent open), Falling edge*/
        P2IFG &= ~BIT4;
        motor_stop_operate();
        if (vent.limit_open_flag == ERROR) {
            angle.value_open = read_angle_value();
            vent.limit_open_flag = OK;
        }
        led_off();
        green_on();
        break;
    case BIT5:      /* P2.5 is LIMIT2(Vent close), Falling edge*/
        P2IFG &= ~BIT5;
        motor_stop_operate();
        if (vent.init_flag == ERROR) {
            angle.value_close = read_angle_value();
            vent.limit_close_flag = OK;
        }
        led_off();
        blue_on();
        break;
    case BIT6:
        P2IFG &= ~BIT6;
        break;
    case BIT7:
        P2IFG &= ~BIT7;
        break;
    default: break;
    }
}

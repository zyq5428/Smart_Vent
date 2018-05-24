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
extern struct motor_info motor;

void p2_int_init(void)
{
    P2IFG = 0x0;    //clear P2IFGs
    P2IE = 0x30;
}

void open_int_en(void)
{
    P2IFG &= ~BIT4;    //clear P2IFGs
    P2IE |= BIT4;
}

void open_int_off(void)
{
    P2IFG &= ~BIT4;    //clear P2IFGs
    P2IE &= ~BIT4;
}

void close_int_en(void)
{
    P2IFG &= ~BIT5;    //clear P2IFGs
    P2IE |= BIT5;
}

void close_int_off(void)
{
    P2IFG &= ~BIT5;    //clear P2IFGs
    P2IE &= ~BIT5;
}

void limit_int_en(void)
{
    P2IFG &= ~(BIT4 | BIT5);    //clear P2IFGs
    P2IE |= BIT4 | BIT5;
}

void limit_int_off(void)
{
    P2IFG &= ~(BIT4 | BIT5);    //clear P2IFGs
    P2IE &= ~(BIT4 | BIT5);
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
        //__delay_cycles(10000);             // Delay for n*(1/MCLK(8000000)=0.1s
        if (!(P2IN & BIT4)) {
            if (motor.motor_flag == open_flag) {
                motor_stop_operate();
                motor.stop_flag = open_limit_stop;
                if (vent.limit_open_flag == ERROR) {
                    angle.value_open = read_angle_value();
                    vent.limit_open_flag = OK;
                }
                led_off();
                green_on();
            }
        }
        break;

    case BIT5:      /* P2.5 is LIMIT2(Vent close), Falling edge*/
        P2IFG &= ~BIT5;
        //__delay_cycles(10000);             // Delay for n*(1/MCLK(8000000)=0.1s
        if (!(P2IN & BIT5)) {
            if (motor.motor_flag == close_flag) {
                motor_stop_operate();
                motor.stop_flag = close_limit_stop;
                if (vent.limit_close_flag == ERROR) {
                    angle.value_close = read_angle_value();
                    vent.limit_close_flag = OK;
                }
                led_off();
                blue_on();
            }
        }
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

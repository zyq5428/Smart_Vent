/*
 * power_on.c
 *
 *  Created on: 2018Äê5ÔÂ18ÈÕ
 *      Author: johnsonzhou
 */

#include "power_on.h"
#include "timer_delay.h"
#include "motor.h"
#include "led.h"
#include "angle.h"
#include "rtc.h"
#include "main.h"
#include "p2_int.h"

struct vent_info vent;
extern unsigned int Global_Flag;

void vent_info_init(void)
{
    vent.init_flag = ERROR;
    vent.limit_open_flag = ERROR;
    vent.limit_close_flag = ERROR;
    vent.MAX_Time = 1000;      //unit is ms
}

void limit_error(void)
{
    led_off();
    red_on();
}

void vent_self_test(void)
{
    __bis_SR_register(GIE);     // General interrupt enable

    delay_hw_s(1);

    vent_info_init();

    angle_en();

    timer_start();

    motor_init(motor_stop, 10000, 90);
    pwm_init();

    if (!((P2IN & BIT4) == 0)) {
        vent_open();
        while ((Global_Flag & OPEN_INT_Flag) == 0) {
            limit_error();
        }
        open_int_isr();
        Global_Flag &= ~(OPEN_INT_Flag);    //Clear flag when finished
    }

    vent_close();
    while ((Global_Flag & CLOSE_INT_Flag) == 0)
        limit_error();
    close_int_isr();
    Global_Flag &= ~(CLOSE_INT_Flag);    //Clear flag when finished

    if (vent.limit_open_flag == ERROR) {
        vent_open();
        while ((Global_Flag & OPEN_INT_Flag) == 0)
            limit_error();
        open_int_isr();
        Global_Flag &= ~(OPEN_INT_Flag);    //Clear flag when finished
    }

    vent.init_flag = OK;

    timer_end();

    Angle_correction();

    angle_off();

    rtc_init(15);
}

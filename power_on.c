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

struct vent_info vent;

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

    vent_info_init();

    angle_en();

    timer_start();

    motor_init(motor_stop, 10000, 80);
    pwm_init();

    if (!((P2IN & BIT4) == 0)) {
        vent_open();
        while (vent.limit_open_flag == ERROR) {
            limit_error();
        }
    }

    vent_close();
    while (vent.limit_close_flag == ERROR) {
        limit_error();
    }

    if (vent.limit_open_flag == ERROR) {
        vent_open();
        while (vent.limit_open_flag == ERROR) {
            limit_error();
        }
    }

    vent.init_flag = OK;

    timer_end();

    Angle_correction();

    angle_off();
}

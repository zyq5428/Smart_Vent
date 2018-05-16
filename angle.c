/*
 * angle.c
 *
 *  Created on: 2018Äê5ÔÂ16ÈÕ
 *      Author: johnsonzhou
 */

#include "angle.h"
#include "adc10.h"
#include "led.h"

struct angle_info angle;

void angle_info_init(void)
{
    angle.value_current = 0;
    angle.value_open = 0;
    angle.value_closs = 0;
    angle.value_0 = 0;
    angle.value_30 = 0;
    angle.value_60 = 0;
    angle.value_90 = 0;
    angle.value_deviation = 0;
    angle.High_Threshold = 682;
    angle.Low_Threshold = 341;
}

void angle_measure(void)
{
    angle_info_init();

    // Configure ADC A1 pin
    SYSCFG2 |= ADCPCTL9;

    angle.value_current = adc_window_comparator_vcc(ADCINCH_9, angle.High_Threshold, angle.Low_Threshold);
}

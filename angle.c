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

#define angle_num 10
unsigned int angle_value[angle_num];

void angle_en(void)
{
    P3OUT |= BIT0;                   // en angle power
}

void angle_off(void)
{
    P3OUT &= ~BIT0;                   // off angle power
}

void angle_info_init(void)
{
    angle.value_current = 0;
    angle.value_open = 0;
    angle.value_close = 0;
    angle.value_0 = 0;
    angle.value_30 = 0;
    angle.value_60 = 0;
    angle.value_90 = 0;
    angle.value_deviation = 0;
    angle.one_degree_adc = 0;
    angle.High_Threshold = 682;
    angle.Low_Threshold = 341;
}

void angle_measure(void)
{
    angle_info_init();

    // Configure ADC A9 pin
    SYSCFG2 |= ADCPCTL9;

    angle.value_current = adc_window_comparator_vcc(ADCINCH_9, angle.High_Threshold, angle.Low_Threshold);
}

void Angle_correction(void)
{
    unsigned int temp;

    if (angle.value_open > angle.value_close) {
        temp = angle.value_open - angle.value_close;
        angle.one_degree_adc = temp / 90;
        angle.value_0 = angle.value_close + (angle.one_degree_adc * 3);
        angle.value_30 = angle.value_close + (angle.one_degree_adc * 30);
        angle.value_60 = angle.value_close + (angle.one_degree_adc * 60);
        angle.value_90 = angle.value_open - (angle.one_degree_adc * 3);
    } else {
        temp = angle.value_close - angle.value_open;
        angle.one_degree_adc = temp / 90;
        angle.value_0 = angle.value_close - (angle.one_degree_adc * 3);
        angle.value_30 = angle.value_close - (angle.one_degree_adc * 30);
        angle.value_60 = angle.value_close - (angle.one_degree_adc * 60);
        angle.value_90 = angle.value_open + (angle.one_degree_adc * 3);
    }
}

unsigned int read_angle_value(void)
{
    unsigned char i;
    unsigned char num = 10;
    unsigned int angle_temp = 0;

    //num = angle_num;

    // Configure ADC A9 pin
    SYSCFG2 |= ADCPCTL9;

    adc_repeat_single_channel_vcc(ADCINCH_9, 10, angle_value);

    for (i = 0; i < num; i++) {
        angle_temp = angle_temp + angle_value[i];
    }
    angle_temp = angle_temp / num;

    return angle_temp;
}

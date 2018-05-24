/*
 * angle.c
 *
 *  Created on: 2018Äê5ÔÂ16ÈÕ
 *      Author: johnsonzhou
 */

#include "angle.h"
#include "adc10.h"
#include "led.h"
#include "motor.h"
#include "timer_delay.h"

struct angle_info angle;
extern struct motor_info motor;
extern struct vent_info vent;

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

#if (0)
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
#endif

#if (1)
void Angle_correction(void)
{
    unsigned int temp;

    temp = angle.value_close - angle.value_open;
    angle.one_degree_adc = (float)temp / 90;
    angle.value_0 = angle.value_close - (unsigned int)(angle.one_degree_adc * 3);
    angle.value_30 = angle.value_close - (unsigned int)(angle.one_degree_adc * 30);
    angle.value_60 = angle.value_close - (unsigned int)(angle.one_degree_adc * 60);
    angle.value_90 = angle.value_open + (unsigned int)(angle.one_degree_adc * 3);
}
#endif

unsigned int read_angle_value(void)
{
    unsigned char i;
    unsigned char num = 10;
    unsigned int angle_temp = 0;

    //num = angle_num;

    // Configure ADC A9 pin
    SYSCFG2 |= ADCPCTL9;

    adc_repeat_single_channel_vcc(ADCINCH_9, 10, angle_value);

    for (i = 1; i < num; i++) {
        angle_temp = angle_temp + angle_value[i];
    }
    angle_temp = angle_temp / (num - 1);

    return angle_temp;
}

unsigned int vent_operate(unsigned int angle_value)
{
    angle.High_Threshold = angle_value + (unsigned int)(angle.value_deviation * angle.one_degree_adc);
    angle.Low_Threshold = angle_value - (unsigned int)(angle.value_deviation * angle.one_degree_adc);

    if (angle.value_current < angle.Low_Threshold) {
        motor.stop_flag = default_value;
        adc_window_comparator_vcc(ADCINCH_9, angle.High_Threshold, angle.Low_Threshold);
        vent_close();
        delay_hw_ms(800);
        if (motor.stop_flag != default_value) {
            motor_stop_operate();
            motor.stop_flag = time_out_stop;
        }
    } else if((angle.value_current < angle.High_Threshold) && (angle.value_current > angle.Low_Threshold)) {
        return angle.value_current;
    } else if (angle.value_current > angle.High_Threshold) {
        motor.stop_flag = default_value;
        adc_window_comparator_vcc(ADCINCH_9, angle.High_Threshold, angle.Low_Threshold);
        vent_open();
        delay_hw_ms(800);
        if (motor.stop_flag != default_value) {
            motor_stop_operate();
            motor.stop_flag = time_out_stop;
        }
    }

    angle.value_current = read_angle_value();
    return angle.value_current;
}

void angle_test(void)
{
    //delay_hw_s(30);
    vent_operate(angle.value_30);
    //delay_hw_s(30);
    vent_operate(angle.value_60);
    //delay_hw_s(30);
    vent_operate(angle.value_90);
    //delay_hw_s(30);
    vent_operate(angle.value_0);
}

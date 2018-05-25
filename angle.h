/*
 * angle.h
 *
 *  Created on: 2018Äê5ÔÂ16ÈÕ
 *      Author: johnsonzhou
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include <msp430.h>

struct angle_info {
    unsigned int value_current;
    unsigned int value_open;
    unsigned int value_close;
    unsigned int value_0;
    unsigned int value_30;
    unsigned int value_60;
    unsigned int value_90;
    unsigned int value_deviation;
    unsigned int value_margin;
    float one_degree_adc;
    unsigned int High_Threshold;
    unsigned int Low_Threshold;
};

void angle_en(void);
void angle_off(void);
void angle_info_init(void);
void angle_measure(void);
void Angle_correction(void);
unsigned int read_angle_value(void);
unsigned int vent_operate(unsigned int angle_value);
void angle_test(void);

#endif /* ANGLE_H_ */

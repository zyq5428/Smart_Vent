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
    unsigned int value_closs;
    unsigned int value_0;
    unsigned int value_30;
    unsigned int value_60;
    unsigned int value_90;
    unsigned int value_deviation;
    unsigned int High_Threshold;
    unsigned int Low_Threshold;
};

void angle_measure(void);

#endif /* ANGLE_H_ */

/*
 * power_on.h
 *
 *  Created on: 2018Äê5ÔÂ18ÈÕ
 *      Author: johnsonzhou
 */

#ifndef POWER_ON_H_
#define POWER_ON_H_

#include <msp430.h>

#define OK  0
#define ERROR   1

struct vent_info {
    unsigned char init_flag;
    unsigned char limit_open_flag;
    unsigned char limit_close_flag;
    unsigned int MAX_Time;
};

void vent_self_test(void);

#endif /* POWER_ON_H_ */

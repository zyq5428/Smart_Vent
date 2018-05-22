/*
 * motor.h
 *
 *  Created on: 2018Äê5ÔÂ18ÈÕ
 *      Author: johnsonzhou
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <msp430.h>

#define motor_stop 0
#define motor_Forward 1
#define motor_Reverse 2
#define motor_aberrant 3

#define open_flag motor_Reverse
#define close_flag motor_Forward

struct motor_info {
    unsigned char motor_flag;
    unsigned int motor_freq;
    unsigned int motor_duty;
};

struct pwm_info {
    unsigned int period_value;
    unsigned int high_value;
};

void motor_init(unsigned char flag, unsigned int freq, unsigned int duty);
void pwm_init(void);
void motor_stop_operate(void);
void motor_open(void);
void vent_open(void);
void vent_close(void);


#endif /* MOTOR_H_ */

/*
 * motor.h
 *
 *  Created on: 2018��5��18��
 *      Author: johnsonzhou
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <msp430.h>

#define motor_stop 0
#define motor_Forward 1
#define motor_Reverse 2
#define motor_aberrant 3

#define default_value 0
#define open_limit_stop 1
#define close_limit_stop 2
#define angle_match_stop 3
#define time_out_stop 4

#define open_flag motor_Reverse
#define close_flag motor_Forward

struct motor_info {
    unsigned char motor_flag;
    unsigned int motor_freq;
    unsigned int motor_duty;
    unsigned char stop_flag;
    unsigned int Forward_counter;
    unsigned int Reverse_counter;
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

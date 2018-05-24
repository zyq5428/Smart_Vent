/*
 * timer_delay.h
 *
 *  Created on: 2018Äê5ÔÂ8ÈÕ
 *      Author: johnsonzhou
 */

#ifndef TIMER_DELAY_H_
#define TIMER_DELAY_H_

#include <msp430.h>

void delay_hw_ms(unsigned int delay_ms);

void delay_hw_s(unsigned int delay_s);

void timer_start(void);

void timer_end(void);

#endif /* TIMER_DELAY_H_ */

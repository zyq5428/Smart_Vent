/*
 * rtc.h
 *
 *  Created on: 2018Äê5ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#ifndef RTC_H_
#define RTC_H_

#include <msp430.h>

void rtc_init(int second);

void rtc_int_isr(void);

#endif /* RTC_H_ */

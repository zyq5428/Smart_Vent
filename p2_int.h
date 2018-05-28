/*
 * p2_int.h
 *
 *  Created on: 2018Äê5ÔÂ17ÈÕ
 *      Author: johnsonzhou
 */

#ifndef P2_INT_H_
#define P2_INT_H_

#include <msp430.h>

void p2_int_init(void);

void open_int_en(void);

void open_int_off(void);

void close_int_en(void);

void close_int_off(void);

void limit_int_en(void);

void limit_int_off(void);

void open_int_isr(void);

void close_int_isr(void);

void cc1310_int_isr(void);

#endif /* P2_INT_H_ */

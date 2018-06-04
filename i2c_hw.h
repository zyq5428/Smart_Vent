/*
 * i2c_hw.h
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#ifndef I2C_HW_H_
#define I2C_HW_H_

#include <msp430.h>

void i2c_init(void);
void i2c_reset(void);
void i2c_Write(unsigned char addr, unsigned char *pstr, int num);
void i2c_read(unsigned char addr, unsigned char *pstr, int num);

#endif /* I2C_HW_H_ */

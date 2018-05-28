/*
 * main.h
 *
 *  Created on: 2018Äê5ÔÂ28ÈÕ
 *      Author: johnsonzhou
 */

#ifndef MAIN_H_
#define MAIN_H_


#define SELF_TEST_Flag              (0x8000)       /* OPEN limit int */
#define OPEN_INT_Flag               (0x0001)       /* OPEN limit int */
#define CLOSE_INT_Flag              (0x0002)       /* CLOSE limit int */
#define RTC_INT_Flag                (0x0004)       /* RTC int */
#define CC1310_INT_Flag             (0x0008)       /* 915 int */
#define UART_RX_Flag                (0x0010)       /* UART RX int */
#define UART_TX_Flag                (0x0020)       /* UART TX int */
#define I2C_RX_Flag                 (0x0040)       /* I2C RX int */
#define I2C_TX_Flag                 (0x0080)       /* I2C TX int */


#endif /* MAIN_H_ */

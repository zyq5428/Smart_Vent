/*
 * sht3x.h
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#ifndef HP203B_H_
#define HP203B_H_

#include <msp430.h>
#include "i2c_hw.h"

#define ADC_CVT_H 0x02

#define CHNL_PT 0x00
#define CHNL_T  0x02

#define OSR_4096 0x00
#define OSR_2048 0x01
#define OSR_1024 0x02
#define OSR_512 0x03
#define OSR_256 0x04
#define OSR_128 0x05

struct adc_cvt_struct {
    unsigned char h;
    unsigned char osr;
    unsigned char chnl;
};

struct hp203b_struct {
    unsigned char Address;
    unsigned char soft_rst_cmd;
    unsigned char osr_ch_cmd;
    unsigned char read_pt_cmd;
    unsigned char read_p_cmd;
    unsigned char read_t_cmd;
    unsigned char Temp_data[3];
    unsigned char press_data[3];
    long int temp;
    long int press;
};

void hp203b_init(void);

#endif /* HP203B_H_ */

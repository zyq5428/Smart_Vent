/*
 * sht3x.c
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#include "hp203b.h"
#include "timer_delay.h"

struct adc_cvt_struct adc_cvt = {
    ADC_CVT_H,
    OSR_1024,
    0,
};

struct hp203b_struct hp203b = {
    0x76,
    0x6,
    ((0x2 << 5) | (0x2 << 2) | 0x0), //OSR=1024,Temperature and Pressure Channels
    0x10, //read Pressure and Temperature value
    0x30, //read Pressure value
    0x32, //read Temperature value
    {0, 0, 0},
    {0, 0, 0},
    0,
    0,
};

unsigned char hp203b_cmd[3];
unsigned char hp203b_data[6];
unsigned char hp203b_press_data[3];
unsigned char hp203b_temp_data[3];

void hp203b_off(void)
{
    P3OUT &= ~BIT2;                   // off sensor power
}

void hp203b_on(void)
{
    P3OUT |= BIT2;                   // on sensor power
}

void hp203b_rst(void)
{
    hp203b_cmd[0] = hp203b.soft_rst_cmd;
    i2c_Write(hp203b.Address, hp203b_cmd, 1);
}

void hp203b_adc_cvt(void)
{
    hp203b.osr_ch_cmd = (adc_cvt.h << 5) | (adc_cvt.osr << 2) | (adc_cvt.chnl);
    hp203b_cmd[0] = hp203b.osr_ch_cmd;
    i2c_Write(hp203b.Address, hp203b_cmd, 1);
}

void hp203b_read_pt(void)
{
    hp203b_cmd[0] = hp203b.read_pt_cmd;
    i2c_Write(hp203b.Address, hp203b_cmd, 1);
    i2c_read(hp203b.Address, hp203b_data, 6);
}

void hp203b_read_p(void)
{
    hp203b_cmd[0] = hp203b.read_p_cmd;
    i2c_Write(hp203b.Address, hp203b_cmd, 1);
    i2c_read(hp203b.Address, hp203b_press_data, 3);
}

void hp203b_read_t(void)
{
    hp203b_cmd[0] = hp203b.read_t_cmd;
    i2c_Write(hp203b.Address, hp203b_cmd, 1);
    i2c_read(hp203b.Address, hp203b_temp_data, 3);
}

void get_press_temp(void)
{
    adc_cvt.chnl = CHNL_PT;
    hp203b_on();
    delay_hw_ms(10);
    hp203b_rst();
    delay_hw_ms(10);
    hp203b_adc_cvt();
    delay_hw_ms(100);
    hp203b_read_pt();
    hp203b.temp = ((hp203b_data[0] << 16) | (hp203b_data[1] << 8) | (hp203b_data[2] << 0)) & 0xfffff;
    hp203b.temp = (hp203b.temp + 50) / 100;
    hp203b.press = ((hp203b_data[3] << 16) | (hp203b_data[4] << 8) | (hp203b_data[5] << 0)) & 0xfffff;
    hp203b.press = (hp203b.press + 50) / 100;
}

void get_press(void)
{
    adc_cvt.chnl = CHNL_PT;
    hp203b_on();
    delay_hw_ms(10);
    hp203b_rst();
    delay_hw_ms(10);
    hp203b_adc_cvt();
    delay_hw_ms(100);
    hp203b_read_p();
    hp203b.press = ((hp203b_press_data[0] << 16) | (hp203b_press_data[1] << 8) | (hp203b_press_data[2] << 0)) & 0xfffff;
    hp203b.press = (hp203b.press + 50) / 100;
}

void get_temp(void)
{
    adc_cvt.chnl = CHNL_T;
    hp203b_on();
    delay_hw_ms(10);
    hp203b_rst();
    delay_hw_ms(10);
    hp203b_adc_cvt();
    delay_hw_ms(100);
    hp203b_read_t();
    hp203b.temp = ((hp203b_temp_data[0] << 16) | (hp203b_temp_data[1] << 8) | (hp203b_temp_data[2] << 0)) & 0xfffff;
    hp203b.temp = (hp203b.temp + 50) / 100;
}

void hp203b_init(void)
{
    get_press_temp();
}


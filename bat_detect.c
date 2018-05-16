/*
 * bat_detect.c
 *
 *  Created on: 2018Äê5ÔÂ11ÈÕ
 *      Author: johnsonzhou
 */

#include "bat_detect.h"
#include "led.h"
#include "adc10.h"

struct bat_info bat;

unsigned int Bat_Elec[30];

void bat_info_init(void)
{
    bat.detect_avg_num = 10;                //Set the number of detects
    bat.Bat_Elec_ADC = 0;
    bat.Alarm_Elec = 3.5;                   //There is an error in the voltage divider circuit and it is necessary to manually test the voltage division value of the alarm voltage.
    bat.Alarm_Elec_ADC = (unsigned int)((bat.Alarm_Elec / 3 / 1.5) *1024);
    bat.Alarm_flag = 0;
}

void read_bat_Elec(void)
{
    bat.Bat_Elec = bat.Bat_Elec_ADC * 0.00439;
}

unsigned char bat_detect(void)
{
    unsigned char i;

    //bat_info_init();

    // Configure ADC A5 pin
    SYSCFG2 |= ADCPCTL5;

    adc_repeat_single_channel_1v5(ADCINCH_5, bat.detect_avg_num, Bat_Elec);

    for (i = 0; i < bat.detect_avg_num; i++) {
        bat.Bat_Elec_ADC = bat.Bat_Elec_ADC + Bat_Elec[i];
    }
    bat.Bat_Elec_ADC = bat.Bat_Elec_ADC / bat.detect_avg_num;

    if (bat.Bat_Elec_ADC < bat.Alarm_Elec_ADC) {              //3.5V / 3 = 1.17V = 0x31e
        bat.Alarm_flag = 1;
        red_on();
    } else {
        bat.Alarm_flag = 0;
        red_off();
    }

    read_bat_Elec();

    return bat.Alarm_flag;
}


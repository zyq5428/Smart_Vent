/*
 * bat_detect.h
 *
 *  Created on: 2018Äê5ÔÂ11ÈÕ
 *      Author: johnsonzhou
 */

#ifndef BAT_DETECT_H_
#define BAT_DETECT_H_

#include <msp430.h>

struct bat_info {
    float Bat_Elec;
    unsigned int Bat_Elec_ADC;
    float Alarm_Elec;
    unsigned int Alarm_Elec_ADC;
    unsigned char Alarm_flag;
    unsigned char detect_avg_num;
};

void bat_info_init(void);

void read_bat_Elec(void);

unsigned char bat_detect(void);


#endif /* BAT_DETECT_H_ */

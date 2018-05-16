/*
 * adc10.h
 *
 *  Created on: 2018Äê5ÔÂ16ÈÕ
 *      Author: johnsonzhou
 */

#ifndef ADC10_H_
#define ADC10_H_

#include <msp430.h>

unsigned char adc_repeat_single_channel_1v5(unsigned int adc_ch, unsigned char num, unsigned int *Result);

unsigned int adc_window_comparator_vcc(unsigned int adc_ch, unsigned int High_Threshold, unsigned int Low_Threshold);


#endif /* ADC10_H_ */

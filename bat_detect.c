/*
 * bat_detect.c
 *
 *  Created on: 2018Äê5ÔÂ11ÈÕ
 *      Author: johnsonzhou
 */

#include "bat_detect.h"
#include "led.h"

struct bat_info bat;

unsigned int *ADC_Result;
unsigned char ADC_Count;

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

    ADC_Result = Bat_Elec;

    //bat_info_init();

    ADC_Count = bat.detect_avg_num;

    // Configure ADC A5 pin
    SYSCFG2 |= ADCPCTL5;

    // Configure ADC10
    ADCCTL0 |= ADCSHT_2 | ADCMSC | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP | ADCCONSEQ_2;                                  // ADCCLK = MODOSC; sampling timer, Repeat-single-channel
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_5 | ADCSREF_1;                       // A5 ADC input select; Vref=1.5V
    ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

    // Configure reference module located in the PMM
    PMMCTL0_H = PMMPW_H;                    // Unlock the PMM registers
    PMMCTL2 |= INTREFEN;                    // Enable internal reference
    while(!(PMMCTL2 & REFGENRDY));          // Poll till internal reference settles

    ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
    __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit

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

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            if (ADC_Count > 1) {
                *ADC_Result++ = ADCMEM0;
                ADC_Count--;
                break;
            } else {
                *ADC_Result++ = ADCMEM0;
                ADCCTL0 &= ~ADCENC;
                __bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
                break;
            }
        default:
            break;
    }
}


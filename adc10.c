/*
 * adc10.c
 *
 *  Created on: 2018Äê5ÔÂ16ÈÕ
 *      Author: johnsonzhou
 */

#include "adc10.h"
#include "led.h"

unsigned int *ADC_Result;
unsigned char ADC_Count;

void adc_window_isr(void)
{

}

unsigned char adc_repeat_single_channel_1v5(unsigned int adc_ch, unsigned char num, unsigned int *Result)
{
    ADC_Count = num;
    ADC_Result = Result;

    // Configure ADC10
    ADCCTL0 |= ADCSHT_2 | ADCMSC | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP | ADCCONSEQ_2;                                  // ADCCLK = MODOSC; sampling timer, Repeat-single-channel
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= adc_ch | ADCSREF_1;                       // A5 ADC input select; Vref=1.5V
    ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

    // Configure reference module located in the PMM
    PMMCTL0_H = PMMPW_H;                    // Unlock the PMM registers
    PMMCTL2 |= INTREFEN;                    // Enable internal reference
    while(!(PMMCTL2 & REFGENRDY));          // Poll till internal reference settles

    ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
    __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit

    return ADC_Count;
}

unsigned char adc_repeat_single_channel_vcc(unsigned int adc_ch, unsigned char num, unsigned int *Result)
{
    ADC_Count = num;
    ADC_Result = Result;

    // Configure ADC10
    ADCCTL0 |= ADCSHT_2 | ADCMSC | ADCON;                             // ADCON, S&H=16 ADC clks
    ADCCTL1 |= ADCSHP | ADCCONSEQ_2;                                  // ADCCLK = MODOSC; sampling timer, Repeat-single-channel
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= adc_ch | ADCSREF_0;                       // A5 ADC input select; Vref=1.5V
    ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

    // Configure reference module located in the PMM
    PMMCTL0_H = PMMPW_H;                    // Unlock the PMM registers
    PMMCTL2 |= INTREFEN;                    // Enable internal reference
    while(!(PMMCTL2 & REFGENRDY));          // Poll till internal reference settles

    ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
    __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit

    return ADC_Count;
}

unsigned int adc_window_comparator_vcc(unsigned int adc_ch, unsigned int High_Threshold, unsigned int Low_Threshold)
{
    // Configure ADC;
    ADCCTL0 = ADCSHT_2 | ADCMSC | ADCON;                // ADCON
    ADCCTL1 = ADCSHP | ADCSHS_0 | ADCCONSEQ_2;          // Rpt single ch; TA1.1 trigger
    ADCCTL2 = ADCRES;                                   // 10-bit conversion results
    ADCMCTL0 = adc_ch | ADCSREF_0;                   // Vref AVCC, A9
    ADCHI = High_Threshold;                             // Window Comparator Hi-threshold
    ADCLO = Low_Threshold;                              // Window Comparator Lo-threshold
    ADCIE |= ADCHIIE | ADCLOIE | ADCINIE;               // Enable ADC conv complete interrupt
    //ADCIE |= ADCINIE;               // Enable ADC conv complete interrupt

    ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
    __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit

    return *ADC_Result;

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
            ADCIFG &= ~ADCHIIFG;                        // Clear interrupt flag
            P4OUT |= BIT3 | BIT4 |BIT5;
            P4OUT &= ~BIT3;
            break;
        case ADCIV_ADCLOIFG:
            ADCIFG &= ~ADCLOIFG;                        // Clear interrupt flag
            P4OUT |= BIT3 | BIT4 |BIT5;
            P4OUT &= ~BIT4;
            break;
        case ADCIV_ADCINIFG:
            ADCIFG &= ~ADCINIFG;                        // Clear interrupt flag
            *ADC_Result = ADCMEM0;
            adc_window_isr();
            P4OUT |= BIT3 | BIT4 |BIT5;
            P4OUT &= ~BIT5;
            //ADCCTL0 &= ~ADCENC;
            //__bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
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




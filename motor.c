/*
 * motor.c
 *
 *  Created on: 2018Äê5ÔÂ18ÈÕ
 *      Author: johnsonzhou
 */

#include "motor.h"

struct motor_info motor;
struct pwm_info pwm;

void motor_init(unsigned char flag, unsigned int freq, unsigned int duty)
{
    motor.motor_flag = flag;
    motor.motor_freq = freq;   //Motor control PWM wave frequency is 10000Hz
    motor.motor_duty = duty;      //Motor control PWM wave duty is 80%
}

void pwm_init(void)
{
    pwm.period_value = 8000000 / motor.motor_freq;  //value = 8MHz / f
    pwm.high_value = pwm.period_value * motor.motor_duty / 100;
}

void motor_stop_operate(void)
{
    P3OUT &= ~(BIT1 | BIT7);                   // off sensor power
    TA0CTL = MC__STOP | TACLR;  // stop and  TAR
    P1DIR |= BIT6 | BIT7;                     // P1.6 and P1.7 output
    P1SEL0 &= ~(BIT6 | BIT7);                    // P1.6 and P1.7 options select
    P1OUT &= ~(BIT6 | BIT7);                    //output low value
}

/*P1.7 is TA0.1, P1.6 is low*/
static void motor_Forward_start(void)
{
    P1DIR |= BIT6 | BIT7;                     // P1.6 and P1.7 output
    P1SEL0 |= BIT7;                    // P1.6 and P1.7 options select
    P1SEL0 &= ~BIT6;                    // P1.6 and P1.7 options select
    P1OUT &= ~BIT6;                    //output low value

    TA0CCR0 = pwm.period_value - 1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = pwm.high_value;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
}

/*P1.6 is TA0.2, P1.7 is low*/
static void motor_Reverse_start(void)
{
    P1DIR |= BIT6 | BIT7;                     // P1.6 and P1.7 output
    P1SEL0 |= BIT6;                    // P1.6 and P1.7 options select
    P1SEL0 &= ~BIT7;                    // P1.6 and P1.7 options select
    P1OUT &= ~BIT7;                    //output low value

    TA0CCR0 = pwm.period_value - 1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = pwm.high_value;                            // CCR1 PWM duty cycle
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
}

static void motor_aberrant_isr(void)
{

}

static void motor_power_en(void)
{
    P3OUT |= (BIT1 | BIT7);                   // output height
}

void motor_open(void)
{
    motor_power_en();
    switch (motor.motor_flag) {
    case motor_Forward:
        motor_Forward_start();
        break;
    case motor_Reverse:
        motor_Reverse_start();
        break;
    case motor_aberrant:
        motor_aberrant_isr();
        break;
    default:
        break;
    }
}

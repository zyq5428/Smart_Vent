/*
 * gpio.c
 *
 *  Created on: 2018Äê5ÔÂ4ÈÕ
 *      Author: johnsonzhou
 */
#include <msp430.h>

void initGpio(void)
{
    /* P1 configuration */
        /* P1.0 is UCA0RXD,P1.1 is UCA0TXD, connect CC1310 */
    P1SEL0 |= BIT0 | BIT1;                  // set P1.0,P1.1 pin as second function(UCA0UART)
        /* P1.2 P1.3 is Reserved connect CC1310 */
    P1DIR |= BIT2 | BIT3;                   // set P1.2,P1.3 pin output low
    P1REN |= BIT2 | BIT3;
    P1OUT &= ~(BIT2 | BIT3);
        /* P1.4 is unused */
    P1DIR |= BIT4;                   // set P1.4 pin output low
    P1REN |= BIT4;
    P1OUT &= ~(BIT4);
        /* P1.5 is Detect battery power as ADC */
    P1DIR &= ~BIT5;                   // set P1.5 pin input as A5
        /* P1.6 is TA0.2,P1.7 is TA0.1, control motor*/
    P1DIR |= BIT6 | BIT7;                   // set P1.6,P1.7 pin output PWM
    P1SEL0 |= BIT6 | BIT7;

    /* P2 configuration */
        /* P2.0 is connect HP203B INT0 interrupt, */
    P2DIR &= ~BIT0;                   // set P2.0 pin input with pullup resistor
    P2REN |= BIT0;
    P2OUT |= BIT0;
        /* P2.1 is connect CC1310 interrupt, Rising edge*/
    P2DIR &= ~BIT1;                   // set P2.1 pin input with pulldown resistor
    P2REN |= BIT1;
    P2OUT &= ~BIT1;
        /* P2.2 is BQ21040 CHG interrupt, Low (FET on) indicates that charging and Open Drain (FET off) indicate no Charging or Charge Complete*/
    P2DIR &= ~BIT2;                   // set P2.2 pin input with pullup resistor
    P2REN |= BIT2;
    P2OUT |= BIT2;
        /* P2.3 is BQ21040 DC_OK interrupt, Low indicates no DC insert and Height indicate DC insert*/
    P2DIR &= ~BIT3;                   // set P2.2 pin input with pulldown resistor
    P2REN |= BIT3;
    P2OUT &= ~BIT3;
        /* P2.4 is LIMIT1 (Vent open),P2.5 is LIMIT2(Vent close), Falling edge*/
    P2DIR &= ~(BIT4 | BIT5);                   // set P1.5 pin input with pullup resistor
    P2REN |= (BIT4 | BIT5);
    P2OUT |= (BIT4 | BIT5);
        /* P2.6 is Pairing button interrupt, Falling edge*/
    P2DIR &= ~BIT6;                   // set P2.2 pin input with pulldown resistor
    P2REN |= BIT6;
    P2OUT |= ~BIT6;
        /* P2.7 is connect HP203B INT1 interrupt, */
    P2DIR &= ~BIT7;                   // set P2.0 pin input with pullup resistor
    P2REN |= BIT7;
    P2OUT |= BIT7;

    /* P3 configuration */
        /* P3 all pin is output */
    P3DIR = 0xFF;
        /* P3.0 is Angle sensor power enable pin,low off,height on */
        /* P3.1 is Angle sensor power enable pin,low off,height on */
        /* P3.2 is Angle sensor power enable pin,low off,height on */
        /* P3.7 is Angle sensor power enable pin,low off,height on */
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT7);                   // off sensor power
        /* P3.3,4,5,6 is unused,because have External pull-up resistor, output height */
    P3OUT |= (BIT3 | BIT4 | BIT5 | BIT6);                   // output height

    /* P4 configuration */
        // Initialize XT1 32kHz crystal
    P4SEL0 |= BIT1 | BIT2;                  // set XT1 pin as second function
/*    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
*/
    P4DIR |= (BIT0 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
        /* P4.3 is Three-color light R pin,low on,height off */
        /* P4.4 is Three-color light B pin,low on,height off */
        /* P4.5 is Three-color light G pin,low on,height off */
    P4OUT |= (BIT3 | BIT4 | BIT5);
        /* P4.0,6,7 is unused,because have External pull-up resistor, output height */
    P4OUT |= (BIT0 | BIT6 | BIT7);

    /* P5 configuration */
        /* P5.2 is UCB0SDA,P5.3 is UCB0SCL, connect HP203B */
    P5SEL0 |= BIT2 | BIT3;                  // set P1.0,P1.1 pin as second function(UCA0UART)
    P5DIR |= (BIT0 | BIT1 | BIT4 | BIT5 | BIT6 | BIT7);
        /* P5.0,1,4,5,6,7 is unused,because have External pull-up resistor, output height */
    P5OUT |= (BIT0 | BIT1 | BIT4 | BIT5 | BIT6 | BIT7);

    /* P6 configuration */
        /* P6 is is unused,because have External pull-up resistor, output height */
    P6DIR = 0xFF;
    P6OUT = 0xFF;

    /* P7 configuration */
        /* P7 other pin is is unused,because have External pull-up resistor, output height */
    P7DIR = 0xFF;
    P7OUT = 0xFF;
        /* P7.7 is is CC1010 NRST,low reset,need hold specific times */

    /* P8 configuration */
        /* P8.1 is Detect battery power as ADC */
    P8DIR &= ~BIT1;                   // set P8.1 pin input as A8
        /* P8.0,2,3 is is unused,because have External pull-up resistor, output height */
    P8DIR |= (BIT0 | BIT2 | BIT3);
    P8OUT |= (BIT0 | BIT2 | BIT3);

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
}



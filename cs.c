/*
 * cs.c
 *
 *  Created on: 2018Äê5ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

//******************************************************************************
//  MSP430FR413x Demo - Configure MCLK for 8MHz and XT1 sourcing ACLK and
//                     FLLREF.
//
//  Description: Configure ACLK = 32768Hz,
//               MCLK = DCO + XT1CLK REF = 8MHz,
//               SMCLK = MCLK = 8MHz.
//               Toggle LED to indicate that the program is running.
//
//           MSP430FR4133
//         ---------------
//     /|\|               |
//      | |      XIN(P4.1)|--
//      --|RST            |  ~32768Hz
//        |     XOUT(P4.2)|--
//        |               |
//        |          P1.0 |---> LED
//        |          P1.4 |---> MCLK = 8MHz
//        |          P8.0 |---> SMCLK = 8MHz
//        |          P8.1 |---> ACLK = 32768Hz
//
//
//   William Goh
//   Wei Zhao
//   Texas Instruments Inc.
//   Semptember 2015
//   Built with IAR Embedded Workbench v6.10 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

void cs_init(void)
{
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__XT1CLK;               // Set XT1CLK as FLL reference source
    CSCTL0 = 0;                             // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                 // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                    // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz

    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;  // Set ACLK = XT1CLK = 32768Hz
                                            // DCOCLK = MCLK and SMCLK source
    CSCTL5 |= DIVM_0 | DIVS_0;              // MCLK = DCOCLK = 8MHZ,
                                            // SMCLK = MCLK = 8MHz
}




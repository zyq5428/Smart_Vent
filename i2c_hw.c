/*
 * i2c_hw.c
 *
 *  Created on: 2017Äê8ÔÂ7ÈÕ
 *      Author: johnsonzhou
 */

#include "i2c_hw.h"

unsigned char *tx_data;
unsigned char *rx_data;
int tx_byte;
int TXByteCtr;
int rx_byte;
int RXByteCtr;

void i2c_Write(unsigned char addr, unsigned char *pstr, int num)
{
    tx_data = pstr;
    tx_byte = num;
    __delay_cycles(1000);                   // Delay between transmissions
    UCB0I2CSA = addr;             // configure slave address
    TXByteCtr = 0;                          // Load TX byte counter
    while (UCB0CTLW0 & UCTXSTP);            // Ensure stop condition got sent

    UCB0CTLW0 |= UCTR | UCTXSTT;            // I2C TX, start condition

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts

}

void i2c_read(unsigned char addr, unsigned char *pstr, int num)
{
    rx_data = pstr;
    rx_byte = num;
    __delay_cycles(1000);                   // Delay between transmissions
    UCB0I2CSA = addr;             // configure slave address
    RXByteCtr = 0;                          // Load TX byte counter
    while (UCB0CTLW0 & UCTXSTP);            // Ensure stop condition got sent

    UCB0CTLW0 &= ~UCTR;                     // I2C RX
    UCB0CTLW0 |= UCTXSTT;                   // start condition

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts

}

void i2c_init(void)
{
	  // Configure USCI_B0 for I2C mode
	  UCB0CTLW0 = UCSWRST;                      // put eUSCI_B in reset state
	  UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK; // I2C master mode, SMCLK
	  UCB0BRW = 0x50;                            // baudrate = SMCLK / 80
	  UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
	  UCB0IE |= UCTXIE0 | UCRXIE0 | UCNACKIE;             // transmit and NACK interrupt enable
}

void i2c_reset(void)
{
      // Configure USCI_B0 for I2C mode
      UCB0CTLW0 = UCSWRST;                      // put eUSCI_B in reset state
      __delay_cycles(10);
      UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         // Vector 2: ALIFG
    case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
      UCB0CTLW0 |= UCTXSTT;                 // resend start if NACK
      break;
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:  		        // Vector 22: RXIFG0
        if ((RXByteCtr >= 0) && (RXByteCtr < rx_byte))                        // Check TX byte counter
        {
              *rx_data++ = UCB0RXBUF;      // Read RX buffer
              RXByteCtr++;                        // Decrement RX byte counter
        }
        else
        {
          UCB0CTLW0 |= UCTXSTP;               // I2C stop condition
          UCB0IFG &= ~UCRXIFG;                // Clear USCI_B0 TX int flag
          __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
        }
    	break;
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
      if ((TXByteCtr >= 0) & (TXByteCtr < tx_byte))                        // Check TX byte counter
      {
        UCB0TXBUF = *tx_data++;      // Load TX buffer
        TXByteCtr++;                        // Decrement TX byte counter
      }
      else
      {
        UCB0CTLW0 |= UCTXSTP;               // I2C stop condition
        UCB0IFG &= ~UCTXIFG;                // Clear USCI_B0 TX int flag
        __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
      }
      break;
    default: break;
  }
}

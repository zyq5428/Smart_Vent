#include <msp430.h>
#include "main.h"
#include "gpio.h"
#include "rtc.h"
#include "led.h"
#include "cs.h"
#include "uart.h"
#include "timer_delay.h"
#include "bat_detect.h"
#include "angle.h"
#include "p2_int.h"
#include "motor.h"
#include "power_on.h"

unsigned int Global_Flag;
unsigned int Current_Status;
unsigned int Backup_Status;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt

    //uart_init();

    limit_int_en();

    bat_info_init();

    angle_info_init();

    Global_Flag = 0x8000;

    while (1) {
        unsigned char right_shift_counter;

        switch (Global_Flag) {
        case  SELF_TEST_Flag:        //BIT15
            vent_self_test();
            Global_Flag &= ~(SELF_TEST_Flag);    //Clear flag when finished
            break;
        case  0:
            __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
            break;
        case  OPEN_INT_Flag:        //BIT0
            open_int_isr();
            Global_Flag &= ~(OPEN_INT_Flag);    //Clear flag when finished
            break;
        case  CLOSE_INT_Flag:        //BIT1
            close_int_isr();
            Global_Flag &= ~(CLOSE_INT_Flag);    //Clear flag when finished
            break;
        case  RTC_INT_Flag:        //BIT2
            rtc_int_isr();
            Global_Flag &= ~(RTC_INT_Flag);    //Clear flag when finished
            break;
        case  CC1310_INT_Flag:        //BIT3
            cc1310_int_isr();
            Global_Flag &= ~(CC1310_INT_Flag);    //Clear flag when finished
            break;
        case  UART_RX_Flag:        //BIT4
            //uart_rx_isr();
            Global_Flag &= ~(UART_RX_Flag);    //Clear flag when finished
            break;
        case  UART_TX_Flag:        //BIT5
            //uart_tx_isr();
            Global_Flag &= ~(UART_TX_Flag);    //Clear flag when finished
            break;
        case  I2C_RX_Flag:        //BIT6
            //i2c_rx_isr();
            Global_Flag &= ~(I2C_RX_Flag);    //Clear flag when finished
            break;
        case  I2C_TX_Flag:        //BIT7
            //i2c_tx_isr();
            Global_Flag &= ~(I2C_TX_Flag);    //Clear flag when finished
            break;
        default:
            for(right_shift_counter = 0; right_shift_counter <= 15; right_shift_counter++) {
                if((Global_Flag >> right_shift_counter) & BIT0) {
                    Backup_Status = Global_Flag;
                    Current_Status = (BIT0 << right_shift_counter);
                    Global_Flag = Current_Status;
                    break;
                }
            }
            break;
        }
    }
}

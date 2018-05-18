#include <msp430.h>
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

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    rtc_init(100);

    uart_init();

    p2_int_init();

    while (0) {
        angle_measure();
    }

    while (1) {
        motor_init(motor_Forward, 10000, 80);
        pwm_init();
        motor_open();


        __bis_SR_register(LPM0_bits);             // Enter LPM0
        __no_operation();                         // For debugger
    }

    //__bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}

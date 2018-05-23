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
#include "power_on.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    //uart_init();

    p2_int_init();

    bat_info_init();

    vent_self_test();

    rtc_wake_isr();

    rtc_init(60);

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}

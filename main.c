#include <msp430.h>
#include "gpio.h"
#include "rtc.h"
#include "led.h"
#include "cs.h"
#include "uart.h"
#include "timer_delay.h"
#include "bat_detect.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    rtc_init(100);

    uart_init();

    while (1) {
        bat_info_init();
        bat_detect();
        delay_hw_s(3);
    }

    //__bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}

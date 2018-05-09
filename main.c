#include <msp430.h>
#include "gpio.h"
#include "rtc.h"
#include "led.h"
#include "cs.h"
#include "uart.h"
#include "timer_delay.h"
#include "i2c_hw.h"
#include "hp203b.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    rtc_init(60);

    uart_init();


    while (1) {
        i2c_init();
        hp203b_init();
        P4OUT ^= BIT3;
    }

    //__bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}

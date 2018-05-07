#include <msp430.h>
#include "gpio.h"
#include "rtc.h"
#include "led.h"
#include "cs.h"
#include "uart.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    cs_init();

    rtc_init(60);

    uart_init();

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}

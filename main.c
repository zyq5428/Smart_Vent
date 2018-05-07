#include <msp430.h>
#include "gpio.h"
#include "rtc.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();

    rtc_init(60);

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3, enable interrupt
}



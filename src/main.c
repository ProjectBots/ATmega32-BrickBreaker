#include "adc.h"
#include "display.h"
#include "time.h"
#include "usart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    usartSetup(USART_B9600, USART_CONFIG_8N1);
    adcSetup();
	displaySetup();

    sei();

    usartWriteString("Hello to Lab 4 :-) \n\r");

    volatile int16_t x = 0, y = 0;

    while (1) {
		displayClearBuffer();

        adcSetChannel(1);
        x = adcRead() / 8;
        adcSetChannel(0);
        y = adcRead() / 16;
		
		displayDrawRectangle(x, y, 1, 5);
		displayUpdate();
    }
    return 0;
}

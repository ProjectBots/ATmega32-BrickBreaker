

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "game.h"
#include "time.h"
#include "utils/adc.h"
#include "utils/disp/display.h"
#include "utils/timer.h"
#include "utils/usart.h"

volatile uint8_t timer0OverflowCount = 0;

ISR(TIMER0_COMP_vect) {
	gameUpdate();
	gameDraw();
}

int main(void) {
	usartSetup(USART_B9600, USART_CONFIG_8N1);
	adcSetup();
	displaySetup();

	uint8_t ocr = (F_CPU / 1024 / 60) - 1; // (F_CPU / prescaler / frequency) - 1
	initTimer0(ocr, TIMER_PRESCALER_1024);	 // Set up Timer0 with a prescaler of 64

	sei();

	while(1) {
		
	}

	/*
	volatile int16_t x = 0, y = 0;

	while (1) {
		uprintfln("x: %d, y: %d", x, y);

		// Draw a rectangle at the new position

		displayClearBuffer();

		adcSetChannel(1);
		x = adcRead() / 8;
		adcSetChannel(0);
		y = adcRead() / 16;

		displayDrawRectangle(x, y, 1, 5);
		displayUpdate();
	}
		*/
	return 0;
}

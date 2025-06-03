

#include "utils/timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"



void initTimer0(uint8_t compareValue, enum TIMER_PRESCALER prescaler) {
	// Set CTC mode (Clear Timer on Compare Match)
	BIT_SET(TCCR0, WGM00);    // Set WGM00 bit for CTC mode
	BIT_CLR(TCCR0, WGM01);    // Clear WGM01 bit for CTC mode

	// Set Compare Match value
	OCR0 = compareValue;

	// Enable Timer0 Compare Match Interrupt
	BIT_SET(TIMSK, OCIE0);  

	// Start Timer0 with specified prescaler
	TCCR0 |= prescaler;

	BIT_CLR(TIFR, OCF0);  // Clear any pending interrupt flag
}
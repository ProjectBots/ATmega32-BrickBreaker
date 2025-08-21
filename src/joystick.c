
#include "joystick.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "bit.h"

void joystickInit() {
	// Set the ADC reference voltage to AREF, Internal Vref turned off
	BIT_CLR(ADMUX, REFS0);
	BIT_CLR(ADMUX, REFS1);

	// Set channel to 0
	ADMUX &= ~0b11111;

	// Enable the ADC, set prescaler to 128 for 125 kHz ADC clock
	ADCSRA = BIT(ADEN) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);
}

void requestJoystickUpdate() {
	// Start ADC conversion
	BIT_SET(ADCSRA, ADSC);
}

uint16_t joystickRead() {
	// Wait for ADC conversion to complete
	while (BIT_IS_SET(ADCSRA, ADSC));

	// Joystick is physically rotated, we need to invert the ADC value
	return 1024 - ADC;
}

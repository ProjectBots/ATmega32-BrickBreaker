
#include "joystick.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "bit.h"

void joystickInit() {
	// Set the ADC reference voltage to AREF, Internal Vref turned off
	BIT_CLR(ADMUX, REFS0);
	BIT_CLR(ADMUX, REFS1);

	// Set channel to 0 (X-axis)
	ADMUX &= ~0b11111;

	// Enable the ADC
	ADCSRA = BIT(ADEN);
}

void requestJoystickUpdate() {
	// Start ADC conversion
	BIT_SET(ADCSRA, ADSC);
}

uint16_t joystickRead() {
	// Wait for ADC conversion to complete
	while (BIT_IS_SET(ADCSRA, ADSC));

	// Joystick is rotated, so we invert the value
	return 1024 - ADC;
}

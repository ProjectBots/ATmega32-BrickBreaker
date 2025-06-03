
#include <avr/io.h>

#include "bit.h"
#include "utils/adc.h"

void adcSetup() {
	//ADMUX = BIT(REFS0);
	//ADCSRA = BIT(ADEN) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);

	// Set ADC reference voltage to AREF, Internal Vref turned off
	BIT_CLR(ADMUX, REFS0);
	BIT_CLR(ADMUX, REFS1);

	
}


void adcSetChannel(__attribute__((unused)) uint8_t channel) {
	/* Clear 5 lower bits for channel selection */
	ADMUX &= 0xE0;
	/* Set single ended channel */
	ADMUX |= channel & 0b111;
}

void adcStartConversion() {
	ADCSRA |= BIT(ADSC); // Start ADC conversion
}


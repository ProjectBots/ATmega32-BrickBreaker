
#include "joystick.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "utils/adc.h"
#include "utils/usart.h"


static volatile bool joystickUpdated = false;
static volatile bool updatedX = false;

static volatile JoystickState joystickState = {0, 0};

void joystickInit(void) {
	adcSetup();
	adcSetChannel(0); // Set initial channel for X-axis
	adcEnableInterrupts();
}

void joystickRead(struct JoystickState* state) {
	if (!joystickUpdated) {
		return; // No new data available
	}

	state->x = joystickState.x; // Read X-axis position
	state->y = joystickState.y; // Read Y-axis position
}

void requestJoystickUpdate() {
	joystickUpdated = false; // Reset the update flag
	joystickState.x = 0; // Reset X-axis position
	joystickState.y = 0; // Reset Y-axis position
	adcStartConversion(); // Start ADC conversion for joystick inputs
}

ISR(ADC_vect) {
	joystickUpdated = true; // Set the flag when ADC conversion is complete
	// Read X-axis
	adcSetChannel(0);
	state->x = adcRead();

	// Read Y-axis
	adcSetChannel(1);
	state->y = adcRead();

	joystickUpdated = false; // Reset the update flag
}
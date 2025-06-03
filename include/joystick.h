
#pragma once

#include <stdint.h>

typedef struct JoystickState {
	int16_t x;  // X-axis position
	int16_t y;  // Y-axis position
} JoystickState;


/**
 * Initialize the joystick system.
 * This function should be called once at the start of the program.
 */
void joystickInit();

/**
 * Update the joystick state.
 * This function waits for the ADC conversion to complete and updates the joystick state.
 * @note This functions assumes you have already called `requestJoystickUpdate()` to start the ADC conversion.
 * @param[out] state Pointer to a JoystickState structure that will be updated with the current joystick state.
 */
void joystickRead(struct JoystickState* state);

/**
 * Request an update of the joystick state.
 * This function will trigger the adc conversion for the joystick inputs.
 */
void requestJoystickUpdate();





#pragma once

#include <stdint.h>

/**
 * Initialize the joystick system.
 * This function should be called once at the start of the program.
 */
void joystickInit();

/**
 * This function will triggers the adc conversion to get the joystick input.
 */
void requestJoystickUpdate();

/**
 * Read the current joystick state.
 * This function waits for the ADC conversion to complete if not already finished.
 * @note This functions assumes you have already called `requestJoystickUpdate()` to start the ADC conversion beforehand.
 * @return The joystick value as a 16-bit unsigned integer. (Actually a 10-bit value, but returned as 16-bit for compatibility.)
 */
uint16_t joystickRead();

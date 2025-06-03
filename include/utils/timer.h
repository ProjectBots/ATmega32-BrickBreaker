
#pragma once

#include <avr/io.h>

enum TIMER_PRESCALER {
	TIMER_PRESCALER_1 = (1 << CS00),				  // No prescaling
	TIMER_PRESCALER_8 = (1 << CS01),				  // Prescaler 8
	TIMER_PRESCALER_64 = (1 << CS01) | (1 << CS00),	  // Prescaler 64
	TIMER_PRESCALER_256 = (1 << CS02),				  // Prescaler 256
	TIMER_PRESCALER_1024 = (1 << CS02) | (1 << CS00)  // Prescaler 1024
};

void initTimer0(uint8_t compareValue, enum TIMER_PRESCALER prescaler);
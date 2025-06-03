
#pragma once

#include <stdint.h>

/** Setup the ADC.
 *  @note Must be invoked before anything else.
 */
void adcSetup();

/** 
 * Set the ADC channel.
 *  @param[in] channel The ADC channel to select (0-7).
 */
void adcSetChannel(uint8_t channel);

/** 
 * signal to start an ADC conversion.
 */
void adcStartConversion();



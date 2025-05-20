/**
 * @author MIC Lab Team
 * @brief avrhal adc functions
 *
 */
#pragma once

#include <stdint.h>

/** Setup the ADC.
 *  @note Must be invoked once before adcRead().
 */
void adcSetup();

/** Carry out a single AD Conversion.
 *
 * This call busy-waits until the conversion is completed.
 *
 * @return The 10-bit conversion result
 */
uint16_t adcRead();

/** Set the channel for a single AD Conversion.
 *
 * @param[in] the AD channel
 */
void adcSetChannel(uint8_t channel);

/** Configures the AD Converter as free running.
 *
 */
void adcSetupFreeRunning();

/** Obtain last AD result
 *
 * When configured via analogReadFreeRunning() this
 * function may be used to obtain the last reading.
 *
 * The value "-1" is returned when no result is available
 * or the pin was not configured via analogReadFreeRunning().
 *
 * @param[in] pin avrhal pin number
 * @return last conversion result or -1
 */
int16_t adcLastRead(uint8_t channel);

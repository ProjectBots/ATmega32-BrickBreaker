#include "utils/spi.h"

#include <avr/io.h>

#include "bit.h"

void spiSetup() {
	/* Set the SPI pins as output */
	BIT_SET(DDRB, SPI_MOSI);
	BIT_SET(DDRB, SPI_SCK);
	BIT_SET(SPSR, SPI2X);
	SPCR |= BIT(SPE) | BIT(MSTR);
}

uint8_t spiTransferByte(uint8_t data) {
	/* Write data byte to register, initiating the transmission */
	/* Wait until the flag-bit is set, indicating a completed data byte transfer */
	/* Return the byte received from the slave IC */
	SPDR = data;
	while (!(BIT_IS_SET(SPSR, SPIF)));
	return SPDR;
}

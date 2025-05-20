/**
 * @author MIC Lab Team - Olaf Sassnick
 * @brief OLED Display Driver with framebuffer
 *
 */

#ifndef _AVRHAL_DISPLAY__H__
#define _AVRHAL_DISPLAY__H__

#include <stdint.h>

#include "bitmap.h"

#define DISPLAY_DEFAULT_CONTRAST 128
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_PAGES 8
#define DISPLAY_BITS_PER_PAGE_COLUMN 8

void displayReset();

/** Carry out a display hardware initialization, including a hardware reset. */
void displaySetup();
void displayClearBuffer();

uint64_t* displayFrameBuffer();
void displayUpdate();

void displayDrawVerticalLine(uint8_t x, uint8_t y, uint8_t length);
void displayDrawHorizontalLine(uint8_t x, uint8_t y, uint8_t length);
void displayDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void displayDrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void displayDrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void displayDrawPixel(uint8_t x, uint8_t y);
void displayDrawBitmap(uint8_t x, uint8_t y, const Bitmap* bmp);
void displayRenderText(uint8_t x, uint8_t y, const char* str);
void displayRenderTextVertical(uint8_t x, uint8_t y, const char* str);
void displayPrint(uint8_t x, uint8_t y, const char* format, ...);
void displayPrintVertical(uint8_t x, uint8_t y, const char* format, ...);

#endif

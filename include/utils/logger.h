
#pragma once

#include "usart.h"

#define uprint(str) usartWriteString(str)
#define uprintln(str) usartWriteString(str "\n\r")
#define uprintf(fmt, ...) usartPrint(fmt, ##__VA_ARGS__)
#define uprintfln(fmt, ...) usartPrint(fmt "\n\r", ##__VA_ARGS__)
#ifndef __UTIL_H
#define __UTIL_H

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

#include "stm32f1xx_hal.h"

void UART1_Init(uint32_t baudrate);
void UART2_Init(uint32_t baudrate);
void UART3_Init(uint32_t baudrate);

void u1_printf(char *fmt, ...);
void u2_printf(char *fmt, ...);
void u3_printf(char *fmt, ...);

#endif

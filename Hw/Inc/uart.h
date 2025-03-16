#ifndef __UART_H
#define __UART_H

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

void U1_Init(uint32_t baudrate);
void u1_printf(char *fmt, ...);

#endif

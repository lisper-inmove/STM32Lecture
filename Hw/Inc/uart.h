#ifndef __UART_H
#define __UART_H

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

#include "stm32f1xx_hal.h"

void U2_Init(uint32_t baudrate);
extern UART_HandleTypeDef uart2;

extern uint8_t rxbuff[64];
extern uint8_t rxstate;

void u_printf(char *fmt, ...);

#endif

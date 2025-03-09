#ifndef __UART_H
#define __UART_H

#include "stdint.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

#define U1_RX_MAX 256

void U1_Init(uint32_t baudrate);
extern UART_HandleTypeDef uart1;

extern uint8_t txbuff[U1_RX_MAX], rxbuff[U1_RX_MAX];
extern uint8_t rxstate;

void U1_Init_IDLE(void);

void u1_printf(char *fmt, ...);

#endif

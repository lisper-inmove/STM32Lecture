#ifndef __UART_H
#define __UART_H

extern UART_HandleTypeDef uart1;

void U1_Init(uint32_t baudrate);
void u1_printf(char *fmt, ...);

#endif

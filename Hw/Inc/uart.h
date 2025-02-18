#ifndef __UART_H
#define __UART_H

void U1_Init(uint32_t baudrate);
void U2_Init(uint32_t baudrate);
void U3_Init(uint32_t baudrate);
extern UART_HandleTypeDef uart1;
extern UART_HandleTypeDef uart2;
extern UART_HandleTypeDef uart3;

extern uint8_t rxbuffer[64], txbuffer[64];
extern uint8_t rxstate;

#endif

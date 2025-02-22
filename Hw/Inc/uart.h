#ifndef __UART_H
#define __UART_H

void U1_Init(uint32_t baudrate);
extern UART_HandleTypeDef uart1;

extern DMA_HandleTypeDef dmatx, dmarx;

extern uint8_t rxbuff[64];
extern uint8_t rxstate;

void U1_Init_IDLE(void);
void U1_Init_DMA1(void);
void U1_Init_DMA_TX(void);
void U1_Init_DMA_RX(void);

#endif

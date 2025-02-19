#ifndef __UART_H
#define __UART_H

#define U1_RX_SIZE 2048
#define U1_TX_SIZE 2048
#define U1_RX_MAX 256

typedef struct {
	uint8_t *start;
	uint8_t *end;
}LCB;

typedef struct {
	uint8_t *start;
	uint8_t *end;
}UCB;

void U1_Init(uint32_t baudrate);
void U2_Init(uint32_t baudrate);
void U3_Init(uint32_t baudrate);
extern UART_HandleTypeDef uart1;
extern UART_HandleTypeDef uart2;
extern UART_HandleTypeDef uart3;

extern uint8_t  txbuff[64], rxbuff[64];
extern uint8_t  rxstate;

void U1_Init_IDLE(void);

#endif

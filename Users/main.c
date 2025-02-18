#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "uart.h"

#define RX_SIZE 200

uint8_t buff[256];

int main(void) {
	HAL_Init();
	RccClock_Init();
	U1_Init(921600);
	while (1) {
		if (rxstate == 1) {
			rxstate = 0;
			HAL_UART_Transmit_IT(&uart1, txbuffer, 20);
		}
	}
}

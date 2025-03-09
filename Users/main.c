#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "uart.h"

#define RX_SIZE 200
#define TIMEOUT 200

uint8_t buff[256];

int main(void) {
	HAL_Init();
	RccClock_Init();
	U1_Init(921600);
	u1_printf("Hello World! No response!!");
	while (1) {
		switch(HAL_UART_Receive(&uart1, buff, RX_SIZE, TIMEOUT)) {
			case HAL_OK:
				HAL_UART_Transmit(&uart1, buff, RX_SIZE, TIMEOUT);
				break;
			case HAL_ERROR:
				break;
			case HAL_BUSY:
				break;
			case HAL_TIMEOUT:
				if (uart1.RxXferCount != RX_SIZE - 1) {
					HAL_UART_Transmit(&uart1, buff, RX_SIZE - 1 - uart1.RxXferCount, TIMEOUT);
				}
				break;
		}
	}
}

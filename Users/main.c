#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "uart.h"
#include "util.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	U1_Init(921600);
	u1_printf("Program start: %d\n", 10);
	while (1) {
		if(rxstate == 1){
			rxstate = 0;
			u1_printf("Get new message: %s\n", rxbuff);
			HAL_UART_Transmit_DMA(&uart1, rxbuff, 20);
		}
	}
}

#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "uart.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	U1_Init(921600);
	u1_printf("Program Start...");
	while (1) {
		if(rxstate == 1){
			rxstate = 0;
			HAL_UART_Transmit_IT(&uart1, txbuff, strlen((char *)txbuff));
		}		
	}
}

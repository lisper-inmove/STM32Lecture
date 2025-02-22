#include "util.h"
#include "uart.h"

void u1_printf(char *fmt, ...) {
	uint8_t tempbuff[256];
	uint16_t i;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)tempbuff, fmt, ap);
	va_end(ap);
	for (i = 0; i < strlen((char *)tempbuff); i++) {
		while(!__HAL_UART_GET_FLAG(&uart1, UART_FLAG_TXE));
		uart1.Instance->DR = tempbuff[i];
	}
	while(!__HAL_UART_GET_FLAG(&uart1, UART_FLAG_TC));
}

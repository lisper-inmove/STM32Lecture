#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart2;

uint8_t rxbuff[64];
uint8_t rxstate;

void U2_Init(uint32_t baudrate) {
	uart2.Instance = USART2;
	uart2.Init.BaudRate = baudrate;
	uart2.Init.WordLength = UART_WORDLENGTH_8B;
	uart2.Init.StopBits = UART_STOPBITS_1;
	uart2.Init.Parity = UART_PARITY_NONE;
	uart2.Init.Mode = UART_MODE_TX;
	uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart2);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	
	GPIO_InitTypeDef GPIO_InitType;
	
	if (huart->Instance == USART2) {
			__HAL_RCC_GPIOA_CLK_ENABLE();
		// USART1_CLK 时钟使能
		__HAL_RCC_USART2_CLK_ENABLE();

		GPIO_InitType.Pin = GPIO_PIN_2;
		GPIO_InitType.Mode = GPIO_MODE_AF_PP;
		GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
		GPIO_InitType.Pin = GPIO_PIN_3;
		GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitType.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
		// 设置 USART1_IRQn 的优先级
		HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
		// 使能 USART1_IRQn
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

void u_printf(char *fmt, ...) {
	uint8_t tempbuff[256];
	uint16_t i;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)tempbuff, fmt, ap);
	va_end(ap);
	for (i = 0; i < strlen((char *)tempbuff); i++) {
		while(!__HAL_UART_GET_FLAG(&uart2, UART_FLAG_TXE));
		uart2.Instance->DR = tempbuff[i];
	}
	while(!__HAL_UART_GET_FLAG(&uart2, UART_FLAG_TC));
}

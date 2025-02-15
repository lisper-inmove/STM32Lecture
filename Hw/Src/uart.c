#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

void U1_Init(uint32_t baudrate) {
	uart1.Instance = USART1;
	uart1.Init.BaudRate = baudrate;
	uart1.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.Init.StopBits = UART_STOPBITS_1;
	uart1.Init.Parity = UART_PARITY_NONE;
	uart1.Init.Mode = UART_MODE_TX_RX;
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart1);
}

void U2_Init(uint32_t baudrate) {
	uart2.Instance = USART2;
	uart2.Init.BaudRate = baudrate;
	uart2.Init.WordLength = UART_WORDLENGTH_8B;
	uart2.Init.StopBits = UART_STOPBITS_1;
	uart2.Init.Parity = UART_PARITY_NONE;
	uart2.Init.Mode = UART_MODE_TX_RX;
	uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart2);
}

void U3_Init(uint32_t baudrate) {
	uart3.Instance = USART3;
	uart3.Init.BaudRate = baudrate;
	uart3.Init.WordLength = UART_WORDLENGTH_8B;
	uart3.Init.StopBits = UART_STOPBITS_1;
	uart3.Init.Parity = UART_PARITY_NONE;
	uart3.Init.Mode = UART_MODE_TX_RX;
	uart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&uart3);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	
	GPIO_InitTypeDef GPIO_InitType;
	
	if (huart->Instance == USART1) {
		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		// 将RX, TX重映射到B6和B7
		__HAL_AFIO_REMAP_USART1_ENABLE();
		
		// 连线时 B6 ~ RX, B7 ~ TX
		GPIO_InitType.Pin = GPIO_PIN_6;
		GPIO_InitType.Mode = GPIO_MODE_AF_PP;
		GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOB, &GPIO_InitType);
		
		GPIO_InitType.Pin = GPIO_PIN_7;
		GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitType.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitType);
	} else if (huart->Instance == USART2) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();
		
		GPIO_InitType.Pin = GPIO_PIN_2;
		GPIO_InitType.Mode = GPIO_MODE_AF_PP;
		GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);
		
		GPIO_InitType.Pin = GPIO_PIN_3;
		GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitType.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);
	}else if (huart->Instance == USART3) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();
		
		GPIO_InitType.Pin = GPIO_PIN_10;
		GPIO_InitType.Mode = GPIO_MODE_AF_PP;
		GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOB,&GPIO_InitType);
		
		GPIO_InitType.Pin = GPIO_PIN_11;
		GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitType.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB,&GPIO_InitType);
	}
}

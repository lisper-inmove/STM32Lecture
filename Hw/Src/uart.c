#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;

uint8_t  txbuff[U1_RX_MAX], rxbuff[U1_RX_MAX];
uint8_t  rxstate;

void U1_Init(uint32_t baudrate) {
	uart1.Instance = USART1;
	uart1.Init.BaudRate = baudrate;
	uart1.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.Init.StopBits = UART_STOPBITS_1;
	uart1.Init.Parity = UART_PARITY_NONE;
	uart1.Init.Mode = UART_MODE_TX_RX;
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	// 先对uart1作一些初始化操作，然后调用 HAL_UART_MspInit函数
	// HAL 库官方 实现 HAL_UART_MspInit的weak形式，我们自己实现该函数实际形式
	// 在 HALT_UART_MspInit 函数中，我们可以实现该中断的关联的引脚，中断使能等
	HAL_UART_Init(&uart1);
	U1_Init_IDLE();
}

void U1_Init_IDLE(void) {
	// 使能 UART_IT_IDLE中断
	__HAL_UART_ENABLE_IT(&uart1, UART_IT_IDLE);
	// 使能 UART_IT_PE UART_IT_ERR UART_IT_RXNE
	// 相当于是 HAL库帮我们 调用了 
	// __HAL_UART_ENABLE_IT(&uart1, UART_IT_PE)
	// __HAL_UART_ENABLE_IT(&uart1, UART_IT_ERR)
	// __HAL_UART_ENABLE_IT(&uart1, UART_IT_RXNE)
	HAL_UART_Receive_IT(&uart1, rxbuff, U1_RX_MAX);
	
	// UART_IT_PE, UART_IT_ERR, UART_IT_IDLE 都属于 UART(串口) 的分中断
	// 属于 USART1_IRQn, USART2_IRQn, USART3_IRQn 这的分中断
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	
	GPIO_InitTypeDef GPIO_InitType;
	
	if (huart->Instance == USART1) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		// USART1_CLK 时钟使能
		__HAL_RCC_USART1_CLK_ENABLE();

		GPIO_InitType.Pin = GPIO_PIN_9;
		GPIO_InitType.Mode = GPIO_MODE_AF_PP;
		GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
		GPIO_InitType.Pin = GPIO_PIN_10;
		GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitType.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
		// 设置 USART1_IRQn 的优先级
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
		// 使能 USART1_IRQn
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART1){
		memcpy(txbuff, rxbuff, U1_RX_MAX);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1, rxbuff, U1_RX_MAX);
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		memcpy(txbuff, rxbuff, U1_RX_MAX);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1, rxbuff, U1_RX_MAX);
	}
}

void u1_printf(char *fmt, ...) {
	uint8_t tempbuff[1024];
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

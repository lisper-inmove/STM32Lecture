#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

uint8_t  txbuff[64], rxbuff[64];
uint8_t  rxstate;

uint8_t U1_RxBuff[U1_RX_SIZE];
uint8_t U1_TxBuff[U1_TX_SIZE];

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
	HAL_UART_Receive_IT(&uart1, U1_RxBuff, U1_RX_MAX);
	
	// UART_IT_PE, UART_IT_ERR, UART_IT_IDLE 都属于 UART(串口) 的分中断
	// 属于 USART1_IRQn, USART2_IRQn, USART3_IRQn 这的分中断
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART1){
		memcpy(txbuff,rxbuff,20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1,rxbuff,20);
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		memcpy(txbuff, rxbuff, 20);
		rxstate = 1;
		HAL_UART_Receive_IT(&uart1, U1_TxBuff, U1_RX_MAX);
	}
}

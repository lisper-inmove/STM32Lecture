#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;

DMA_HandleTypeDef dmatx, dmarx;

uint8_t rxbuff[64];
uint8_t rxstate;


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
	U1_Init_DMA1();
	U1_Init_IDLE();
}

void U1_Init_IDLE(void) {
	// 使能 UART_IT_IDLE中断
	__HAL_UART_ENABLE_IT(&uart1, UART_IT_IDLE);
}

void U1_Init_DMA1(void) {
	// DMA1时钟使能
	__HAL_RCC_DMA1_CLK_ENABLE();
	U1_Init_DMA_TX();
	U1_Init_DMA_RX();
	// rxbuff，最终会挂在, usart的 pRxBufferPtr上
	// 20则挂在 RxXferSize 上
	HAL_UART_Receive_DMA(&uart1, rxbuff, 20);
}

void U1_Init_DMA_TX(void) {
	dmatx.Instance = DMA1_Channel4;
	dmatx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	dmatx.Init.PeriphInc = DMA_PINC_DISABLE;
	dmatx.Init.MemInc = DMA_MINC_ENABLE;
	dmatx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dmatx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dmatx.Init.Mode = DMA_NORMAL;
	dmatx.Init.Priority = DMA_PRIORITY_MEDIUM;
	__HAL_LINKDMA(&uart1, hdmatx, dmatx);
	HAL_DMA_Init(&dmatx);
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn,3,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void U1_Init_DMA_RX(void) {
	dmarx.Instance = DMA1_Channel5;
	dmarx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dmarx.Init.PeriphInc = DMA_PINC_DISABLE;
	dmarx.Init.MemInc = DMA_MINC_ENABLE;
	dmarx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	dmarx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	dmarx.Init.Mode = DMA_NORMAL;
	dmarx.Init.Priority = DMA_PRIORITY_MEDIUM;
	__HAL_LINKDMA(&uart1, hdmarx, dmarx);
	HAL_DMA_Init(&dmarx);
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,3,0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART1){
		HAL_UART_Receive_DMA(&uart1, rxbuff, 20);
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		rxstate = 1;
	}
}

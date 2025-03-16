#include "util.h"

UART_HandleTypeDef uart1;
UART_HandleTypeDef uart2;
UART_HandleTypeDef uart3;

void UART1_Init(uint32_t baudrate) {
  uart1.Instance = USART1;
  uart1.Init.BaudRate = baudrate;
  uart1.Init.WordLength = UART_WORDLENGTH_8B;
  uart1.Init.StopBits = UART_STOPBITS_1;
  uart1.Init.Parity = UART_PARITY_NONE;
  uart1.Init.Mode = UART_MODE_TX_RX;
  uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_UART_Init(&uart1);
}

void UART2_Init(uint32_t baudrate) {
  uart2.Instance = USART2;
  uart2.Init.BaudRate = baudrate;
  uart2.Init.WordLength = UART_WORDLENGTH_8B;
  uart2.Init.StopBits = UART_STOPBITS_1;
  uart2.Init.Parity = UART_PARITY_NONE;
  uart2.Init.Mode = UART_MODE_TX_RX;
  uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_UART_Init(&uart2);
}

void UART3_Init(uint32_t baudrate) {
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
    __HAL_RCC_USART1_CLK_ENABLE();
    GPIO_InitType.Pin = GPIO_PIN_9;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitType);
  } else if(huart->Instance == USART2) {
		 __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
    GPIO_InitType.Pin = GPIO_PIN_2;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitType);
	} else if (huart->Instance == USART3) {
		 __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();
    GPIO_InitType.Pin = GPIO_PIN_10;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitType);
	}
}

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

void u2_printf(char *fmt, ...) {
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


void u3_printf(char *fmt, ...) {
  uint8_t tempbuff[256];
  uint16_t i;
  va_list ap;
  va_start(ap, fmt);
  vsprintf((char *)tempbuff, fmt, ap);
  va_end(ap);
  for (i = 0; i < strlen((char *)tempbuff); i++) {
    while(!__HAL_UART_GET_FLAG(&uart3, UART_FLAG_TXE));
    uart3.Instance->DR = tempbuff[i];
  }
  while(!__HAL_UART_GET_FLAG(&uart3, UART_FLAG_TC));
}


#include "string.h"
#include "stm32f1xx_hal.h"
#include "uart.h"

UART_HandleTypeDef uart1;


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

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

  GPIO_InitTypeDef GPIO_InitType;

  if (huart->Instance == USART1) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    GPIO_InitType.Pin = GPIO_PIN_9;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitType);

    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
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

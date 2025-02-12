#include "stm32f1xx_hal.h"
#include "led.h"

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitType;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_0;
	GPIO_InitType.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitType.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitType);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

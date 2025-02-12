#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	// HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	LED_Init();
	while (1) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_Delay(500);
	}
}

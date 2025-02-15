#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_pwr.h"
#include "rcc.h"
#include "led.h"
#include "sw.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	// HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	LED_Init();
	SW_Init_EVT();
	while (1) {
		for(uint8_t i = 0; i < 10; i++) {
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			HAL_Delay(500);
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_SuspendTick();
		HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFE);
		HAL_ResumeTick();
	}
}

#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "sw.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	// HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	LED_Init();
	SW_Init();
	while (1) {
		switch(SW_Scan(1)) {
			case 8: HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
							break;
			case 11: HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
							break;
			}
	}
}

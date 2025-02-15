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
	SW_Init_IT(0);
	SW_Init_EvtOutput();
	while (1) {
		
	}
}

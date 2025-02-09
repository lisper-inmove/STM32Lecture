#include "stm32f1xx_hal.h"

int main(void) {
	HAL_Init();
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
}

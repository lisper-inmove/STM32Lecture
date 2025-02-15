#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "led.h"
#include "sw.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	// HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
	LED_Init();
	SW_Init_IT(1);
	HAL_Delay(5000);
	// 将GPIO的引脚锁定，直到下次复位，再也无法修改
	HAL_GPIO_LockPin(GPIOB, GPIO_PIN_0);
	// 将GPIO引脚的初始化
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
	while (1) {
		// HAL_Delay(1000);
		// __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_13);
	}
}

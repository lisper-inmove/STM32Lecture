#include "stm32f1xx_hal.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

TIM_HandleTypeDef timer1;
TIM_ClockConfigTypeDef timer1_clock;
uint32_t counter = 0;

void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep) {
  timer1.Instance = TIM1;
  timer1.Init.Prescaler = psc;
  timer1.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer1.Init.Period = arr;
  timer1.Init.RepetitionCounter = rep;
  timer1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	timer1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&timer1);
	__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
	
	timer1_clock.ClockSource = TIM_CLOCKSOURCE_TI1;
	// timer1_clock.ClockSource = TIM_CLOCKSOURCE_TI2;
	timer1_clock.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	timer1_clock.ClockFilter =  0x03;
	HAL_TIM_ConfigClockSource(&timer1, &timer1_clock);
	
  HAL_TIM_Base_Start(&timer1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitType;
  if (htim->Instance == TIM1) {
    __HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitType.Pin = GPIO_PIN_8;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLDOWN;
		
		// 与 TIM_CLOCKSOURCE_TI2 对应。表示 通道2使用PA9
		// GPIO_InitType.Pin = GPIO_PIN_9;
		// GPIO_InitType.Mode = GPIO_MODE_INPUT;
		// GPIO_InitType.Pull = GPIO_PULLUP;
		
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
  }
}

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
	timer1_clock.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	timer1_clock.ClockFilter =  0x03;
	HAL_TIM_ConfigClockSource(&timer1, &timer1_clock);
	
	__HAL_TIM_ENABLE_IT(&timer1, TIM_IT_TRIGGER);
	
	// 更新中断会在此函数中自动打开: __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
  HAL_TIM_Base_Start_IT(&timer1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitType;
  if (htim->Instance == TIM1) {
    __HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitType.Pin = GPIO_PIN_8;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLDOWN;
		
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
		
    HAL_NVIC_SetPriority(TIM1_TRG_COM_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_IRQn);
  }
}

// 定时器更新中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    u2_printf("Timer1 update interupt... %d\n", __HAL_TIM_GET_COUNTER(htim));
  }
}

// 定时器触发中断
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    u2_printf("Timer1 trigger interupt... %d\n", __HAL_TIM_GET_COUNTER(htim));
  }
}

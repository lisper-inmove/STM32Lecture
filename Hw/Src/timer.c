#include "stm32f1xx_hal.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

TIM_HandleTypeDef timer1;
TIM_ClockConfigTypeDef timer1_clock;
DMA_HandleTypeDef timer1_dmaup;
uint16_t timer1_dmabuff[2] = {2, 4};
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
	timer1_clock.ClockFilter = 0x03;
	HAL_TIM_ConfigClockSource(&timer1, &timer1_clock);
	
  HAL_TIM_Base_Start_DMA(&timer1, (uint32_t *)timer1_dmabuff, 2);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef GPIO_InitType;
  if (htim->Instance == TIM1) {
    __HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		GPIO_InitType.Pin = GPIO_PIN_8;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA, &GPIO_InitType);
		
		timer1_dmaup.Instance = DMA1_Channel5;
    timer1_dmaup.Init.Direction = DMA_MEMORY_TO_PERIPH;
    timer1_dmaup.Init.PeriphInc = DMA_PINC_DISABLE;
    timer1_dmaup.Init.MemInc = DMA_MINC_ENABLE;
    timer1_dmaup.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    timer1_dmaup.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    timer1_dmaup.Init.Mode = DMA_CIRCULAR;
    timer1_dmaup.Init.Priority = DMA_PRIORITY_MEDIUM;
    __HAL_LINKDMA(&timer1, hdma[TIM_DMA_ID_UPDATE], timer1_dmaup);
    HAL_DMA_Init(&timer1_dmaup);
		
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  }
}

// 定时器更新中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
		if (htim->hdma[TIM_DMA_ID_UPDATE]->State == HAL_DMA_STATE_READY) {
			u2_printf("Timer1 DMA interupt...");
			u2_printf("Value of arr, %d\n", htim->Instance->ARR);
		} else {
			u2_printf("Timer1 update interupt... %d\n", __HAL_TIM_GET_COUNTER(htim));
			u2_printf("Value of arr, %d\n", htim->Instance->ARR);
		}
  }
}

void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		u2_printf("TIM1 Half Cplt Callback");
	}
}

// 定时器触发中断
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    u2_printf("Timer1 trigger interupt... %d\n", __HAL_TIM_GET_COUNTER(htim));
  }
}

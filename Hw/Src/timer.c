#include "stm32f1xx_hal.h"
#include "timer.h"
#include "uart.h"

TIM_HandleTypeDef timer1;
DMA_HandleTypeDef timer1_dmaup;
uint16_t timer1_dmabuff[4] = {3999, 5999, 7999, 9999};
uint32_t counter = 0;

void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep) {
  timer1.Instance = TIM1;
  timer1.Init.Prescaler = psc;
  timer1.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer1.Init.Period = arr;
  timer1.Init.RepetitionCounter = rep;
  timer1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&timer1);
  // __HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
	// __HAL_TIM_ENABLE_IT(&timer1, TIM_IT_UPDATE);
  HAL_TIM_Base_Start_DMA(&timer1, (uint32_t *)timer1_dmabuff, 4);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		// HAL_NVIC_SetPriority(TIM1_UP_IRQn,3,0);
		// HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
		
		timer1_dmaup.Instance = DMA1_Channel5;
		timer1_dmaup.Init.Direction = DMA_MEMORY_TO_PERIPH;
		timer1_dmaup.Init.PeriphInc = DMA_PINC_DISABLE;
		timer1_dmaup.Init.MemInc = DMA_MINC_ENABLE;
		timer1_dmaup.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		timer1_dmaup.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		timer1_dmaup.Init.Mode = DMA_NORMAL;
		timer1_dmaup.Init.Priority = DMA_PRIORITY_MEDIUM;
		__HAL_LINKDMA(&timer1, hdma[TIM_DMA_ID_UPDATE], timer1_dmaup);
		HAL_DMA_Init(&timer1_dmaup);
		
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);		
  }
}

void HAL_TIM_BaseMspDeInit(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM1) {
    __HAL_RCC_TIM1_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
  }
}

void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	// 半完成中断。timer1_dmabuff 有4个值，第二个完成时，此回调函数被触发
	if (htim->Instance == TIM1) {
		u1_printf("Hal cplt... %d\n", counter++);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	// DMA完成的回调
	// 中断完成的回调是同一个函数
  if (htim->Instance == TIM1) {
		u1_printf("Value of arr, %d\n", htim->Instance->ARR);
		if (htim->hdma[TIM_DMA_ID_UPDATE]->State == HAL_DMA_STATE_READY) {
			u1_printf("Timer1 alarmed by dma... %d\n", counter++);
			HAL_DMA_DeInit(htim->hdma[TIM_DMA_ID_UPDATE]);
		} else {
			u1_printf("Timer1 alarmed by it... %d\n", counter++);
		}
  }
}

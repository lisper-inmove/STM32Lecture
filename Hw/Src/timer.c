#include "stm32f1xx_hal.h"
#include "timer.h"

TIM_HandleTypeDef timer1;

void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep) {
	/*
		当我传值为 3999, 35999, 2，主频率为 72MHZ时
	  定时器频率: Prescaler = 72000000 / 36000 = 2000HZ
	  计时次数: Period = 4000，计数模式为Up时，Period减为0时，计时结束
		实际计时为 (4000 / 2000) * (2 + 1) = 6秒
		*/
  timer1.Instance = TIM1;
	// 预分频值，决定定时器时钟的分频系数
	// 定时器的频率 = 主频率 / (psc + 1)
	timer1.Init.Prescaler = psc;
	// 计数模式 向上/向下/中心对齐
	timer1.Init.CounterMode = TIM_COUNTERMODE_UP;
	// 装载值 ，决定定时次数，实际次数为 arr + 1
	timer1.Init.Period = arr;
	// 高级定时器才有，控制重复周期
	timer1.Init.RepetitionCounter = rep;
	// TIMx_ARR寄存器有没有缓冲
	// 决定了本次修改是否立即生效
	// 如当前定时器是0~100，已经计时到80了，此时重新设置定时器为0~200
	// 当设置为 TIM_AUTORELOAD_PRELOAD_DISABLE时，立即更新，不使用缓冲
	// ENABLE时则会，先计数到100，然后再更新(使用缓冲)
	timer1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	// HAL_TIM_BASE_Init 中调用 TIM_Base_SetConfig 执行 TIMx->EGR = TIM_EGR_UG;
	// 在 TIM_AUTORELOAD_PRELOAD_DISABLE 模式下，HAL_TIM_Base_Init函数结束就已经开始计时了，就算Start函数还没有被调用
	// 可以在 Init函数之后 __HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE); 来清除这个标志位
	HAL_TIM_Base_Init(&timer1);
	__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start(&timer1);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_ENABLE();
	}
}

void HAL_TIM_BaseMspDeInit(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		__HAL_RCC_TIM1_CLK_DISABLE();
	}
}

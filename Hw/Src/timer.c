#include "stm32f1xx_hal.h"
#include "timer.h"
#include "uart.h"
#include "util.h"

TIM_HandleTypeDef timer1;
TIM_IC_InitTypeDef timer1_ic1;
TIM_IC_InitTypeDef timer1_ic2;
TIM_IC_InitTypeDef timer1_ic3;
TIM_IC_InitTypeDef timer1_ic4;

uint32_t counter = 0;


void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep) {
  timer1.Instance = TIM1;
  timer1.Init.Prescaler = psc;
  timer1.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer1.Init.Period = arr;
  timer1.Init.RepetitionCounter = rep;
  timer1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	timer1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	HAL_TIM_IC_Init(&timer1);
	__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);

	// 输入捕获极性
	// #define  TIM_ICPOLARITY_RISING             TIM_INPUTCHANNELPOLARITY_RISING
	// #define  TIM_ICPOLARITY_FALLING            TIM_INPUTCHANNELPOLARITY_FALLING
	// #define  TIM_ICPOLARITY_BOTHEDGE           TIM_INPUTCHANNELPOLARITY_BOTHEDGE
	timer1_ic1.ICPolarity = TIM_ICPOLARITY_RISING;
	// 输入捕获通道选择（直接、间接或第三输入）
	// #define TIM_ICSELECTION_DIRECTTI           TIM_CCMR1_CC1S_0                 // 直接连接到对应引脚（常用）
	// #define TIM_ICSELECTION_INDIRECTTI         TIM_CCMR1_CC1S_1                 // 间接连接（用于捕获互补信号）
	// #define TIM_ICSELECTION_TRC                TIM_CCMR1_CC1S                   // 触发输入（用于同步、从模式）
	timer1_ic1.ICSelection = TIM_ICSELECTION_DIRECTTI;
	// 输入捕获预分频器
	// #define TIM_ICPSC_DIV1                     0x00000000U                          /*!< Capture performed each time an edge is detected on the capture input */
	// #define TIM_ICPSC_DIV2                     TIM_CCMR1_IC1PSC_0                   /*!< Capture performed once every 2 events                                */
	// #define TIM_ICPSC_DIV4                     TIM_CCMR1_IC1PSC_1                   /*!< Capture performed once every 4 events                                */
	// #define TIM_ICPSC_DIV8                     TIM_CCMR1_IC1PSC                     /*!< Capture performed once every 8 events                                */
	timer1_ic1.ICPrescaler = TIM_ICPSC_DIV1;
	// 输入滤波器设置 0x0 ~ 0xF
	timer1_ic1.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&timer1, &timer1_ic1, TIM_CHANNEL_1);
	
	timer1_ic2.ICPolarity = TIM_ICPOLARITY_RISING;
	timer1_ic2.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer1_ic2.ICPrescaler = TIM_ICPSC_DIV1;
	timer1_ic2.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&timer1, &timer1_ic2, TIM_CHANNEL_2);
	
	timer1_ic3.ICPolarity = TIM_ICPOLARITY_RISING;
	timer1_ic3.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer1_ic3.ICPrescaler = TIM_ICPSC_DIV1;
	timer1_ic3.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&timer1, &timer1_ic3, TIM_CHANNEL_3);
	
	timer1_ic4.ICPolarity = TIM_ICPOLARITY_RISING;
	timer1_ic4.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer1_ic4.ICPrescaler = TIM_ICPSC_DIV1;
	timer1_ic4.ICFilter = 0x08;
	HAL_TIM_IC_ConfigChannel(&timer1, &timer1_ic4, TIM_CHANNEL_4);
	
	__HAL_TIM_ENABLE_IT(&timer1, TIM_IT_UPDATE);
	
	HAL_TIM_IC_Start_IT(&timer1,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&timer1,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&timer1,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&timer1,TIM_CHANNEL_4);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitType;
	
	if(htim->Instance == TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();		
		
		GPIO_InitType.Pin = GPIO_PIN_8;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);

		GPIO_InitType.Pin = GPIO_PIN_9;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);	

		GPIO_InitType.Pin = GPIO_PIN_10;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);

		GPIO_InitType.Pin = GPIO_PIN_11;
		GPIO_InitType.Mode = GPIO_MODE_INPUT;
		GPIO_InitType.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA,&GPIO_InitType);		

		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
		
		HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1){
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
			u2_printf("HAL_TIM_IC_CaptureCallback 1 %d\r\n",HAL_TIM_ReadCapturedValue(&timer1,TIM_CHANNEL_1)+(counter * 65536));
		}else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
			u2_printf("HAL_TIM_IC_CaptureCallback 2 %d\r\n",HAL_TIM_ReadCapturedValue(&timer1,TIM_CHANNEL_2)+(counter * 65536));
		}else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			u2_printf("HAL_TIM_IC_CaptureCallback 3 %d\r\n",HAL_TIM_ReadCapturedValue(&timer1,TIM_CHANNEL_3)+(counter * 65536));
		}else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
			u2_printf("HAL_TIM_IC_CaptureCallback 4 %d\r\n",HAL_TIM_ReadCapturedValue(&timer1,TIM_CHANNEL_4)+(counter * 65536));
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1){
		u2_printf("HAL_TIM_PeriodElapsedCallback :%d\r\n", ++counter);
	}
}


#include "stm32f1xx_hal.h"
#include "rcc.h"

void RccClock_Init(void) {
	// 使用内部HSI时钟源通过PLL倍频到64MHz
	RCC_OscInitTypeDef RCC_OscInitType;
	RCC_OscInitType.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitType.HSIState = RCC_HSI_ON;
	RCC_OscInitType.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitType.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitType.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitType.PLL.PLLMUL = RCC_PLL_MUL16;
	HAL_RCC_OscConfig(&RCC_OscInitType);
	
	RCC_ClkInitTypeDef RCC_ClkInitType;
	RCC_ClkInitType.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitType.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitType.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitType.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitType.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitType,  FLASH_LATENCY_2);
}

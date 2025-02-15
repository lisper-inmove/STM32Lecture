#include "stm32f1xx_hal.h"
#include "sw.h"

// 0: 没有按下
// 1: 按下了
uint8_t sw8_sta = 0;
uint8_t sw11_sta = 0;

void SW_Init(void) {
	GPIO_InitTypeDef GPIO_InitType;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_13;
	GPIO_InitType.Mode = GPIO_MODE_INPUT;
	GPIO_InitType.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitType);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_0;
	GPIO_InitType.Mode = GPIO_MODE_INPUT;
	GPIO_InitType.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitType);
}

void SW_Init_IT(uint8_t mode) {
	GPIO_InitTypeDef GPIO_InitType;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_13;
	if(mode == 0) {
		GPIO_InitType.Mode = GPIO_MODE_IT_RISING;
	} else {
		GPIO_InitType.Mode = GPIO_MODE_IT_FALLING;
	}
	GPIO_InitType.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitType);
	// EXTI15_10_IRQn 表示中断号
	// 中断发生时 EXTI15_10_IRQHandler 函数被调用
	// 我们可以选择在 EXTI15_10_IRQHandler函数中处理逻辑
	// 也可以在 EXTI15_10_IRQHandler函数中调用 HAL_GPIO_EXTI_IRQHandler
	//     这个函数会先清除 引脚上的 中断设置，然后调用 HAL_GPIO_EXTI_Callback
	//     处理逻辑就写在 HAL_GPIO_EXTI_Callback 函数中
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_0;
	if(mode == 0) {
		GPIO_InitType.Mode = GPIO_MODE_IT_FALLING;
	} else {
		GPIO_InitType.Mode = GPIO_MODE_IT_RISING;
	}
	GPIO_InitType.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitType);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void SW_Init_EVT(void) {
	GPIO_InitTypeDef GPIO_InitType;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_0;
	GPIO_InitType.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitType.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitType);
}

void SW_Init_EvtOutput(void) {
	GPIO_InitTypeDef GPIO_InitType;	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	GPIO_InitType.Pin = GPIO_PIN_0;
	GPIO_InitType.Mode = GPIO_MODE_AF_PP;
	GPIO_InitType.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitType);
	HAL_GPIOEx_ConfigEventout(AFIO_EVENTOUT_PORT_A, AFIO_EVENTOUT_PIN_0);
	HAL_GPIOEx_EnableEventout();
}


// 返回值  0: 无按钮触发
//         8: SW8触发
//  mode 0: 按下执行, 1: 抬起执行

uint32_t PRESS_DOWN_LOOP_NUMBER = 0x7FFF;
uint32_t PRESS_UP_LOOP_NUMBER = 0x7FFF;
// mode 为0时，修改 PRESS_DOWN_LOOP_NUMBER 可以控制长按，短按 触发
// mode 为1时，修改 PRESS_UP_LOOP_NUMBER 可以控制 延迟触发
uint8_t SW_Scan(uint8_t mode) {
	uint32_t i;
	/*-------------- SW8 ------------------*/
	if ((SW8_IN == 1) && (sw8_sta == 0)) {
		
		// 消抖
		for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
			if (SW8_IN == 0)
				return 0;
		}
		sw8_sta = 1;
		
		// mode为1时，这个if只是为了修改sw8_sta，从而在抬起之后的下一次可以进入到else
		if (mode == 0) {
			return 8;
		}
	} else if ((SW8_IN == 0) && (sw8_sta == 1)) {
		for (i = 0; i < PRESS_UP_LOOP_NUMBER; i++) {
			if (SW8_IN == 1) {
				return 0;
			}
		}
		sw8_sta = 0;
		
		// mode为0时，这个if只是为了修改sw8_sta，从而在下次按下之后可以进入到上面的if
		if (mode == 1) {
			return 8;
		}
	}
	
	/*-------------- SW11 ------------------*/
	if ((SW11_IN == 0) && (sw11_sta == 0)) {
		for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
			if (SW11_IN == 1)
				return 0;
		}
		sw11_sta = 1;
		if (mode == 0) {
			return 11;
		}
	} else if ((SW11_IN == 1) && (sw11_sta == 1)) {
		for (i = 0; i < PRESS_UP_LOOP_NUMBER; i++) {
			if (SW11_IN == 0) {
				return 0;
			}
		}
		sw11_sta = 0;
		if (mode == 1) {
			return 11;
		}
	}
	return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN) {
	uint32_t i;
	switch(GPIO_PIN) {
		case GPIO_PIN_13:
				if (SW8_IN == 1) {
						for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
							if (SW8_IN == 0) {
								return;
							}
					}
					__SEV();
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
				} else {
					for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
							if (SW8_IN == 1) {
								return;
							}
					}
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
				}
				break;
		case GPIO_PIN_0:
				if (SW11_IN == 0) {
					for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
						if (SW11_IN == 1) {
							return;
						}
					}
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
				} else {
					for (i = 0; i < PRESS_DOWN_LOOP_NUMBER; i++) {
						if (SW11_IN == 0) {
							return;
						}
					}
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
				}
				break;
	}
}

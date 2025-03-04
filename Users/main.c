#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "rcc.h"
#include "uart.h"
#include "util.h"
#include "timer.h"

int main(void) {
  HAL_Init();
  RccClock_Init();
  U1_Init(921600);
  u1_printf("Program start: %d\n", 10);
	// 计时器开始计时
	Timer1_Init(3999, 35999, 0);
	uint8_t counter = 0;
  while (1) {
		// 轮询方式，需要在循环中判断标志位
    if (__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_UPDATE)) {
			// 清掉该标志位，避免重复处理
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
			u1_printf("Timer1 alarmed... %d\n", counter++);
			if (counter >= 3) {
				HAL_TIM_Base_Stop(&timer1);
				HAL_TIM_Base_DeInit(&timer1);
				u1_printf("Timer1 stopped...");
			}
		}
  }
}

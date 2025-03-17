#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "util.h"
#include "timer.h"

int main(void) {
  HAL_Init();
  RccClock_Init();
  UART2_Init(921600);
	Timer1_Init(4, 0, 0);
	// RX 连接 PA2
  u2_printf("Program start: %d\n", 10);
	
	// PA8 连接 SW8
	// PA9 连接 SW11
	// PA12 连接 SW12
  while (1) {
		if (__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_UPDATE)) {
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
			u2_printf("Timer1 Update.");
		}
		if (__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_TRIGGER)) {
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_TRIGGER);
			u2_printf("Timer1 Trigger.");
		}
	}
}

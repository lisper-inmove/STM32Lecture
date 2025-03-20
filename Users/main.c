#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "util.h"
#include "timer.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	UART2_Init(921600);
	Timer1_Init(1, 0, 0);
	u2_printf("Program start: %d\n", 10);
	
	// 接线:
	// PA8  SW8
	// PA9  SW11
	// PA12 SW12
	while (1) {
		HAL_Delay(1000);
		u2_printf("Value of arr, %d\n", timer1.Instance->ARR);
	}
}

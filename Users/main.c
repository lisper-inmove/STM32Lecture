#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "util.h"
#include "timer.h"

int main(void) {
	HAL_Init();
	RccClock_Init();
	UART2_Init(921600);
	Timer1_Init(4, 0, 0);
	u2_printf("Program start: %d\n", 10);
	
	while (1) {}
}

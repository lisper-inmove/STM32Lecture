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
	Timer1_Init(3999, 35999, 0);
  while (1) {
    
  }
}

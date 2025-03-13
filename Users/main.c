#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "rcc.h"
#include "uart.h"

int main(void) {
  HAL_Init();
  RccClock_Init();
  U2_Init(921600);
  u_printf("Program start11: %d\n", 10);
  while (1) {

  }
}

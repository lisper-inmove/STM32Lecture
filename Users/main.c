#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "util.h"

int main(void) {
  HAL_Init();
  RccClock_Init();
  UART1_Init(921600);
  u1_printf("Program start: %d\n", 10);
  while (1) {}
}

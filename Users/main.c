#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "uart.h"

int main(void) {
  HAL_Init();
  RccClock_Init();
  U1_Init(921600);
  u1_printf("Program start: %d\n", 10);
  while (1) {}
}

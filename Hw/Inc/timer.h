#ifndef TIMER_H
#define TIMER_H

#include "stm32f1xx_hal_tim.h"

extern TIM_HandleTypeDef timer1;
void Timer1_Init(uint16_t arr, uint16_t psc, uint8_t rep);

extern DMA_HandleTypeDef  timer1_dmaup;

#endif /* TIMER_H */

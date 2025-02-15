#ifndef __SW_H
#define __SW_H

#define SW8_IN HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)
#define SW11_IN HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

void SW_Init(void);
void SW_Init_IT(uint8_t mode);
void SW_Init_EVT(void);
uint8_t SW_Scan(uint8_t mode);

#endif

#include "stm32f1xx_hal.h"
#include "rcc.h"
#include "util.h"
#include "timer.h"

#define ARR_VALUE 0xFFFF

int main(void) {
	HAL_Init();
	RccClock_Init();
	UART2_Init(921600);
	Timer1_Init(ARR_VALUE, 7200 - 1, 0);
	u2_printf("Program start: %d\n", 10);
	int32_t counter = 0;
	
	// 接线
	// PA8 SW8
	// PA9 SW9
	// PA10 SW10
	// PA11 SW11
	// USB_RX PA2
	while (1) {
		// HAL_Delay(1000);
		// u2_printf("Value of arr, %d\n", timer1.Instance->ARR);
		if(__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_UPDATE)){
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_UPDATE);
			u2_printf("Timer update: %d\r\n",++counter);
		}
		if(__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_CC1)){
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_CC1);
			u2_printf("Timer1 IC1 %d\r\n",HAL_TIM_ReadCapturedValue(&timer1, TIM_CHANNEL_1)+(counter * ARR_VALUE));
		}
		if(__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_CC2)){
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_CC2);
			u2_printf("Timer1 IC2 %d\r\n", HAL_TIM_ReadCapturedValue(&timer1, TIM_CHANNEL_2)+(counter * ARR_VALUE));
		}
		if(__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_CC3)){
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_CC3);
			u2_printf("Timer1 IC3 %d\r\n", HAL_TIM_ReadCapturedValue(&timer1, TIM_CHANNEL_3)+(counter * ARR_VALUE));
		}
		if(__HAL_TIM_GET_FLAG(&timer1, TIM_FLAG_CC4)){
			__HAL_TIM_CLEAR_FLAG(&timer1, TIM_FLAG_CC4);
			u2_printf("Timer1 IC4 %d\r\n", HAL_TIM_ReadCapturedValue(&timer1, TIM_CHANNEL_4)+(counter * ARR_VALUE));
		}
	}
}

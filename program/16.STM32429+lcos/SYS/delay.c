#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

extern volatile int OS_TimeMS;
//延时nms
//nms:要延时的ms数
void delay_ms(u32 nms)
{
	u32 time;
	time = OS_TimeMS;
	while((OS_TimeMS-time) < nms);
}



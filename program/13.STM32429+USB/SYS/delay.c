#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

extern volatile int OS_TimeMS;
void delay_us(u32 nus)
{
	int time;
	time = OS_TimeMS;
	while((OS_TimeMS-time) < nus);
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{
	int time;
	time = OS_TimeMS;
	while((OS_TimeMS-time) < (nms*1000));
}



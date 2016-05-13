#ifndef __lcd_300_224_H
#define __lcd_300_224_H	

#include "stm32f4xx.h"
#include "stm32f429_lcd.h"
#include "delay.h"

void LCOS_SPI_Configuration(void);
void LCOS_regs_init(void);
void power_init(void);
void LCOS_Init(void);

#endif

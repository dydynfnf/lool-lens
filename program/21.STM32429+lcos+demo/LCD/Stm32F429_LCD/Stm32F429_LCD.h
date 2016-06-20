#ifndef __STM32F429_LCD_H
#define __STM32F429_LCD_H

#include "stm32f4xx_ltdc.h"
#include "stm32f4xx_dma2d.h"
#include "delay.h"

void LCD__Init(void);
void LCD_LayerInit(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void DMA2D_Config(void);
void DMA2D_Display(void);
void LCD_AF_GPIOConfig(void);

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif 






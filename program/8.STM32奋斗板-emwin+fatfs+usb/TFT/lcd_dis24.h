/**
  ******************************************************************************
  * @file FSMC/SRAM/fsmc_sram.h 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Header for fsmc_sram.c file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_DIS24_H
#define __LCD_DIS24_H

#include "stm32f10x.h"

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ram
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ram

void Delay(__IO uint32_t nCount);
void LCD_WR_REG(u16 index);
void LCD_WR_CMD(u16 index,u16 val);
u16 LCD_RD_Data(void);
void LCD_WR_Data(u16 val);
void LCD_WR_Data_8(u16 val);

void LCD_init(void);
void TFT_CLEAR(u16 x,u16 y,u32 len,u32 wid);
void TFT_DrawPoint(u16 x,u16 y, u16 color);
u16 TFT_ReadPoint(u16 x,u16 y);
void TFT_HLine(u16 x0,u16 y,u16 x1,u16 color);
void TFT_VLine(u16 x,u16 y0,u16 y1,u16 color);
void TFT_Fill(u16 x0,u16 y0, u16 x1, u16 y1, u16 color);
void TFT_ShowBmp(u16 x,u16 y,u16 lenth,u16 wide,const u8 *p);

#endif /* __LCD_DIS24_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithLCD/main.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    13-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_480_272.h"
#include "GUI.h"
//#include "GUIDEMO.h"

extern volatile int OS_TimeMS;  
unsigned char i=0,j;
int main(void)
{
	GUI_MEMDEV_Handle hMem0,hMem1;
	SysTick_Config(180000);
	SDRAM_Init();
	
	LCD_Config();
	
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();

//	GUI_DispStringAt("blacklight",0,100);
	
//	GUIDEMO_Main();
	
	hMem0 = GUI_MEMDEV_CreateFixed(0,0,100,50,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_888);
	hMem1 = GUI_MEMDEV_CreateFixed(0,0,100,50,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_888);
	GUI_MEMDEV_Select(hMem0);
	GUI_Clear();
	GUI_DispString("Text");
	GUI_MEMDEV_CopyToLCDAt(hMem0,0,0);
	
	while(1)
	{	
		GUI_MEMDEV_Select(hMem1);
		GUI_MEMDEV_DrawPerspectiveX(hMem0,0,0,50,50-50*i/100,100-100*i/100,25*i/100);
		GUI_MEMDEV_CopyToLCDAt(hMem1,i,50+i);
		
		GUI_Delay(10);
		
		GUI_MEMDEV_Select(hMem1);
		GUI_Clear();
		GUI_MEMDEV_CopyToLCDAt(hMem1,i,50+i);
		
		i+=1;
		if(i>100)
		{
			i=0;
		}
	}
	
	while(1);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

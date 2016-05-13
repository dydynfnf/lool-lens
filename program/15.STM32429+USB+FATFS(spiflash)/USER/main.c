#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "NandFlash.h"
#include "W25Qxx.h"
#include "sdram.h"
#include "lcd_480_272.h"
#include "delay.h"
#include "GUI.h"

#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h"
#include <stdio.h>

#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

#include "picture.h"
#include "font.h"

#include "power.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

#define _DF1S	0x81

extern volatile int OS_TimeMS;

FATFS fs;
extern GUI_FONT XBF_Font;
extern u32 FrameBuffer2[544][480];
char text[]={"Expanding the i.MX 6 series, the i.MX 6UltraLite is a high performance, \
ultra-efficient processor family featuring an advanced implementation of a single ARM® \
Cortex®-A7 core, which operates at speeds up to 528 MHz. The i.MX 6UltraLite applications \
processor includes an integrated power management module that reduces the complexity of \
external power supply and simplifies power sequencing. Each processor in this family provides \
various memory interfaces, including 16-bit LPDDR2, DDR3, DDR3L, raw and managed NAND flash,\
NOR flash, eMMC, Quad SPI and a wide range of other interfaces for connecting peripherals \
such as WLAN, Bluetooth™, GPS, displays and camera sensors."};
int main(void)
{
	u32 i=0,dir=1;
	GUI_RECT rect={10,10,232,262};
	GUI_MEMDEV_Handle hMem1,hMem2;
	
	FILINFO finfo;
	#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
  #endif
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	power_init();
	SysTick_Config(180000);
	SDRAM_Init();
	W25QXX_Init();
	NAND_Init();
	NAND_Reset();
	
//////////////////////////////////////////////
//////////////////////////////////////////////	

	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
//	while(1);
	
	disk_initialize(0);
	f_mount(&fs,"0:",1);

////////////////////////////////////////////////	
////////////////////////////////////////////////

	LCD_Config();
	
	GUI_SelectLayer(0);
	GUI_Init();
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_Clear();

	GUI_SelectLayer(1);
	GUI_SetColor((200<<24) | GUI_WHITE);
	GUI_FillRoundedRect(0,0,242,270,10);
	GUI_SelectLayer(0);

//	hMem1 = GUI_MEMDEV_CreateFixed(0,0,484,272,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_888);
//	GUI_MEMDEV_Select(hMem1);
	_ShowJPG("0:/test.jpg",0,0);
	_ShowPNG("0:/test.png",320,0);
//	GUI_MEMDEV_CopyToLCD(hMem1);
	
	_ShowJPG("0:/gp.jpg",0,272);
	
	_ShowXBF();
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor( (200<<24) | GUI_WHITE);
	GUI_DispStringAt("15℃ ",240,200);
	
	while(i!=271)
	{		
		GUI_Delay(1);
		if(dir==1)
		{
			i++;
			if(i==272)
			{
				dir=0;
			}
		}
		if(dir==0)
		{
			i--;
			if(i==0)
			{
				dir=1;
			}
		}
		GUI_SelectLayer(1);
		GUI_SetOrg(0, i);
		GUI_SelectLayer(0);
		GUI_SetOrg(0, i);
	}		  

	GUI_SelectLayer(1);
//	hMem1 = GUI_MEMDEV_CreateFixed(0,0,242,272,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
//	hMem2 = GUI_MEMDEV_CreateFixed(0,272,242,272,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
//	GUI_MEMDEV_Select(hMem1);
//	GUI_FillRoundedRect(0,0,240,270,10);
//	GUI_MEMDEV_CopyFromLCD(hMem2);
//	for(i=0;i<272;i++)
//	{
//		GUI_MEMDEV_CopyFromLCD(hMem2);
//		GUI_MEMDEV_CopyToLCDAt(hMem1,0,i);
//		GUI_Delay(100);
//		GUI_MEMDEV_CopyToLCDAt(hMem2,0,272);
//	}
//	GUI_MEMDEV_CopyToLCDAt(hMem1,0,272);
		GUI_FillRoundedRect(0,0,242,270,10);
	for(i=272;i>0;i--)
	{
		GUI_SetOrg(0, i);
		GUI_Delay(1);
	}
	
	GUI_SetFont(&GUI_Font16B_ASCII);
	for(i=0;i<272;i+=1)
	{
		rect.y0=10+i;
		rect.y1=262+i;
		GUI_SetColor( (200<<24) | GUI_BLACK);
		GUI_DispStringInRectWrap(text,&rect,GUI_TA_LEFT|GUI_TA_TOP,GUI_WRAPMODE_WORD);
		GUI_Delay(60);
		GUI_SetColor( (200<<24) | GUI_WHITE);
		GUI_FillRoundedRect(0,0,242,270,10);
	}
	
	while(1);
	
}






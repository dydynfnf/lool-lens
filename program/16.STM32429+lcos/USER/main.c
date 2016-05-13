#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "NandFlash.h"
#include "W25Qxx.h"
#include "sdram.h"
#include "lcos_300_224.h"
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
extern u32 FrameBuffer1[448][900];
extern u32 FrameBuffer2[448][900];
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
	u32 i=0,j=0,dir=1;
	GUI_RECT rect={160,10,290,102};
	GUI_MEMDEV_Handle hMem1,hMem2;
	
	FILINFO finfo;
	#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
  #endif
	
	power_init();
	SysTick_Config(168000);
	delay_ms(1000);
	SDRAM_Init();
	W25QXX_Init();
	NAND_Init();
	NAND_Reset();
  
//////////////////////////////////////////////
//////////////////////////////////////////////	

//	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	
	disk_initialize(0);
	f_mount(&fs,"0:",1);

//////////////////////////////////////////////	
//////////////////////////////////////////////
	
	LCOS_Init();
	GUI_Init();
	
	while(1)
	{
		
	GUI_SelectLayer(1);
	GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_SelectLayer(0);
		
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_Clear();
	
	_ShowXBF();
	GUI_UC_SetEncodeUTF8();
	GUI_DispStringAt("棱镜测试 ",0,112);
		
	DMA2D_Display();
	
	_ShowJPG("0:/test.jpg",0,0);
	_ShowPNG("0:/test.png",0,0);
	_ShowJPG("0:/gp.jpg",0,224);
	
	_ShowXBF();
	GUI_UC_SetEncodeUTF8();
	GUI_SetColor( (200<<24) | GUI_WHITE);
	GUI_DispStringAt("15℃ ",0,112);
	
	GUI_SetOrg(0, 224);
	DMA2D_Display();
	
	for(i=224;i>3;i-=3)
	{		
		GUI_SetOrg(0, i);
		DMA2D_Display();
	}		  

	GUI_SetOrg(0, 0);
	DMA2D_Display();
	
	GUI_SelectLayer(1);
	GUI_SetColor((150<<24)|GUI_WHITE);
	GUI_FillRoundedRect(150,0,300,112,10);
	for(i=112;i>10;i-=3)
	{
		GUI_SetOrg(0, i);
		DMA2D_Display();
	}
	
	GUI_SetFont(&GUI_Font16B_ASCII);
	for(i=112;i>10;i-=3)
	{
		rect.y0=10+i;
		GUI_SetColor((255<<24)|GUI_BLACK);
		GUI_DispStringInRectWrap(text,&rect,GUI_TA_LEFT|GUI_TA_TOP,GUI_WRAPMODE_WORD);
		DMA2D_Display();
		GUI_SetColor((150<<24)|GUI_WHITE);
		GUI_DispStringInRectWrap(text,&rect,GUI_TA_LEFT|GUI_TA_TOP,GUI_WRAPMODE_WORD);
	}
	
	GUI_Delay(2000);
	
	}
	
}


//int main(void)
//{
//	u32 i=0,j=0;
//	u8 k=0;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
//	power_init();
//	SysTick_Config(180000);
//	SDRAM_Init();
//	W25QXX_Init();
//	NAND_Init();
//	NAND_Reset();
//	delay_ms(200);
//	LCOS_Init();
//	DMA2D_Config();
//	
//	for(i=0;i<448;i++)
//	{
//		for(j=0;j<900;j++)
//		{
//			FrameBuffer1[i][j]=0xff0000ff;
//		}
//	}
//	for(i=0;i<448;i++)
//	{
//		for(j=0;j<900;j++)
//		{
//			FrameBuffer2[i][j]=0x640000ff;
//		}
//	}
//	for(i=100;i<150;i++)
//	{
//		for(j=1;j<200;j+=3)
//		{
//			FrameBuffer1[i][j]=0xff000000;
//		}
//	}
//	for(i=0;i<50;i++)
//	{
//		for(j=1;j<200;j+=3)
//		{
//			FrameBuffer2[i][j]=0x60000000;
//		}
//	}
//	
//	DMA2D_Config();
//  DMA2D_StartTransfer();
//	while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);
//	
//	while(1)
//	{
//		for(i=100;i<150;i++)
//		{
//			for(j=1;j<200;j+=3)
//			{
//				FrameBuffer1[i][j]=0xff0000ff-k;
//			}
//		}
//		DMA2D_StartTransfer();
//		while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);
//		k++;
//		delay_ms(10);
//	}
////  while (1)
////  {
////		DMA2D->FGMAR = (uint32_t)FrameBuffer1+900*i*4;
////		DMA2D_StartTransfer();
////		while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);
////		i++;
////		delay_ms(10);
////		if(i>100)
////		{
////			i=0;
////		}
////  }
//}





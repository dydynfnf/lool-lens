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

#include "demo.h"

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
extern u8 MEM_EN;
extern u8 Layer0_alpha;
extern u8 Layer1_alpha;

GUI_POINT Points_nav[] = {
{217, 193+Down_Vscreen},
{217, 154+Down_Vscreen},
{211, 143+Down_Vscreen},
{197, 130+Down_Vscreen},
{197, 111+Down_Vscreen},
{204, 101+Down_Vscreen},
{216, 97+Down_Vscreen},
{237, 98+Down_Vscreen},
{247, 93+Down_Vscreen},
{253, 84+Down_Vscreen},
{250, 73+Down_Vscreen},
{236, 55+Down_Vscreen},
{221, 46+Down_Vscreen},
{205, 39+Down_Vscreen},
{181, 35+Down_Vscreen},
};

int main(void)
{
	u32 i=0,j=0,dir=1;
	
	FILINFO finfo;
	#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
  #endif
	
	//power_init();//电路更新 不在需要
	SysTick_Config(168000);
	delay_ms(1000);
	SDRAM_Init();
	W25QXX_Init();
	NAND_Init();
	NAND_Reset();
  
//////////////////////////////////////////////
//////////////////////////////////////////////	

	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	//while(1);
	
	disk_initialize(0);
	f_mount(&fs,"0:",1);

//////////////////////////////////////////////	
//////////////////////////////////////////////
	
	LCOS_Init();
	GUI_Init();

	GUI_SelectLayer(1);
	_GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	GUI_SelectLayer(0);
	_GUI_SetBkColor(GUI_BLACK);
	GUI_Clear();
	
	time(Up_Vscreen);
	
	weather(Middle_Vscreen);
	
	nav(Down_Vscreen);
	
	while(1)
	{
		move_to_vscreen(Up_Vscreen,Middle_Vscreen);
		for(i=0;i<2;i++)
		{
			turn_over(0,0,Middle_Vscreen+120,135-1,Middle_Vscreen+160-1);
			delay_ms(300);
			turn_over(0,0,Middle_Vscreen+160,135-1,Middle_Vscreen+223-1);
			delay_ms(700);
		}
		
		move_to_vscreen(Middle_Vscreen,Down_Vscreen);
		GUI_SelectLayer(0);
		draw_nav_line(Points_nav,15,(0xff<<24) | (120<<16) | (120<<8) | 120,7);
		GUI_SelectLayer(1);
		draw_nav_destination(Points_nav[14].x,Points_nav[14].y);
		draw_nav_arrow(Points_nav[0].x,Points_nav[0].y);
		GUI_Delay(1000);
		GUI_SelectLayer(0);
		
		move_to_vscreen(Down_Vscreen,Up_Vscreen);
		_ShowXBF("0:/yahei_48pt.xbf");
		for(i=0;i<4;i++)
		{
			_GUI_SetColor(GUI_WHITE);
			GUI_DispStringAt(":",135,Up_Vscreen+135);
			DMA2D_Display();
			delay_ms(500);
			_GUI_SetColor(GUI_BLACK);
			GUI_DispStringAt(":",135,Up_Vscreen+135);
			
			_GUI_SetColor(GUI_WHITE);
			GUI_DispStringAt(" ",135,Up_Vscreen+135);
			DMA2D_Display();
			delay_ms(500);
			_GUI_SetColor(GUI_BLACK);
			GUI_DispStringAt(" ",135,Up_Vscreen+135);
		}
	}

//	GUI_SetColor(GUI_WHITE);
//	GUI_FillRect(0,0,299,223);
//	GUI_SetColor(GUI_RED);
//	GUI_SetPenSize(4);
//	DMA2D_Display();
//	for(i=0;i<360;i+=5)
//	{
//		rotate(&p_ref,p_origin,p_rotated,2,i);
//		GUI_DrawPoint(p_rotated[0].x,p_rotated[0].y);
//		GUI_DrawPoint(p_rotated[1].x,p_rotated[1].y);
//	}
//	DMA2D_Display();
//	while(1);
}



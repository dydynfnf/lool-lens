#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "NandFlash.h"
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

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

#define _DF1S	0x81

extern volatile int OS_TimeMS;

////////////////////////////////////////////////////////////////////////////////////////////

FIL file,fdst;
u32 bw;
static void _ShowJPG(const char * sFilename)
{
	GUI_HMEM hMem;
	char *_acBuffer2;
	u8 result;

	hMem = GUI_ALLOC_AllocZero(1024*512);

	_acBuffer2 = GUI_ALLOC_h2p(hMem);

	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}

	result = f_read(&file, _acBuffer2, file.fsize, &bw);
	if (result != FR_OK)
	{
		return;
	}

	//GUI_JPEG_GetInfo(_acBuffer2, file.fsize, &JpegInfo);
	GUI_JPEG_Draw(_acBuffer2,file.fsize,0,0);

	GUI_ALLOC_Free(hMem);
	f_close(&file);
}

static char _acBuffer[512];
GUI_JPEG_INFO JpegInfo;
FRESULT result;
GUI_JPEG_INFO JpegInfo;
 
 
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
     static int FileAddress = 0;
     UINT NumBytesRead;
     FIL *PicFile;
 
     PicFile = (FIL *)p;
 
    
     if (NumBytesReq > sizeof(_acBuffer)) {
     NumBytesReq = sizeof(_acBuffer);
     }
 
 
    
     if(Off == 1) FileAddress = 0;
     else FileAddress = Off;
     result =f_lseek(PicFile, FileAddress);
 
 
    
     result = f_read(PicFile, _acBuffer, NumBytesReq, &NumBytesRead);
 
    
     *ppData = (const U8 *)_acBuffer;
 
    
     return NumBytesRead;
}
 
 
static void _ShowJPGEx(const char * sFilename)
{   
 
             
     result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
     if (result != FR_OK)
     {
         return;
     }
    
     GUI_JPEG_GetInfoEx(_GetData, &file, &JpegInfo);
              GUI_JPEG_DrawScaledEx(_GetData,
                                     &file,
                                       (LCD_GetXSize() - JpegInfo.XSize)/2,
                                       (LCD_GetYSize() - JpegInfo.YSize)/2,
                                       1,
                                       1); 
   f_close(&file);
}
////////////////////////////////////////////////////////////////////////////////////////////

FATFS fs;
//FIL fsrc, fdst; 
u32 i=0;

int main(void)
{        
	GUI_MEMDEV_Handle hMem0,hMem1;
	FILINFO finfo;
  DIR dirs;
	char path[50]={""};
	#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
  #endif
	
	SysTick_Config(18000);
	SDRAM_Init();
	NAND_Init();
	NAND_Reset();
	
//////////////////////////////////////////////
//////////////////////////////////////////////	

//	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
//	while(1);
	disk_initialize(0);
	f_mount(&fs,"0:",1);
	
if (f_opendir(&dirs, path) == FR_OK)            //读取该磁盘的根目录        
{
	while (f_readdir(&dirs, &finfo) == FR_OK)  	 //循环依次读取文件名
	{	 
		if (finfo.fattrib & AM_ARC) 			     //判断文件属性是否为存档型	 TXT文件一般都为存档型
		{
			if(!finfo.fname[0])	 break;    		     //如果是文件名为空表示到目录的末尾。退出	 
			if(finfo.lfname[0])
			{
			}
			else
			{						  			    	                     //8.3格式文件名
			}			
			f_close(&fdst);	    											   //关闭目标文件
		}
	} 
} 

////////////////////////////////////////////////	
////////////////////////////////////////////////

	LCD_Config();
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();

//	GUI_DispStringAt("blacklight",0,100);
	_ShowJPGEx("0:/test.jpg");
	
	GUI_SetColor(GUI_GREEN);
	GUI_FillRect(0, 273, 480, 544);
	GUI_SetOrg(0, 0); /* Set origin to screen 1 */
	GUI_Delay(1000);
	GUI_SetOrg(0, 272); /* Set origin to screen 2 */
	GUI_SetColor(GUI_YELLOW);
	
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
		
		GUI_Delay(1);
		
		GUI_MEMDEV_Select(hMem1);
		GUI_Clear();
		GUI_MEMDEV_CopyToLCDAt(hMem1,i,50+i);
		
		i+=1;
		if(i>100)
		{
			i=0;
		}
		GUI_SetOrg(0, i);
	}		  
		
}






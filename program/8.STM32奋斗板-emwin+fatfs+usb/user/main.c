#define GLOBALS

#include "includes.h"
#include "stm32f10x_exti.h"
#include "misc.h"

#include "fsmc_sram.h"
#include "GUI.h"
#include "lcd_dis24.h"
#include "GUIDEMO.h"

#include "sdio_sdcard.h"
#include "integer.h"
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "mass_mal.h"
#include "stdio.h"	   
#include "demo.h"

/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;			     //SDIO中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //抢先优先级0  范围：0或1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //子优先级0	范围：0-7
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USB低优先级中断请求
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//抢占优先级 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//子优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB高优先级中断请求
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//抢占优先级 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 名    称：void SD_TEST(void)
* 功    能：SD卡测试函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 

SD_Error SD_Status = SD_OK;
extern SD_CardInfo SDCardInfo;
extern uint32_t Buffer_Block_Tx[512], Buffer_Block_Rx[512];
void SD_TEST(void)
{
	SD_Status = SD_Init();
    SD_Status = SD_GetCardInfo(&SDCardInfo);
    SD_Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    SD_Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
    SD_Status = SD_SetDeviceMode(SD_DMA_MODE);  
 	if (SD_Status == SD_OK)
  	{
        // 从地址0开始读取512字节  
    	SD_Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  512); 
  	}
}

/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void){

    SystemInit();	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

static char _acBuffer[512];
GUI_JPEG_INFO JpegInfo;
FIL file;
FRESULT result;

//static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) {
//     static int FileAddress = 0;
//     FIL *file;
//     DWORD    NumBytesRead;
//     U8     * pData;
// 
//     pData  = (U8 *)*ppData;
//     file = (FIL *)p;
//     //
//     // Set file pointer to the required position
//     //
//     if(Off == 1) FileAddress = 0;
//     else FileAddress = Off;
//     result =f_lseek(file, FileAddress);
//     //
//     // Read data into buffer
//     //
//     result = f_read(file, pData, NumBytesReq, &NumBytesRead);
// 
//     //
//     // Return number of available bytes
//     //
//     return NumBytesRead;
// 
//}
// 
//static void _ShowPNGEx(const char * sFilename, uint16_t usPOSX, uint16_t usPOSY)
//{   
// 
//             
//     result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
//     if (result != FR_OK)
//     {
//         return;
//     }
//    
//     GUI_PNG_DrawEx(_GetData, &file, usPOSX, usPOSY);
// 
//     f_close(&file);
//}
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
   //f_close(&file);
}

/****************************************************************************
* 名    称：int main(void)
* 功    能：奋斗板初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
extern volatile int OS_TimeMS;
unsigned char i=0xff;
unsigned int pastms=0;

//extern uint16_t MAL_Init (uint8_t lun);
//extern void USB_Disconnect_Config(void);
FRESULT res;
int main(void)
{
//	GUI_HMEM hMem;
//	char *_acBuffer;
//	FIL file;
//	FIL fsrc;
	FATFS fs;
//	unsigned int br;
	
    RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能    
    FSMC_LCD_Init();				   //FSMC TFT接口初始?
	SysTick_Config(72000);
	LCD_init();
	
	NVIC_Configuration();
	SD_TEST();
	USB_Disconnect_Config();	//设置USB连接控制线    
	MAL_Init(0);  	        //存储介质初始化  
	NVIC_Configuration();     //USB通信中断配置
	USB_Init();				//USB初始化
	
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
//	hMem = GUI_ALLOC_AllocZero(512);
//	_acBuffer = GUI_ALLOC_h2p(hMem);
	
	disk_initialize(0);						     //fatfs可以管理多个介质的分区， 所以把物理存储介质SST25VF016B标示为0区，相当于磁盘编号
	res = f_mount(0, &fs);
	_ShowJPGEx("0:/weather/QQ.jpg");
	//_ShowPNGEx("0:/weather/cloudy1_night.png",100,100);
//	res = f_open(&file, "0:/weather/QQ.jpg", FA_OPEN_EXISTING | FA_OPEN_ALWAYS | FA_READ);
//	if(res != FR_OK)
//	{
//		res=0;
//	}
//	res = f_read(&file, _acBuffer, file.fsize, &br);
//	if(res != FR_OK)
//	{
//		res=0;
//	}
//	
//	GUI_JPEG_Draw(_acBuffer, file.fsize, 0, 0);
//	GUI_ALLOC_Free(hMem);
//	f_close(&file);
	
	GUI_DispStringAt("blacklight",0,0);
	//GUI_DispCharAt('鍟?,0,0);
	while(1);
	
}

/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

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
#include "demo.h"

/****************************************************************************
* Ãû    ³Æ£ºvoid NVIC_Configuration(void)
* ¹¦    ÄÜ£ºÖÐ¶ÏÔ´ÅäÖÃº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* ÓÅÏÈ¼¶×é1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;			     //SDIOÖÐ¶Ï
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //ÇÀÏÈÓÅÏÈ¼¶0  ·¶Î§£º0»ò1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //×ÓÓÅÏÈ¼¶0	·¶Î§£º0-7
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* Ãû    ³Æ£ºvoid SD_TEST(void)
* ¹¦    ÄÜ£ºSD¿¨²âÊÔº¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 

SD_Error SD_Status = SD_OK;
extern SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[512], Buffer_Block_Rx[512];

void SD_TEST(void)
{
	SD_Status = SD_Init();
    SD_Status = SD_GetCardInfo(&SDCardInfo);
    SD_Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    SD_Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
    SD_Status = SD_SetDeviceMode(SD_DMA_MODE);  
 	if (SD_Status == SD_OK)
  	{
        // ´ÓµØÖ·0¿ªÊ¼¶ÁÈ¡512×Ö½Ú  
    	SD_Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  512); 
  	}
}

/****************************************************************************
* Ãû    ³Æ£ºvoid RCC_Configuration(void)
* ¹¦    ÄÜ£ºÏµÍ³Ê±ÖÓÅäÖÃÎª72MHZ£¬ ÍâÉèÊ±ÖÓÅäÖÃ
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/ 
void RCC_Configuration(void){

    SystemInit();	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

static char _acBuffer[512];
GUI_JPEG_INFO JpegInfo;
FIL file;
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

/****************************************************************************
* Ãû    ³Æ£ºint main(void)
* ¹¦    ÄÜ£º·Ü¶·°å³õÊ¼»¯º¯Êý
* Èë¿Ú²ÎÊý£ºÎÞ
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ    Ã÷£º
* µ÷ÓÃ·½·¨£ºÎÞ 
****************************************************************************/  
extern volatile int OS_TimeMS;
unsigned char i=0xff;
unsigned int pastms=0;

int main(void)
{
	
	GUI_HMEM hMem;
	char *_acBuffer;
//	FIL file;
//	FRESULT res;
	FATFS fs;
	
	unsigned int br;
	
    RCC_Configuration();  	       //ÏµÍ³Ê±ÖÓ³õÊ¼»¯¼°¶Ë¿ÚÍâÉèÊ±ÖÓÊ¹ÄÜ    
    FSMC_LCD_Init();				   //FSMC TFT½Ó¿Ú³õÊ¼»
	SysTick_Config(72000);
	LCD_init();
	NVIC_Configuration();
	SD_TEST();
	
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
//	hMem = GUI_ALLOC_AllocZero(512);
//	_acBuffer = GUI_ALLOC_h2p(hMem);
	
	disk_initialize(0);						     //fatfs¿ÉÒÔ¹ÜÀí¶à¸ö½éÖÊµÄ·ÖÇø£¬ ËùÒÔ°ÑÎïÀí´æ´¢½éÖÊSST25VF016B±êÊ¾Îª0Çø£¬Ïàµ±ÓÚ´ÅÅÌ±àºÅ
	f_mount(0, &fs);
	_ShowJPGEx("0:/weather/QQ.jpg");
	//_ShowPNGEx("0:/weather/cloudy1_night.png",0,0);
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
	while(1);
	
}
///****************************************************************************
//* Copyright (C), 2011 ·Ü¶·Ç¶ÈëÊ½¹¤×÷ÊÒ www.ourstm.net
//*
//* ±¾Àý³ÌÔÚ ·Ü¶·°æSTM32¿ª·¢°åV3£¬MINI£¬V2£¬V2.1ÉÏµ÷ÊÔÍ¨¹ý           
//* QQ: 9191274, ÍúÍú£ºsun68, Email: sun68@163.com 
//* ÌÔ±¦µêÆÌ£ºourstm.taobao.com  
//*
//* ÎÄ¼þÃû: main.c
//* ÄÚÈÝ¼òÊö:	
//*       
//*	¿ª·¢°åÓëPCµÄUSBÁ¬½Óºó£¬¿ÉÒÔÍ¨¹ýPC¶ÁÐ´°åÉÏTF¿¨ÉÏµÄÎÄ¼þ £¨UÅÌ¹¦ÄÜ£©£¬Ö§³ÖSDHC¿¨ 
//*	¸¨ÖúÈí¼þ£ºPC»úÉÏÐèÒªÔËÐÐ´®¿Úµ÷ÊÔÖúÊÖÈí¼þ¡£
//    
//	»ùÓÚMDK°æ±¾£º        3.8
//	»ùÓÚ¹Ù·½ÍâÉè¿â°æ±¾£º 3.5
//	»ùÓÚUSB¿â°æ±¾£º      3.3
//*
//* ÎÄ¼þÀúÊ·:
//* °æ±¾ºÅ  ÈÕÆÚ       ×÷Õß    ËµÃ÷
//* v0.3    2011-8-17 sun68  ´´½¨¸ÃÎÄ¼þ
//*
//*/
//  
///* Includes ------------------------------------------------------------------*/
//#define GLOBALS
//#include "stm32f10x.h"
//#include "usb_lib.h"
//#include "usb_pwr.h"
//#include "stdio.h"	   
//#include "demo.h"

//void Usart1_Init(void);
//extern uint16_t MAL_Init (uint8_t lun);
//extern void USB_Disconnect_Config(void);

///****************************************************************************
//* Ãû    ³Æ£ºvoid RCC_Configuration(void)
//* ¹¦    ÄÜ£ºÏµÍ³Ê±ÖÓÅäÖÃÎª72MHZ£¬ ÍâÉèÊ±ÖÓÅäÖÃ
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/ 
//void RCC_Configuration(void){

//  SystemInit();	  
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
//  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
//  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
//}

///****************************************************************************
//* Ãû    ³Æ£ºvoid GPIO_Configuration(void)
//* ¹¦    ÄÜ£ºÍ¨ÓÃIO¿ÚÅäÖÃ
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£º
//****************************************************************************/  
//void GPIO_Configuration(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //×´Ì¬LED1
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //Í¨ÓÃÍÆÍìÊä³öÄ£Ê½
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //Êä³öÄ£Ê½×î´óËÙ¶È50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure); 				  
//}
///****************************************************************************
//* Ãû    ³Æ£ºvoid NVIC_Configuration(void)
//* ¹¦    ÄÜ£ºÖÐ¶ÏÔ´ÅäÖÃ
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/
//void NVIC_Configuration(void)
//{
//  /*  ½á¹¹ÉùÃ÷*/
//  NVIC_InitTypeDef NVIC_InitStructure;

//  /* Configure the NVIC Preemption Priority Bits */  
//  /* Configure one bit for preemption priority */
//  /* ÓÅÏÈ¼¶×é ËµÃ÷ÁËÇÀÕ¼ÓÅÏÈ¼¶ËùÓÃµÄÎ»Êý£¬ºÍ×ÓÓÅÏÈ¼¶ËùÓÃµÄÎ»Êý  */    
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  
//  
//  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USBµÍÓÅÏÈ¼¶ÖÐ¶ÏÇëÇó
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//ÇÀÕ¼ÓÅÏÈ¼¶ 1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//×ÓÓÅÏÈ¼¶Îª1
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB¸ßÓÅÏÈ¼¶ÖÐ¶ÏÇëÇó
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//ÇÀÕ¼ÓÅÏÈ¼¶ 1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//×ÓÓÅÏÈ¼¶Îª0
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_Init(&NVIC_InitStructure);
//}
///****************************************************************************
//* Ãû    ³Æ£ºint main(void)
//* ¹¦    ÄÜ£ºÖ÷º¯Êý
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/
//int main(void)
//{
//  RCC_Configuration(); 		//ÉèÖÃÏµÍ³Ê±ÖÓ 
//  Usart1_Init();			//´®¿Ú³õÊ¼»¯
//  USB_Disconnect_Config();	//ÉèÖÃUSBÁ¬½Ó¿ØÖÆÏß    
//  MAL_Init(0);  	        //´æ´¢½éÖÊ³õÊ¼»¯  
//  GPIO_Configuration();     //×´Ì¬LEDµÄ³õÊ¼»¯   
//  NVIC_Configuration();     //USBÍ¨ÐÅÖÐ¶ÏÅäÖÃ
//  USB_Init();				//USB³õÊ¼»¯
//  while (bDeviceState != CONFIGURED);	 //µÈ´ýUSBÁ¬ÈëHOST
//  Led_ON();								 //LED1 ÁÁ
//  while (1);
//}

///****************************************************************************
//* Ãû    ³Æ£ºvoid Usart1_Init(void)
//* ¹¦    ÄÜ£º´®¿Ú1³õÊ¼»¯º¯Êý
//* Èë¿Ú²ÎÊý£ºÎÞ
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/
//void Usart1_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;
// 
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);	 //Ê¹ÄÜ´®¿Ú1Ê±ÖÓ

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //¸´ÓÃÍÆÍìÊä³ö
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A¶Ë¿Ú 

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //¸´ÓÃ¿ªÂ©ÊäÈë
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A¶Ë¿Ú 

//  USART_InitStructure.USART_BaudRate = 115200;						//ËÙÂÊ115200bps
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//Êý¾ÝÎ»8Î»
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//Í£Ö¹Î»1Î»
//  USART_InitStructure.USART_Parity = USART_Parity_No;				//ÎÞÐ£ÑéÎ»
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //ÎÞÓ²¼þÁ÷¿Ø
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//ÊÕ·¢Ä£Ê½

//  /* Configure USART1 */
//  USART_Init(USART1, &USART_InitStructure);							//ÅäÖÃ´®¿Ú²ÎÊýº¯Êý   
//   /* Enable the USART1 */
//  USART_Cmd(USART1, ENABLE);	
//  
//}

///******************************************************
//		ÕûÐÎÊý¾Ý×ª×Ö·û´®º¯Êý
//        char *itoa(int value, char *string, int radix)
//		radix=10 ±êÊ¾ÊÇ10½øÖÆ	·ÇÊ®½øÖÆ£¬×ª»»½á¹ûÎª0;  

//	    Àý£ºd=-379;
//		Ö´ÐÐ	itoa(d, buf, 10); ºó
//		
//		buf="-379"							   			  
//**********************************************************/
//char *itoa(int value, char *string, int radix)
//{
//    int     i, d;
//    int     flag = 0;
//    char    *ptr = string;

//    /* This implementation only works for decimal numbers. */
//    if (radix != 10)
//    {
//        *ptr = 0;
//        return string;
//    }

//    if (!value)
//    {
//        *ptr++ = 0x30;
//        *ptr = 0;
//        return string;
//    }

//    /* if this is a negative value insert the minus sign. */
//    if (value < 0)
//    {
//        *ptr++ = '-';

//        /* Make the value positive. */
//        value *= -1;
//    }

//    for (i = 10000; i > 0; i /= 10)
//    {
//        d = value / i;

//        if (d || flag)
//        {
//            *ptr++ = (char)(d + 0x30);
//            value -= (d * i);
//            flag = 1;
//        }
//    }

//    /* Null terminate the string. */
//    *ptr = 0;

//    return string;

//} /* NCL_Itoa */
///****************************************************************************
//* Ãû    ³Æ£ºvoid USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
//* ¹¦    ÄÜ£º¸ñÊ½»¯´®¿ÚÊä³öº¯Êý
//* Èë¿Ú²ÎÊý£ºUSARTx:  Ö¸¶¨´®¿Ú
//			Data£º   ·¢ËÍÊý×é
//			...:     ²»¶¨²ÎÊý
//* ³ö¿Ú²ÎÊý£ºÎÞ
//* Ëµ    Ã÷£º¸ñÊ½»¯´®¿ÚÊä³öº¯Êý
//        	"\r"	»Ø³µ·û	   USART_OUT(USART1, "abcdefg\r")   
//			"\n"	»»ÐÐ·û	   USART_OUT(USART1, "abcdefg\r\n")
//			"%s"	×Ö·û´®	   USART_OUT(USART1, "×Ö·û´®ÊÇ£º%s","abcdefg")
//			"%d"	Ê®½øÖÆ	   USART_OUT(USART1, "a=%d",10)
//* µ÷ÓÃ·½·¨£ºÎÞ 
//****************************************************************************/
//void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 

//	const char *s;
//    int d;
//   
//    char buf[16];
//    va_list ap;
//    va_start(ap, Data);

//	while(*Data!=0){				                          //ÅÐ¶ÏÊÇ·ñµ½´ï×Ö·û´®½áÊø·û
//		if(*Data==0x5c){									  //'\'
//			switch (*++Data){
//				case 'r':							          //»Ø³µ·û
//					USART_SendData(USARTx, 0x0d);	   

//					Data++;
//					break;
//				case 'n':							          //»»ÐÐ·û
//					USART_SendData(USARTx, 0x0a);	
//					Data++;
//					break;
//				
//				default:
//					Data++;
//				    break;
//			}
//			
//			 
//		}
//		else if(*Data=='%'){									  //
//			switch (*++Data){				
//				case 's':										  //×Ö·û´®
//                	s = va_arg(ap, const char *);
//                	for ( ; *s; s++) {
//                    	USART_SendData(USARTx,*s);
//						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
//                	}
//					Data++;
//                	break;
//            	case 'd':										  //Ê®½øÖÆ
//                	d = va_arg(ap, int);
//                	itoa(d, buf, 10);
//                	for (s = buf; *s; s++) {
//                    	USART_SendData(USARTx,*s);
//						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
//                	}
//					Data++;
//                	break;
//				default:
//					Data++;
//				    break;
//			}		 
//		}
//		else USART_SendData(USARTx, *Data++);
//		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
//	}
//}

/******************* (C) COPYRIGHT 2011 ·Ü¶·STM32 *****END OF FILE****/

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
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ���ú���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;			     //SDIO�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //�������ȼ�0  ��Χ��0��1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //�����ȼ�0	��Χ��0-7
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ��    �ƣ�void SD_TEST(void)
* ��    �ܣ�SD�����Ժ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
        // �ӵ�ַ0��ʼ��ȡ512�ֽ�  
    	SD_Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  512); 
  	}
}

/****************************************************************************
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
	
    RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��    
    FSMC_LCD_Init();				   //FSMC TFT�ӿڳ�ʼ�
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
	
	disk_initialize(0);						     //fatfs���Թ��������ʵķ����� ���԰�����洢����SST25VF016B��ʾΪ0�����൱�ڴ��̱��
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
//* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
//*
//* �������� �ܶ���STM32������V3��MINI��V2��V2.1�ϵ���ͨ��           
//* QQ: 9191274, ������sun68, Email: sun68@163.com 
//* �Ա����̣�ourstm.taobao.com  
//*
//* �ļ���: main.c
//* ���ݼ���:	
//*       
//*	��������PC��USB���Ӻ󣬿���ͨ��PC��д����TF���ϵ��ļ� ��U�̹��ܣ���֧��SDHC�� 
//*	���������PC������Ҫ���д��ڵ������������
//    
//	����MDK�汾��        3.8
//	���ڹٷ������汾�� 3.5
//	����USB��汾��      3.3
//*
//* �ļ���ʷ:
//* �汾��  ����       ����    ˵��
//* v0.3    2011-8-17 sun68  �������ļ�
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
//* ��    �ƣ�void RCC_Configuration(void)
//* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
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
//* ��    �ƣ�void GPIO_Configuration(void)
//* ��    �ܣ�ͨ��IO������
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷�����
//****************************************************************************/  
//void GPIO_Configuration(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //״̬LED1
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure); 				  
//}
///****************************************************************************
//* ��    �ƣ�void NVIC_Configuration(void)
//* ��    �ܣ��ж�Դ����
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/
//void NVIC_Configuration(void)
//{
//  /*  �ṹ����*/
//  NVIC_InitTypeDef NVIC_InitStructure;

//  /* Configure the NVIC Preemption Priority Bits */  
//  /* Configure one bit for preemption priority */
//  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��  */    
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	  
//  
//  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	    //USB�����ȼ��ж�����
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�Ϊ1
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;			//USB�����ȼ��ж�����
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;			//��ռ���ȼ� 1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�Ϊ0
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_Init(&NVIC_InitStructure);
//}
///****************************************************************************
//* ��    �ƣ�int main(void)
//* ��    �ܣ�������
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/
//int main(void)
//{
//  RCC_Configuration(); 		//����ϵͳʱ�� 
//  Usart1_Init();			//���ڳ�ʼ��
//  USB_Disconnect_Config();	//����USB���ӿ�����    
//  MAL_Init(0);  	        //�洢���ʳ�ʼ��  
//  GPIO_Configuration();     //״̬LED�ĳ�ʼ��   
//  NVIC_Configuration();     //USBͨ���ж�����
//  USB_Init();				//USB��ʼ��
//  while (bDeviceState != CONFIGURED);	 //�ȴ�USB����HOST
//  Led_ON();								 //LED1 ��
//  while (1);
//}

///****************************************************************************
//* ��    �ƣ�void Usart1_Init(void)
//* ��    �ܣ�����1��ʼ������
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/
//void Usart1_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;
// 
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);	 //ʹ�ܴ���1ʱ��

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		 //USART1 TX
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 //�����������
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 //A�˿� 

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 //USART1 RX
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	 //���ÿ�©����
//  GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 //A�˿� 

//  USART_InitStructure.USART_BaudRate = 115200;						//����115200bps
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
//  USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

//  /* Configure USART1 */
//  USART_Init(USART1, &USART_InitStructure);							//���ô��ڲ�������   
//   /* Enable the USART1 */
//  USART_Cmd(USART1, ENABLE);	
//  
//}

///******************************************************
//		��������ת�ַ�������
//        char *itoa(int value, char *string, int radix)
//		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

//	    ����d=-379;
//		ִ��	itoa(d, buf, 10); ��
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
//* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
//* ��    �ܣ���ʽ�������������
//* ��ڲ�����USARTx:  ָ������
//			Data��   ��������
//			...:     ��������
//* ���ڲ�������
//* ˵    ������ʽ�������������
//        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
//			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
//			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
//			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
//* ���÷������� 
//****************************************************************************/
//void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 

//	const char *s;
//    int d;
//   
//    char buf[16];
//    va_list ap;
//    va_start(ap, Data);

//	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
//		if(*Data==0x5c){									  //'\'
//			switch (*++Data){
//				case 'r':							          //�س���
//					USART_SendData(USARTx, 0x0d);	   

//					Data++;
//					break;
//				case 'n':							          //���з�
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
//				case 's':										  //�ַ���
//                	s = va_arg(ap, const char *);
//                	for ( ; *s; s++) {
//                    	USART_SendData(USARTx,*s);
//						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
//                	}
//					Data++;
//                	break;
//            	case 'd':										  //ʮ����
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

/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

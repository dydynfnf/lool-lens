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
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;

//NandFlash状态变量
uint8_t Nand_Status;
 
extern volatile int OS_TimeMS;  

u8 w[4096];
u8 r[4096];
u32 i=0;

int main(void)
{        
	GUI_MEMDEV_Handle hMem0,hMem1;
	SysTick_Config(180000);
	
	USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif            
            &USR_desc,
            &USBD_MSC_cb, 
            &USR_cb);
//	SDRAM_Init();
//	NAND_Init();
//	NAND_Reset();
//	for(i=0;i<2048;i++)
//	{
//		w[i]=i;
//	}
//	STORAGE_Write(1,w,0,2);
//	for(i=0;i<2048;i++)
//	{
//		w[i]=i+1;
//	}
//	STORAGE_Write(1,w,2,2);
//	for(i=0;i<2048;i++)
//	{
//		w[i]=i+2;
//	}
//	STORAGE_Write(1,w,4,2);
//	STORAGE_Read(1,r,0,2);
//	STORAGE_Read(1,r,1,2);
//	STORAGE_Read(1,r,2,2);
	
	while(1);
	
//	SDRAM_Init();
//	
//	LCD_Config();
//	
//	GUI_Init();
//	GUI_SetFont(&GUI_Font32B_ASCII);
//	GUI_SetBkColor(GUI_BLUE);
//	GUI_Clear();

////	GUI_DispStringAt("blacklight",0,100);
//	
////	GUIDEMO_Main();
//	
//	GUI_SetColor(GUI_BLUE);
//	GUI_FillRect(0, 0, 480, 272);
//	GUI_SetColor(GUI_GREEN);
//	GUI_FillRect(0, 273, 480, 544);
//	GUI_SetOrg(0, 0); /* Set origin to screen 1 */
//	GUI_Delay(1000);
//	GUI_SetOrg(0, 272); /* Set origin to screen 2 */
//	GUI_SetColor(GUI_YELLOW);
//	
//	hMem0 = GUI_MEMDEV_CreateFixed(0,0,100,50,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_888);
//	hMem1 = GUI_MEMDEV_CreateFixed(0,0,100,50,GUI_MEMDEV_NOTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_888);
//	GUI_MEMDEV_Select(hMem0);
//	GUI_Clear();
//	GUI_DispString("Text");
//	GUI_MEMDEV_CopyToLCDAt(hMem0,0,0);
//	
//	while(1)
//	{	
//		GUI_MEMDEV_Select(hMem1);
//		GUI_MEMDEV_DrawPerspectiveX(hMem0,0,0,50,50-50*i/100,100-100*i/100,25*i/100);
//		GUI_MEMDEV_CopyToLCDAt(hMem1,i,50+i);
//		
//		GUI_Delay(1);
//		
//		GUI_MEMDEV_Select(hMem1);
//		GUI_Clear();
//		GUI_MEMDEV_CopyToLCDAt(hMem1,i,50+i);
//		
//		i+=1;
//		if(i>100)
//		{
//			i=0;
//		}
//		GUI_SetOrg(0, i);
//	}		  
		
}

////读数组
//uint8_t RBuffer[2048];
////写数组
//uint8_t WBuffer[2048];
//uint16_t BadBlock[60];
//u32 WSDRAM[5];
//u32 RSDRAM[5];
//void Led_Init(void);

//int main(void)
//{
//	NAND_IDTypeDef qwe;
//	NAND_ADDRESS WriteReadAddr;
//	u16 i; 
//	SysTick_Config(180000);
//	Led_Init();
//	NAND_Init();
//	NAND_Reset();
//	
////	for(i=0;i<5;i++)
////	WSDRAM[i]=i+3;
////	SDRAM_WriteBuffer(WSDRAM,3,5);
////	SDRAM_ReadBuffer(RSDRAM,3,5);
//	for(i=0;i<250;i++);
//	NAND_ReadID( &qwe );
//	SDRAM_Init();
//	for(i=0;i<5;i++)
//	WSDRAM[i]=i+3;
//	SDRAM_WriteBuffer(WSDRAM,3,5);
//	SDRAM_ReadBuffer(RSDRAM,3,5);
//	WriteReadAddr .Page =0;
//	WriteReadAddr .Block =0;
//	WriteReadAddr .Zone=0;
//	Nand_Status =NAND_EraseBlock(WriteReadAddr);	
//	if(Nand_Status==NAND_READY )
//	GPIO_ResetBits(GPIOD, GPIO_Pin_13);	
//	Nand_Status =NAND_ReadPage(RBuffer, WriteReadAddr,1);
//	for(i=0;i<2048;i++)
//	{
//		if(i<256)
//		WBuffer[i]=i;
//    else
//    WBuffer[i]=66;			
//	}
//	NAND_WritePage(WBuffer, WriteReadAddr,1);
//	Nand_Status =NAND_ReadPage(RBuffer, WriteReadAddr,1);
//	BadBlock_Check(WBuffer,RBuffer,BadBlock);
//	GPIO_SetBits(GPIOD, GPIO_Pin_13);
//	while(1);
//}

//void Led_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); /*使能LED灯使用的GPIO时钟*/
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
//	GPIO_Init(GPIOD , &GPIO_InitStructure); 
//	GPIO_SetBits(GPIOD, GPIO_Pin_13);
//}





#include "includes.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "fsmc_sram.h"
#include "GUI.h"
#include "lcd_dis24.h"
#include "GUIDEMO.h"
				   

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOA| 
	RCC_APB2Periph_GPIOB| RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
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
unsigned char i=0x3f;
unsigned int pastms=0;
unsigned char temp;

extern GUI_CONST_STORAGE GUI_BITMAP bmbatter;
extern GUI_CONST_STORAGE GUI_BITMAP bmfavorite;
extern GUI_CONST_STORAGE GUI_BITMAP bmichat;
extern GUI_CONST_STORAGE GUI_BITMAP bmmail;
extern GUI_CONST_STORAGE GUI_BITMAP bmtransfer;

int main(void)
{
    RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能    
    FSMC_LCD_Init();				   //FSMC TFT接口初始化 
	SysTick_Config(72000);
	LCD_init();
	
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_Clear();

//	GUI_DispStringAt("blacklight",0,0);
	
	//GUIDEMO_Main();
	
	//GUI_SetOrg(0,100);
	GUI_DrawBitmap(&bmbatter, 50, 60);
	GUI_DrawBitmap(&bmfavorite, 180, 5);
	GUI_DrawBitmap(&bmichat, 210, 5);
	GUI_DrawBitmap(&bmmail, 240, 0);
	GUI_DrawBitmap(&bmtransfer, 270, 5);
	while(1)
	{
		if(OS_TimeMS-pastms>=100)
		{
//			LCD_WR_REG(0x00BE); //set PWM for B/L
//			LCD_WR_Data(0x0001);
//			LCD_WR_Data(i);
//			LCD_WR_Data(0x0001);
//			LCD_WR_Data(0x00ff);
//			LCD_WR_Data(0x0000);
//			LCD_WR_Data(0x0000);
			i--;
			pastms = OS_TimeMS;
		}
		if(i==0)
		{
			i=0x3f;
		}

//		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
//		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//		SPI_I2S_SendData(SPI1, 03);
//		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//		temp = SPI_I2S_ReceiveData(SPI1);
//		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//		SPI_I2S_SendData(SPI1, i);
//		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//		temp = SPI_I2S_ReceiveData(SPI1);		
//		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		
		//GUI_DispDecAt(i,0,24,4);
	}		
}

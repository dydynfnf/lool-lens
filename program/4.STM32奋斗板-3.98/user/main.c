#define GLOBALS 
#include "includes.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "fsmc_sram.h"
#include "GUI.h"
				   

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
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
u16 i=0;
extern const GUI_BITMAP bmbmptest;
int main(void)
{
    RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能    
    FSMC_LCD_Init();				   //FSMC TFT接口初始化 

    GUI_Init();
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_DispStringAt("ABCD",10,10);
	
	GUI_DrawBitmap(&bmbmptest,0,0);

    while(1);	  
}


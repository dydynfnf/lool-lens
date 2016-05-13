#define GLOBALS 
#include "includes.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "fsmc_sram.h"
#include "GUI.h"
				   

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
u16 i=0;
extern const GUI_BITMAP bmbmptest;
int main(void)
{
    RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��    
    FSMC_LCD_Init();				   //FSMC TFT�ӿڳ�ʼ�� 

    GUI_Init();
	GUI_SetBkColor(GUI_RED);
	GUI_Clear();

	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_DispStringAt("ABCD",10,10);
	
	GUI_DrawBitmap(&bmbmptest,0,0);

    while(1);	  
}


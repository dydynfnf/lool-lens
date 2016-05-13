#include "includes.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "fsmc_sram.h"
#include "GUI.h"
#include "lcd_dis24.h"
#include "GUIDEMO.h"
				   

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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
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
extern volatile int OS_TimeMS;
unsigned char i=0xff;
unsigned int pastms=0;
int main(void)
{
    RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��    
    FSMC_LCD_Init();				   //FSMC TFT�ӿڳ�ʼ�� 
	SysTick_Config(72000);
	LCD_init();
	
	GUI_Init();
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();

	GUI_DispStringAt("blacklight",0,0);
	
	//GUIDEMO_Main();
	
	//GUI_SetOrg(0,100);
	while(1)
	{
		if(OS_TimeMS-pastms>=10)
		{
			LCD_WR_REG(0x00BE); //set PWM for B/L
			LCD_WR_Data(0x0001);
			LCD_WR_Data(i--);
			LCD_WR_Data(0x0001);
			LCD_WR_Data(0x00f0);
			LCD_WR_Data(0x0000);
			LCD_WR_Data(0x0000);
			
			pastms = OS_TimeMS;
		}
		GUI_DispDecAt(i,0,24,4);
	}		
}


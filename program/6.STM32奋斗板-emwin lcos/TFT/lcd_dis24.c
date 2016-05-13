/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: Fun.c
* ���ݼ���:
*       �������ṩ�˷ܶ�����5����ģ�����������
		
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-07-18 sun68  �������ļ�
*
*/

#include "fsmc_sram.h"  
#include "lcd_dis24.h" 

//=================================================================================

//5��������ʾ����



unsigned int  HDP=900/3-1;  //HS_Display_Area
unsigned int  HT =1728/3-1;  //(HS_Display_Area + HS_pulse_width + HS_Blanking + HS_Front_Porch)
unsigned int  HPS=264/3;  //(HS_pulse_width + HS_Blanking)
unsigned char HPW=126/3-1;  //HS_pulse_width
unsigned int  LPS=0;  

 

unsigned int  VDP=224-1;  //VS_Display_Area
unsigned int  VT =312-1;  //(VS_Display_Area + VS_pulse_width + VS_Blanking + VS_Front_Porch)
unsigned int  VPS=69;  //(VS_pulse_width + VS_Blanking)
unsigned char VPW=3-1;   //VS_pulse_width
unsigned int  FPS=0;

/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nCount)
* ��    �ܣ���ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/****************************************************************************
* ��    �ƣ�LCD_WR_REG(unsigned int index)
* ��    �ܣ�FSMCд��ʾ���Ĵ�����ַ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_REG(u16 index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}

/****************************************************************************
* ��    �ƣ�void LCD_WR_CMD(unsigned int index,unsigned int val)
* ��    �ܣ�FSMCд��ʾ���Ĵ������ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_CMD(u16 index,u16 val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

/****************************************************************************
* ��    �ƣ�unsigned short LCD_RD_data(void)
* ��    �ܣ�FSMC����ʾ��16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
u16 LCD_RD_Data(void){
	u16 a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}

/****************************************************************************
* ��    �ƣ�LCD_WR_Data(unsigned int val)
* ��    �ܣ�FSMCд16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void    LCD_WR_Data(u16 val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

 void LCD_WR_Data_8(u16 val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

/****************************************************************************
* ��    �ƣ�SPI_Configuration(void)
* ��    ��: SPI����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 

SPI_InitTypeDef   SPI_InitStructure;
void SPI_Configuration(void)
{
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}

/****************************************************************************
* ��    �ƣ�lcos_reg_init(void)
* ��    ��: lcos����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 

u8 SPI1_Buffer_Tx[9][2] =
{
0x01,0x10,
0x03,0x3F,
0x07,0x0A,
0x08,0x00,
0x09,0x01,
0x0A,0x00,
0x0B,0x01,
0x0C,0x00,
0x06,0x28,
};
u8 SPI1_Buffer_Rx[9][2]={0};
u8 Tx_Idx = 0;
u8 send_count=0;
void lcos_reg_init(void)
{
	SPI_Configuration();
	for(send_count=0; send_count<9; send_count++)
	{
		Tx_Idx = 0;
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		while(Tx_Idx < 2)
		{ 
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[send_count][Tx_Idx]);
			
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			SPI1_Buffer_Rx[send_count][Tx_Idx++] = SPI_I2S_ReceiveData(SPI1);		 
		}
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
	}
}

/****************************************************************************
LCD��ʼ������
****************************************************************************/ 

void LCD_init(void)
{
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    Delay(0xAFFFF);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );
	Delay(0xAFFFF);

	LCD_WR_REG(0x00E2);	        //PLL ��Ƶ��120M   PLL=10*36/3=120M
	LCD_WR_Data(0x001D);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);
	
	LCD_WR_REG(0x00E0);         
	LCD_WR_Data(0x0001);
	Delay(0xAFFF);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);		// PLL ʹ��  PLL��Ϊʱ��
	Delay(0xAFFF);
	LCD_WR_REG(0x0001);         // �����λ
	Delay(0xAFFF);

	LCD_WR_REG(0x00B0);	        //����LCDģʽ
	LCD_WR_Data(0x0024);
	LCD_WR_Data(0x0040);
	LCD_WR_Data((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_Data(HDP&0X00FF);
    LCD_WR_Data((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_Data(VDP&0X00FF);
    LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_Data((HT>>8)&0X00FF);  //Set HT
	LCD_WR_Data(HT&0X00FF);
	LCD_WR_Data((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(HPS&0X00FF);
	LCD_WR_Data(HPW);			   //Set HPW
	LCD_WR_Data((LPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(LPS&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_Data((VT>>8)&0X00FF);   //Set VT
	LCD_WR_Data(VT&0X00FF);
	LCD_WR_Data((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_Data(VPS&0X00FF);
	LCD_WR_Data(VPW);			   //Set VPW
	LCD_WR_Data((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_Data(FPS&0X00FF);
	
	LCD_WR_REG(0xE6);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x14);
	LCD_WR_Data(0x79);

	LCD_WR_REG(0x00B8);
	LCD_WR_Data(0x000F);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_Data(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x00BA);
	LCD_WR_Data(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x0036);            //rotation
	LCD_WR_Data(0x0000);


	LCD_WR_REG(0x00F0);             //16λ  565ģʽ
	LCD_WR_Data(0x0003);


	Delay(0xAFFF);

	//LCD_clear();
	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x000ff);
	
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00ff);
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);

//	LCD_WR_REG(0x00E2);	        //PLL ��Ƶ��120M   PLL=10*36/3=120M
//	LCD_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
//	LCD_WR_Data(0x0002);
//	LCD_WR_Data(0x0004);
//	
//	LCD_WR_REG(0x00E0);         
//	LCD_WR_Data(0x0001);
//	Delay(0xAFFF);
//	LCD_WR_REG(0x00E0);
//	LCD_WR_Data(0x0003);		// PLL ʹ��  PLL��Ϊʱ��
//	Delay(0xAFFF);
//	LCD_WR_REG(0x0001);         // �����λ
//	Delay(0xAFFF);

//	LCD_WR_REG(0x00B0);	        //����LCDģʽ
//	LCD_WR_Data(0x0020);
//	LCD_WR_Data(0x0000);
//	LCD_WR_Data((HDP>>8)&0X00FF);  //Set HDP
//	LCD_WR_Data(HDP&0X00FF);
//    LCD_WR_Data((VDP>>8)&0X00FF);  //Set VDP
//	LCD_WR_Data(VDP&0X00FF);
//    LCD_WR_Data(0x0000);

//	LCD_WR_REG(0x00B4);	//HSYNC
//	LCD_WR_Data((HT>>8)&0X00FF);  //Set HT
//	LCD_WR_Data(HT&0X00FF);
//	LCD_WR_Data((HPS>>8)&0X00FF);  //Set HPS
//	LCD_WR_Data(HPS&0X00FF);
//	LCD_WR_Data(HPW);			   //Set HPW
//	LCD_WR_Data((LPS>>8)&0X00FF);  //Set HPS
//	LCD_WR_Data(LPS&0X00FF);
//	LCD_WR_Data(0x0000);

//	LCD_WR_REG(0x00B6);	//VSYNC
//	LCD_WR_Data((VT>>8)&0X00FF);   //Set VT
//	LCD_WR_Data(VT&0X00FF);
//	LCD_WR_Data((VPS>>8)&0X00FF);  //Set VPS
//	LCD_WR_Data(VPS&0X00FF);
//	LCD_WR_Data(VPW);			   //Set VPW
//	LCD_WR_Data((FPS>>8)&0X00FF);  //Set FPS
//	LCD_WR_Data(FPS&0X00FF);
//	
//	LCD_WR_REG(0xE6);
//	LCD_WR_Data(0x03);
//	LCD_WR_Data(0x99);
//	LCD_WR_Data(0x94);

//	LCD_WR_REG(0x00B8);
//	LCD_WR_Data(0x000F);    //GPIO3=input, GPIO[2:0]=output
//	LCD_WR_Data(0x0001);    //GPIO0 normal

//	LCD_WR_REG(0x00BA);
//	LCD_WR_Data(0x000F);    //GPIO[3:0] out 1

//	LCD_WR_REG(0x0036);            //rotation
//	LCD_WR_Data(0x0000);


//	LCD_WR_REG(0x00F0);             //16λ  565ģʽ
//	LCD_WR_Data(0x0003);


//	Delay(0xAFFF);

//	//LCD_clear();
//	LCD_WR_REG(0x0029); //display on

//	LCD_WR_REG(0x00BE); //set PWM for B/L
//	LCD_WR_Data(0x0001);
//	LCD_WR_Data(0x000ff);
//	
//	LCD_WR_Data(0x0001);
//	LCD_WR_Data(0x00ff);
//	LCD_WR_Data(0x0000);
//	LCD_WR_Data(0x0000);
	
	Delay(0xFFFFF);Delay(0xFFFFF);Delay(0xFFFFF);Delay(0xFFFFF);Delay(0xFFFFF);Delay(0xFFFFF);
	lcos_reg_init();
}

/****************************************************************************
��������
****************************************************************************/ 
void TFT_CLEAR(u16 x,u16 y,u32 len,u32 wid)
{                    
    u32 w;

	LCD_WR_REG(0x002A);	
	LCD_WR_Data(x>>8);	    
	LCD_WR_Data(x);
	LCD_WR_Data((len-1)>>8);	    
	LCD_WR_Data((len-1));
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    
	LCD_WR_Data(y);
	LCD_WR_Data((wid-1)>>8);	    
	LCD_WR_Data((wid-1));
	LCD_WR_REG(0x002c);
	w=(len-x)*(wid-y);
	while(w--) LCD_WR_Data(0xffff);
}   

/***************************************************************************
���㺯��
****************************************************************************/ 
void TFT_DrawPoint(u16 x,u16 y, u16 color)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//������ַX
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//������ַY
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	LCD_WR_Data(color); 	   	
}

/***************************************************************************
���㺯��
****************************************************************************/ 
u16 TFT_ReadPoint(u16 x,u16 y)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//������ַX
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//������ַY
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2e);
	return(LCD_RD_Data()); 	   	
}

/***************************************************************************
��ˮƽ�ߺ���
****************************************************************************/ 
void TFT_HLine(u16 x0,u16 y,u16 x1,u16 color)
{	
	u32 i,point_count;							       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x0>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x0);	    			
	LCD_WR_Data(x1>>8);	    			//������ַX
	LCD_WR_Data(x1);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//������ַY
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	point_count=x1-x0+1;
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	} 
}

/***************************************************************************
����ֱ�ߺ���
****************************************************************************/ 
void TFT_VLine(u16 x,u16 y0,u16 y1,u16 color)
{	
	u32 i,point_count;							       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//������ַX
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y0>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y0);	    			    
	LCD_WR_Data(y1>>8);	    			//������ַY
	LCD_WR_Data(y1);	    			
	LCD_WR_REG(0x2c);
	point_count=y1-y0+1;
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	} 
}

/***************************************************************************
������亯�� 
****************************************************************************/ 
void TFT_Fill(u16 x0,u16 y0, u16 x1, u16 y1, u16 color)
{	
	u32 i,point_count;						       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x0>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x0);	    			
	LCD_WR_Data(x1>>8);	    			//������ַX
	LCD_WR_Data(x1);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y0>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y0);	    			    
	LCD_WR_Data(y1>>8);	    			//������ַY
	LCD_WR_Data(y1);	    			
	LCD_WR_REG(0x2c);
	point_count=(x1-x0+1)*(y1-y0+1);
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	}	
}

/****************************************************************************
* ��    �ƣ�void TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
* ��    �ܣ���ָ��λ����ʾһ��ͼƬ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_ShowBmp(u16 x,u16 y,u16 lenth,u16 wide,const u8 *p)
{      
    u32 size,temp; 
    //�趨һ��ͼƬ��ռ�Ĵ��ڴ�С 
	
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x&0x00ff);
	LCD_WR_Data(((u16)x+lenth-1)>>8);	    		//������ַX
	LCD_WR_Data(((u16)x+lenth-1)&0x00ff);
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y&0x00ff);
	LCD_WR_Data((y+wide-1)>>8);	    	 	//������ַY
	LCD_WR_Data((y+wide-1)&0x00ff);
	LCD_WR_REG(0x2c);	 
	temp=(u32)lenth*wide*2;
	for(size=0;size<temp;size++)LCD_WR_Data_8(p[size]); 
}         

/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

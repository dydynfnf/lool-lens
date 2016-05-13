/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: Fun.c
* 内容简述:
*       本例程提供了奋斗板配5寸屏模块的驱动程序
		
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-07-18 sun68  创建该文件
*
*/

#include "fsmc_sram.h"  
#include "lcd_dis24.h" 

//=================================================================================

//5寸屏的显示设置



unsigned int  HDP=799;  //HS_Display_Area
unsigned int  HT =1200;  //(HS_Display_Area + HS_pulse_width + HS_Blanking + HS_Front_Porch)
unsigned int  HPS=93;  //(HS_pulse_width + HS_Blanking)
unsigned char HPW=40;  //HS_pulse_width
unsigned int  LPS=46;  

 

unsigned int  VDP=479;  //VS_Display_Area
unsigned int  VT =670;  //(VS_Display_Area + VS_pulse_width + VS_Blanking + VS_Front_Porch)
unsigned int  VPS=43;  //(VS_pulse_width + VS_Blanking)
unsigned char VPW=20;   //VS_pulse_width
unsigned int  FPS=23;

/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/****************************************************************************
* 名    称：LCD_WR_REG(unsigned int index)
* 功    能：FSMC写显示器寄存器地址函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_REG(u16 index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}

/****************************************************************************
* 名    称：void LCD_WR_CMD(unsigned int index,unsigned int val)
* 功    能：FSMC写显示器寄存器数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_CMD(u16 index,u16 val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

/****************************************************************************
* 名    称：unsigned short LCD_RD_data(void)
* 功    能：FSMC读显示区16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
u16 LCD_RD_Data(void){
	u16 a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}

/****************************************************************************
* 名    称：LCD_WR_Data(unsigned int val)
* 功    能：FSMC写16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
LCD初始化函数
****************************************************************************/ 
void LCD_init(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    Delay(0xAFFFF);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFF);

    LCD_WR_REG(0x00E2);	        //PLL 倍频到120M   PLL=10*36/3=120M
	LCD_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);
	
	LCD_WR_REG(0x00E0);         
	LCD_WR_Data(0x0001);
	Delay(0xAFFF);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);		// PLL 使能  PLL作为时钟
	Delay(0xAFFF);
	LCD_WR_REG(0x0001);         // 软件复位
	Delay(0xAFFF);
	LCD_WR_REG(0xE6);
	LCD_WR_Data(0x04);
	LCD_WR_Data(0x93);
	LCD_WR_Data(0xE0);

	LCD_WR_REG(0x00B0);	        //设置LCD模式
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);
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

	LCD_WR_REG(0x00B8);
	LCD_WR_Data(0x000F);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_Data(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x00BA);
	LCD_WR_Data(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x0036);            //rotation
	LCD_WR_Data(0x0000);


	LCD_WR_REG(0x00F0);             //16位  565模式
	LCD_WR_Data(0x0003);


	Delay(0xAFFF);

	//LCD_clear();
	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x000ff);
	
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);

//	LCD_WR_REG(0x00d0);//设置动态背光控制配置 
//	LCD_WR_Data(0x000d);

   			

}

/****************************************************************************
清屏函数
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
画点函数
****************************************************************************/ 
void TFT_DrawPoint(u16 x,u16 y, u16 color)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//结束地址X
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//结束地址Y
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	LCD_WR_Data(color); 	   	
}

/***************************************************************************
读点函数
****************************************************************************/ 
u16 TFT_ReadPoint(u16 x,u16 y)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//结束地址X
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//结束地址Y
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2e);
	return(LCD_RD_Data()); 	   	
}

/***************************************************************************
画水平线函数
****************************************************************************/ 
void TFT_HLine(u16 x0,u16 y,u16 x1,u16 color)
{	
	u32 i,point_count;							       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x0>>8);	    			//起始地址X
	LCD_WR_Data(x0);	    			
	LCD_WR_Data(x1>>8);	    			//结束地址X
	LCD_WR_Data(x1);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//结束地址Y
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	point_count=x1-x0+1;
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	} 
}

/***************************************************************************
画垂直线函数
****************************************************************************/ 
void TFT_VLine(u16 x,u16 y0,u16 y1,u16 color)
{	
	u32 i,point_count;							       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//结束地址X
	LCD_WR_Data(x);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y0>>8);	    			//起始地址Y
	LCD_WR_Data(y0);	    			    
	LCD_WR_Data(y1>>8);	    			//结束地址Y
	LCD_WR_Data(y1);	    			
	LCD_WR_REG(0x2c);
	point_count=y1-y0+1;
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	} 
}

/***************************************************************************
矩形填充函数 
****************************************************************************/ 
void TFT_Fill(u16 x0,u16 y0, u16 x1, u16 y1, u16 color)
{	
	u32 i,point_count;						       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x0>>8);	    			//起始地址X
	LCD_WR_Data(x0);	    			
	LCD_WR_Data(x1>>8);	    			//结束地址X
	LCD_WR_Data(x1);	    			    
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y0>>8);	    			//起始地址Y
	LCD_WR_Data(y0);	    			    
	LCD_WR_Data(y1>>8);	    			//结束地址Y
	LCD_WR_Data(y1);	    			
	LCD_WR_REG(0x2c);
	point_count=(x1-x0+1)*(y1-y0+1);
	for(i=0;i<=point_count;i++)
	{
		LCD_WR_Data(color); 	 
	}	
}

/****************************************************************************
* 名    称：void TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
* 功    能：在指定位置显示一副图片
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_ShowBmp(u16 x,u16 y,u16 lenth,u16 wide,const u8 *p)
{      
    u32 size,temp; 
    //设定一个图片所占的窗口大小 
	
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x&0x00ff);
	LCD_WR_Data(((u16)x+lenth-1)>>8);	    		//结束地址X
	LCD_WR_Data(((u16)x+lenth-1)&0x00ff);
    LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y&0x00ff);
	LCD_WR_Data((y+wide-1)>>8);	    	 	//结束地址Y
	LCD_WR_Data((y+wide-1)&0x00ff);
	LCD_WR_REG(0x2c);	 
	temp=(u32)lenth*wide*2;
	for(size=0;size<temp;size++)LCD_WR_Data_8(p[size]); 
}         

/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

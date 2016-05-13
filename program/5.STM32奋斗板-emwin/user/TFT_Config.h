#ifndef	__TFT_CONFIG_h__
#define	__TFT_CONFIG_h__
#define TFT_X_MAX			800		//屏幕的X轴最大像素点
#define TFT_Y_MAX			480	    //屏幕的Y轴最大像素点
			
#define TFT_XY_Switch		0		//显示长宽互换
#define TFT_X_Rev			0		//X轴扫描反转
#define TFT_Y_Rev			0		//Y轴扫描反转

#if TFT_XY_Switch == 0
	#define Dis_X_MAX		TFT_X_MAX
	#define Dis_Y_MAX		TFT_Y_MAX
#endif

#if TFT_XY_Switch == 1
	#define Dis_X_MAX		TFT_Y_MAX
	#define Dis_Y_MAX		TFT_X_MAX
#endif

#define R 0xf800
#define G 0x07e0
#define B 0x001e

#define		TFT_Base_Addr		0x60000000	//TFT基地址


#define	TFT_Reg_Ptr			((vu16 *)TFT_Base_Addr)
#define	TFT_DAT_Ptr			((vu16 *)(TFT_Base_Addr+(0x1<<17)))

#define TFT_RES_Enable	  GPIO_ResetBits(GPIOE, GPIO_Pin_1); //使能tft屏幕复位引脚
#define TFT_RES_Disable	  GPIO_SetBits(GPIOE, GPIO_Pin_1);

#endif


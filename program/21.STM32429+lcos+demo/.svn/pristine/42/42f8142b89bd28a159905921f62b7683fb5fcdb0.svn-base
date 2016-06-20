#include "lcos_300_224.h"

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

void LCOS_SPI_Configuration(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); //PB5复用为 SPI1
	
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

void LCOS_regs_init(void)
{
	LCOS_SPI_Configuration();
	for(send_count=0; send_count<9; send_count++)
	{
		Tx_Idx = 0;
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		while(Tx_Idx < 2)
		{ 
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(SPI1, SPI1_Buffer_Tx[send_count][Tx_Idx]);
			
			while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
			SPI1_Buffer_Rx[send_count][Tx_Idx++] = SPI_I2S_ReceiveData(SPI1);		 
		}
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
}

void power_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}

void LCOS_Init(void)
{
	LCD__Init();
  LCD_LayerInit();
  LCD_DisplayOn();
	delay_ms(20);
	LCOS_regs_init();
	DMA2D_Config();
}

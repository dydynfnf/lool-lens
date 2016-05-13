#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif

#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"


#define Led_ON()   GPIO_SetBits(GPIOB, GPIO_Pin_5);  	      //LED1��
#define Led_OFF()  GPIO_ResetBits(GPIOB, GPIO_Pin_5); 	      //LED2��

EXT uint32_t Mass_Memory_Size[2];
EXT uint32_t Mass_Block_Size[2];
EXT uint32_t Mass_Block_Count[2];




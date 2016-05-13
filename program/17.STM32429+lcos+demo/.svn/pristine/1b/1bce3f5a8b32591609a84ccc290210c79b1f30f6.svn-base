#include "stm32f429_lcd.h"

extern u32 FrameBuffer0[224*3][900];//allocation in SDRAM.c
extern u32 FrameBuffer1[224*3][900];//allocation in SDRAM.c
extern u16 BlendedBuffer[224][900];//allocation in SDRAM.c

static uint32_t HSYNC = 126;
static uint32_t HBP = 138;
static uint32_t WIDTH  = 900;
static uint32_t HFP = 564;

static uint32_t VSYNC = 3;
static uint32_t VBP = 66;
static uint32_t HEIGHT = 224;
static uint32_t VFP = 19;


void LCD_AF_GPIOConfig(void);

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void LCD__Init(void)
{ 
  
  LTDC_InitTypeDef       LTDC_InitStruct;
  LTDC_Layer_TypeDef     LTDC_Layerx;
  
  /* Enable the LTDC Clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
    
  /* Configure the LCD Control pins */
  LCD_AF_GPIOConfig();  
	
	/* Sets LCD backlight and display On -----------------------------------------*/
//  LCD_BL_ON();	
  
  /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
//  SDRAM_Init();
  
  /* Check if the LCD is AMPIRE480272 or AMPIRE640480 */
//  LCD_CheckDevice();
  
  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
  /* Initialize the vertical synchronization polarity as active low */  
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
  /* Initialize the data enable polarity as active high */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
  /* Initialize the pixel clock polarity as input pixel clock */ 
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;
  
  /* Configure R,G,B component values for LCD background color */                   
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;  
  

	/* Initialize the LCD pixel width and pixel height */
	/* Configure PLLSAI prescalers for LCD */
	/* Enable Pixel Clock */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/8 = 8 Mhz */
	RCC_PLLSAIConfig(27*8, 7, 2);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);
	
	/* Enable PLLSAI Clock */
	RCC_PLLSAICmd(ENABLE);
	/* Wait for PLLSAI activation */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
	{
	}
	
	/* Timing configuration */  
	/* Configure horizontal synchronization width */     
	LTDC_InitStruct.LTDC_HorizontalSync = HSYNC-1;
	/* Configure vertical synchronization height */
	LTDC_InitStruct.LTDC_VerticalSync = VSYNC-1;
	/* Configure accumulated horizontal back porch */
	LTDC_InitStruct.LTDC_AccumulatedHBP = HSYNC+HBP-1; 
	/* Configure accumulated vertical back porch */
	LTDC_InitStruct.LTDC_AccumulatedVBP = VSYNC+VBP-1;  
	/* Configure accumulated active width */  
	LTDC_InitStruct.LTDC_AccumulatedActiveW = HSYNC+HBP+WIDTH-1;
	/* Configure accumulated active height */
	LTDC_InitStruct.LTDC_AccumulatedActiveH = VSYNC+VBP+HEIGHT-1;
	/* Configure total width */
	LTDC_InitStruct.LTDC_TotalWidth = HSYNC+HBP+WIDTH+HFP-1; 
	/* Configure total height */
	LTDC_InitStruct.LTDC_TotalHeigh = VSYNC+VBP+HEIGHT+VFP-1;
	
	LTDC_Init(&LTDC_InitStruct); 
}

/**
  * @brief  Initializes the LCD layers.
  * @param  None
  * @retval None
  */
void LCD_LayerInit(void)
{ 

  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;
  
	/* LCD AMPIRE480272 is used */
	/* Windowing configuration */ 
	/* Horizontal start = horizontal synchronization + Horizontal back porch = 43 
	Vertical start   = vertical synchronization + vertical back porch     = 12
	Horizontal stop = Horizontal start + WIDTH -1 
	Vertical stop   = Vertical start + HEIGHT -1        
	*/      
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = HSYNC+HBP;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = (HSYNC+HBP+WIDTH- 1); 
	LTDC_Layer_InitStruct.LTDC_VerticalStart = VSYNC+VBP;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = (VSYNC+VBP+HEIGHT- 1);
	
	/* Pixel Format configuration*/            
	LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
	/* Alpha constant (255 totally opaque) */
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
	/* Default Color configuration (configure A,R,G,B component values) */          
	LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;        
	LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
	LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
	LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
	/* Configure blending factors */       
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
	/* the length of one line of pixels in bytes + 3 then :
	Line Lenth = Active high width x number of bytes per pixel + 3 
	Active high width         = WIDTH 
	number of bytes per pixel = 2    (pixel_format : RGB565) 
	*/
	LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((WIDTH * 2) + 3);
	/*  the pitch is the increment from the start of one line of pixels to the 
	start of the next line in bytes, then :
	Pitch = Active high width x number of bytes per pixel     
	*/ 
	LTDC_Layer_InitStruct.LTDC_CFBPitch = (WIDTH * 2);
	/* configure the number of lines */  
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber = HEIGHT;
	
	/* Input Address configuration */    
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)BlendedBuffer;
	
	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);
	
	/* Configure Layer2 */
//	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = (uint32_t)&FrameBuffer1;
//	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
//	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
//	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
//  
//  LTDC_ReloadConfig(LTDC_IMReload);
  
  /* Enable foreground & background Layers */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE);
//  LTDC_LayerCmd(LTDC_Layer2, ENABLE);
  LTDC_ReloadConfig(LTDC_IMReload);
  
//  LCD_SetFont(&LCD_DEFAULT_FONT);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
  /* Display On */
  LTDC_Cmd(ENABLE); /* display ON */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{
  /* Display Off */
  LTDC_Cmd(DISABLE); 
}

/**
  * @brief DMA2D configuration.
  * @note  This function Configure tha DMA2D peripheral :
  *        1) Configure the Transfer mode as memory to memory with blending.
  *        2) Configure the output color mode as RGB565 pixel format.
  *        3) Configure the foreground
  *          - first image loaded from FLASH memory
  *          - constant alpha value (decreased to see the background)
  *          - color mode as RGB565 pixel format
  *        4) Configure the background
  *          - second image loaded from FLASH memory
  *          - color mode as RGB565 pixel format
  * @retval
  *  None
  */

void DMA2D_Config(void)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  DMA2D_FG_InitTypeDef   DMA2D_FG_InitStruct;  
  DMA2D_BG_InitTypeDef   DMA2D_BG_InitStruct;

  /* Enable the DMA2D Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); 

  /* DMA2D DeInitialization */
  DMA2D_DeInit();
  
  /* Configure transfer mode : memory to memory with blending */
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M_BLEND; 
  
  /* Configure color mode : RGB565 pixel format */
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;  
   
  /* Configure output Address : output frame buffer is located at SRAM */                
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = (uint32_t)BlendedBuffer;
  
  /* Configure the alpha and RGB values */
  DMA2D_InitStruct.DMA2D_OutputGreen = 0;
  DMA2D_InitStruct.DMA2D_OutputBlue = 0;
  DMA2D_InitStruct.DMA2D_OutputRed = 0;
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0;

  /* Configure the output offset */
  DMA2D_InitStruct.DMA2D_OutputOffset = 0;  
             
  /* Configure number of lines : height */
  DMA2D_InitStruct.DMA2D_NumberOfLine = 224; 
  
  /* Configure number of pixel per line : width */
  DMA2D_InitStruct.DMA2D_PixelPerLine = 900;
  
  /* DMA2D Initialization */
  DMA2D_Init(&DMA2D_InitStruct);

  /* Configure default values for foreground */
  DMA2D_FG_StructInit(&DMA2D_FG_InitStruct);
  
  /* Configure foreground memory address : first input memory address at FLASH memory */
  DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)FrameBuffer1;
  
  /* Configure foreground color mode */
  DMA2D_FG_InitStruct.DMA2D_FGCM = CM_ARGB8888;
  
  /* Configure foreground alpha value */
  DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_MODE = NO_MODIF_ALPHA_VALUE;
  
  /* Configure foreground alpha mode : The constant alpha for foreground is decreased 
    to see the background */
//  DMA2D_FG_InitStruct.DMA2D_FGPFC_ALPHA_VALUE = 0;

  /* Foreground Initialization */
  DMA2D_FGConfig(&DMA2D_FG_InitStruct);

  /* Configure default values for background */
  DMA2D_BG_StructInit(&DMA2D_BG_InitStruct);
  
  /* Configure background memory address : second input memory address at FLASH memory */
  DMA2D_BG_InitStruct.DMA2D_BGMA = (uint32_t)FrameBuffer0;
  
  /* Configure background color mode */
  DMA2D_BG_InitStruct.DMA2D_BGCM = CM_ARGB8888;
  
  /* Configure background alpha mode */
  DMA2D_BG_InitStruct.DMA2D_BGPFC_ALPHA_MODE = REPLACE_ALPHA_VALUE;
  
  /* Configure background alpha value */
  DMA2D_BG_InitStruct.DMA2D_BGPFC_ALPHA_VALUE = 255;
  
  /* Background Initialization */
  DMA2D_BGConfig(&DMA2D_BG_InitStruct);

}

void DMA2D_Display(void)
{
	DMA2D_StartTransfer();
	while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);
	DMA2D->IFCR=0x3f;
}

/**
  * @brief GPIO configuration for LTDC.
  * @param  None
  * @retval None
  */
void LCD_AF_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable GPIOI, GPIOJ, GPIOK AHB Clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | \
                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI ,  ENABLE);

/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R0 <-> PH.02  |  LCD_TFT G0 <-> PE.05 |  LCD_TFT B0 <-> PE.04      |
 |  LCD_TFT R1 <-> PH.03  |  LCD_TFT G1 <-> PE.06 |  LCD_TFT B1 <-> PG.12      |
 |  LCD_TFT R2 <-> PH.08  |  LCD_TFT G2 <-> PH.13 |  LCD_TFT B2 <-> PG.10      |
 |  LCD_TFT R3 <-> PH.09  |  LCD_TFT G3 <-> PH.14 |  LCD_TFT B3 <-> PG.11      |
 |  LCD_TFT R4 <-> PH.10  |  LCD_TFT G4 <-> PH.15 |  LCD_TFT B4 <-> PI.04      |
 |  LCD_TFT R5 <-> PH.11  |  LCD_TFT G5 <-> PI.00 |  LCD_TFT B5 <-> PI.05      |
 |  LCD_TFT R6 <-> PH.12  |  LCD_TFT G6 <-> PI.01 |  LCD_TFT B6 <-> PI.06      |
 |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PI.02 |  LCD_TFT B7 <-> PI.07      |
 -------------------------------------------------------------------------------
          |  LCD_TFT HSYNC <-> PI.10  | LCDTFT VSYNC <->  PI.09 |
          |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
           -----------------------------------------------------
*/

 /* GPIOI configuration */
  //GPIO E
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_LTDC);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//GPIO F
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	//GPIO G
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_LTDC);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	//GPIO H
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOH, &GPIO_InitStruct);
	
  //GPIO I
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStruct); 
} 


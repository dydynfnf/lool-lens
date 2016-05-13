#include "NandFlash.h"

//状态变量
extern uint8_t Nand_Status;
//NAND初始化 
void NAND_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
  FMC_NAND_PCCARDTimingInitTypeDef  p;
  FMC_NANDInitTypeDef FMC_NANDInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE); 
	//GPIOD引脚初始化
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FMC);   //数据位2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FMC);   //数据位3
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FMC);   //NOE位
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FMC);   //NWE位
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6,  GPIO_AF_FMC);   //NWAIT位
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC);   //地址位16
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);   //地址位17
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);   //数据位0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);   //数据位1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | 
                                 GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	//GPIOE引脚初始化
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FMC);   //数据位4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FMC);   //数据位5
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FMC);   //数据位6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);   //数据位7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//GPIOG引脚初始化
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FMC);   //NCE3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/*--------------FMC 总线 存储器参数配置------------------------------*/
  p.FMC_SetupTime = 0x0;         //建立时间
  p.FMC_WaitSetupTime = 0x4;     //等待时间
  p.FMC_HoldSetupTime = 0x2;     //保持时间
  p.FMC_HiZSetupTime = 0x0;      //高阻建立时间

  FMC_NANDInitStructure.FMC_Bank = FMC_Bank3_NAND; //使用FMC BANK3
  FMC_NANDInitStructure.FMC_Waitfeature = FMC_Waitfeature_Enable; //使能FMC的等待功能
  FMC_NANDInitStructure.FMC_MemoryDataWidth = FMC_NAND_MemoryDataWidth_8b; //NAND Flash的数据宽度为8位
  FMC_NANDInitStructure.FMC_ECC = FMC_ECC_Enable;                  //使能ECC特性
  FMC_NANDInitStructure.FMC_ECCPageSize = FMC_ECCPageSize_2048Bytes; //ECC页大小2048
  FMC_NANDInitStructure.FMC_TCLRSetupTime = 0x00;             
  FMC_NANDInitStructure.FMC_TARSetupTime = 0x00;
  FMC_NANDInitStructure.FMC_CommonSpaceTimingStruct = &p;
  FMC_NANDInitStructure.FMC_AttributeSpaceTimingStruct = &p;

  FMC_NANDInit(&FMC_NANDInitStructure);

  /*!使能FMC BANK3 */
  FMC_NANDCmd(FMC_Bank3_NAND, ENABLE);
}

//读取设备ID
void  NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
    uint64_t data = 0;
  /*!< Send Command to the command area */
  *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = 0x90;
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;

   /*!< Sequence to read ID from NAND flash */
   data = *(__IO uint64_t *)(Bank_NAND_ADDR | DATA_AREA);

   NAND_ID->Maker_ID   = ADDR_1st_CYCLE (data);
   NAND_ID->Device_ID  = ADDR_2nd_CYCLE (data);
   NAND_ID->Third_ID   = ADDR_3rd_CYCLE (data);
   NAND_ID->Fourth_ID  = ADDR_4th_CYCLE (data);	
	 NAND_ID ->Fifth_ID =ADDR_5th_CYCLE (data);
}

//软件复位
void NAND_Reset(void)
{
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_RESET;
}

//块擦除
uint8_t NAND_EraseBlock(NAND_ADDRESS Address)
{
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS );
  *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS );
	*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_3rd_CYCLE(ROW_ADDRESS);
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE1;
	/* 读忙脚, 等待直到空闲 */
	while(NAND_FLASH_BUZY);
	Nand_Status =NAND_GetStatus();
	return Nand_Status ;
}

//读取状态
uint8_t NAND_ReadStatus(void)
{
	uint8_t data=0;
	Nand_Status =NAND_BUSY ;
	*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_STATUS;
	data = *(__IO uint8_t *)(Bank_NAND_ADDR);
	if((data & NAND_ERROR)== NAND_ERROR)
	{
		Nand_Status =NAND_ERROR ;
	}
	else if((data & NAND_READY)==NAND_READY)
	{
		Nand_Status =NAND_READY ;
	}
	else
		Nand_Status =NAND_BUSY ;
	return Nand_Status ;
}

//获得状态（添加延时）
uint8_t NAND_GetStatus(void)
{
	uint32_t timeout = 0x1000000;
	Nand_Status =NAND_BUSY ;
	Nand_Status =NAND_ReadStatus();
	while((Nand_Status!=NAND_READY ) && (timeout!=0))
	{
		Nand_Status =NAND_ReadStatus();
		timeout--;
	}
	if(timeout == 0x00)
  {
    Nand_Status =  NAND_TIMEOUT_ERROR;
  }
	return Nand_Status ;
}

//将相关页写入数据
uint8_t NAND_WritePage(uint8_t *Buffer, NAND_ADDRESS Address,uint32_t NumPageToWrite)
{
	uint32_t Index=0x00;
	uint32_t Size;
	uint32_t NumPageWritten = 0x00;
	Nand_Status =NAND_VALID_ADDRESS ;
	while((NumPageToWrite != 0x00) && ((Nand_Status!=NAND_INVALID_ADDRESS) || (Nand_Status==NAND_READY)))
	{
		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE0;
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS );
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS );
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_3rd_CYCLE(ROW_ADDRESS );
		Size =NAND_PAGE_SIZE*(NumPageWritten +1);
		while(NAND_FLASH_BUZY);
	  for(;Index<Size;Index++)
		{
			*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA) = Buffer[Index];
		}
		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE1;
		Nand_Status = NAND_GetStatus();
		if(Nand_Status ==NAND_READY)
		{
			NumPageWritten ++;
			NumPageToWrite --;
			if((NAND_AddressIncrement (&Address))!=NAND_VALID_ADDRESS)
			{
				Nand_Status =NAND_INVALID_ADDRESS ;
			}
		}
	}
	return Nand_Status ;
}

//读取相关页数据
uint8_t NAND_ReadPage(uint8_t *Buffer, NAND_ADDRESS Address,uint32_t NumPageToRead)
{
	uint32_t Index=0x00;
	uint32_t Size;
	uint32_t NumPageRead = 0x00;
	Nand_Status =NAND_VALID_ADDRESS ;
	while((NumPageToRead != 0x00) && ((Nand_Status!=NAND_INVALID_ADDRESS)))
	{
		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ0;
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(ROW_ADDRESS );
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(ROW_ADDRESS );
		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_3rd_CYCLE(ROW_ADDRESS );
		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ1;
		Size =NAND_PAGE_SIZE*(NumPageRead +1);
		while(NAND_FLASH_BUZY);
		for(;Index<Size;Index++)
		{
			Buffer[Index]= *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA);
		}
		NumPageRead++;
		NumPageToRead --;
		Nand_Status =NAND_AddressIncrement (&Address);
	}
	Nand_Status = NAND_GetStatus();
	return Nand_Status ;
}
void Nand_Block_Write(uint8_t *Buffer,uint32_t BLOCK_Addr,uint16_t Block_Len)
{
	u16 i,j;
	u32 Index;
	NAND_ADDRESS Addr;
	for(j=0;j<Block_Len ;j++)
	{
		Addr.Block =BLOCK_Addr+j;
		Addr .Page =0;
		if(Addr.Block>=NAND_ZONE_SIZE)
		{
			Addr.Block -=NAND_ZONE_SIZE;
			Addr .Zone =1;
		}
		NAND_EraseBlock(Addr);
		for(i=0;i<NAND_BLOCK_SIZE;i++)
		{
			Nand_Status = NAND_GetStatus();
			if(Nand_Status==NAND_READY)
			{
				*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE0;
		    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
        *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
		    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(USB_ADDRESS);
		    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(USB_ADDRESS);
		    *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_3rd_CYCLE(USB_ADDRESS);
				while(NAND_FLASH_BUZY);
				for(Index=NAND_PAGE_SIZE*i+j*NAND_BLOCK_SIZE *NAND_PAGE_SIZE;Index<NAND_PAGE_SIZE*(i+1)+j*NAND_BLOCK_SIZE *NAND_PAGE_SIZE;Index++)
				{
					*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA) = Buffer[Index];
				}
				*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_WRITE1;
			}
		}
	}
	
}
void Nand_Block_Read(uint8_t *Buffer,uint32_t BLOCK_Addr,uint16_t Block_Len)
{
	u16 i,j;
	u32 Index;
	NAND_ADDRESS Addr;
	for(j=0;j<Block_Len ;j++)
	{
		Addr.Block =BLOCK_Addr+j;
		Addr .Page =0;
		if(Addr.Block>=NAND_ZONE_SIZE)
		{
			Addr.Block -=NAND_ZONE_SIZE;
			Addr .Zone =1;
		}
		for(i=0;i<NAND_BLOCK_SIZE;i++)
		{
			Nand_Status = NAND_GetStatus();
			if(Nand_Status==NAND_READY)
			{
				*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ0;
				*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
				*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
				*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(USB_ADDRESS);
				*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(USB_ADDRESS);
				*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_3rd_CYCLE(USB_ADDRESS);
				*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ1;
				while(NAND_FLASH_BUZY);
				for(Index=NAND_PAGE_SIZE*i+j*NAND_BLOCK_SIZE *NAND_PAGE_SIZE;Index<NAND_PAGE_SIZE*(i+1)+j*NAND_BLOCK_SIZE *NAND_PAGE_SIZE;Index++)
				{
					Buffer[Index]= *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA);
				}
			}
		}
	}
}
//地址换算
uint8_t NAND_AddressIncrement(NAND_ADDRESS* Address)
{
	Nand_Status =NAND_VALID_ADDRESS ;
	Address ->Page ++;
	if(Address ->Page==NAND_BLOCK_SIZE )
	{
		Address->Page=0;
		Address ->Block ++;
		if(Address->Block==NAND_ZONE_SIZE)
		{
			Address ->Block =0;
			Address ->Zone ++;
			if(Address ->Zone ==NAND_MAX_ZONE)
			{
				Nand_Status =NAND_INVALID_ADDRESS ;
			}
		}
	}
	return Nand_Status ;
}
void BadBlock_Check(uint8_t *WriteBuffer,uint8_t *ReadBuffer,uint16_t *BadBlock)
{
	uint16_t Index=0,i=0,j=0;
	NAND_ADDRESS CheckAddress;
	CheckAddress.Page =0;
	CheckAddress .Block =0;
	CheckAddress .Zone=1;
//	for(Index=0;Index<NAND_PAGE_SIZE ;Index ++)
//	{
//		if(Index<256)
//		WriteBuffer [Index]=Index ;
//		else
//		WriteBuffer [Index]=66;
//	}
	for(CheckAddress.Block  =0;CheckAddress.Block <NAND_ZONE_SIZE;CheckAddress.Block++)
	{
		NAND_EraseBlock(CheckAddress);
		for(Index=0;Index<NAND_BLOCK_SIZE ;Index++)
		{
			NAND_WritePage(WriteBuffer,CheckAddress,1);
			NAND_ReadPage(ReadBuffer,CheckAddress,1);			
			CheckAddress.Page++;
			for(i=0;i<NAND_PAGE_SIZE;i++)
			{
				if(ReadBuffer[i]!=WriteBuffer[i])
				{
					if(j!=0)
					{
						if(BadBlock[j-1]!=CheckAddress.Block)
						{
							BadBlock[j]=CheckAddress.Block;
							j++;
						}
					}
					if(j==0)
					{
						BadBlock[0]=CheckAddress.Block;
						j++;
					}					
				}
				ReadBuffer[i]=0;
			}	
		}
	}
}



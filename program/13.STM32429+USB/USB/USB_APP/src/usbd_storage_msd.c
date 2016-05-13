/**
  ******************************************************************************
  * @file    usbd_storage_msd.c
  * @author  MCD application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the disk operations functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_mem.h"
#include "usb_conf.h"
#include "sdram.h"
#include "NandFlash.h"

extern u8 FlashBuffer[2048*64];//allocation in SDRAM.c

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup STORAGE 
  * @brief media storage application module
  * @{
  */ 

/** @defgroup STORAGE_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Defines
  * @{
  */ 

#define STORAGE_LUN_NBR                  1 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Variables
  * @{
  */ 
/* USB Mass storage Standard Inquiry Data */
const int8_t  STORAGE_Inquirydata[] = {//36
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'm', 'i', 'c', 'r', 'o', 'S', 'D', ' ', /* Product      : 16 Bytes */
  'F', 'l', 'a', 's', 'h', ' ', ' ', ' ',
  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
}; 

/**
  * @}
  */ 


/** @defgroup STORAGE_Private_FunctionPrototypes
  * @{
  */ 
int8_t STORAGE_Init (uint8_t lun);

int8_t STORAGE_GetCapacity (uint8_t lun, 
                           uint32_t *block_num, 
                           uint32_t *block_size);

int8_t  STORAGE_IsReady (uint8_t lun);

int8_t  STORAGE_IsWriteProtected (uint8_t lun);

int8_t STORAGE_Read (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_Write (uint8_t lun, 
                        uint8_t *buf, 
                        uint32_t blk_addr,
                        uint16_t blk_len);

int8_t STORAGE_GetMaxLun (void);


USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;

/**
  * @}
  */ 


/** @defgroup STORAGE_Private_Functions
  * @{
  */ 


/**
  * @brief  Initialize the storage medium
  * @param  lun : logical unit number
  * @retval Status
  */

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
int8_t STORAGE_Init (uint8_t lun)
{
	SDRAM_Init();
	NAND_Init();
	NAND_Reset();
	
  return (0);
}

/**
  * @brief  return medium capacity and block size
  * @param  lun : logical unit number
  * @param  block_num :  number of physical block
  * @param  block_size : size of a physical block
  * @retval Status
  */

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
  *block_size =  2048;  
  *block_num = 64*2048*2;//16*1024*1024/(*block_size);
  
  return (0);
}

/**
  * @brief  check whether the medium is ready
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsReady (uint8_t lun)
{
  return (0);
}

/**
  * @brief  check whether the medium is write-protected
  * @param  lun : logical unit number
  * @retval Status
  */
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
  return  0;
}

/**
  * @brief  Read data from the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to save data
  * @param  blk_addr :  address of 1st block to be read
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
int8_t STORAGE_Read (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{
// 	uint32_t i=0;
//	uint32_t add = 0xD0000000 + blk_addr * 512;
//	uint32_t len = blk_len * 512;
//	for(i=0;i<len;i++)
//	{
//		*buf++ = *(__IO uint8_t *)(add + i);
//	}
	
//	NAND_ADDRESS Addr;
//	Addr.Page=blk_addr;
//	Addr .Block=0;
//	Addr .Zone =0;
//	NAND_ReadPage(buf, Addr, blk_len);
	
	NAND_ADDRESS Addr;
	uint32_t ZoneStart,BlockStart,PageStart;
	uint32_t ZoneEnd,BlockEnd,PageEnd;
	uint32_t i = 0;
	
	ZoneStart = blk_addr/(64*2048);
	BlockStart = (blk_addr%(64*2048))/64;
	PageStart = (blk_addr%(64*2048))%64;
	
	ZoneEnd = (blk_addr+blk_len-1)/(64*2048);
	BlockEnd = ((blk_addr+blk_len-1)%(64*2048))/64;
	PageEnd = ((blk_addr+blk_len-1)%(64*2048))%64;
	
	for(Addr.Zone=ZoneStart;Addr.Zone<=ZoneEnd;Addr.Zone++)
	{
		for(Addr.Block=BlockStart;Addr.Block<=BlockEnd;Addr.Block++)
		{
			if((Addr.Block==BlockStart)&&(Addr.Block==BlockEnd))
			{
				Addr.Page = PageStart;
				NAND_ReadPage(buf+i, Addr, PageEnd-PageStart+1);
				i+=(PageEnd-PageStart+1)*2048;
			}
			else if(Addr.Block==BlockStart)
			{
				Addr.Page = PageStart;
				NAND_ReadPage(buf+i, Addr, 64-PageStart);
				i+=(64-PageStart)*2048;
			}
			else if(Addr.Block==BlockEnd)
			{
				Addr.Page = 0;
				NAND_ReadPage(buf+i, Addr, PageEnd+1);
				i+=(PageEnd+1)*2048;
			}
			else
			{
				Addr.Page = 0;
				NAND_ReadPage(buf+i, Addr, 64);
				i+=64*2048;
			}
		}
	}
	
  return 0;
}

/**
  * @brief  Write data to the medium
  * @param  lun : logical unit number
  * @param  buf : Pointer to the buffer to write from
  * @param  blk_addr :  address of 1st block to be written
  * @param  blk_len : nmber of blocks to be read
  * @retval Status
  */
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
int8_t STORAGE_Write (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
//	uint32_t i=0;
//	uint32_t add = 0xD0000000 + blk_addr * 512;
//	uint32_t len = blk_len * 512;
//	for(i=0;i<len;i++)
//	{
//		*(__IO uint8_t *)(add + i) = *buf++;
//	}

//	u32 i;
//	NAND_ADDRESS Addr;
//	
//	Addr.Page=0;
//	Addr .Block=0;
//	Addr .Zone =0;
//	NAND_ReadPage(FlashBuffer,Addr,64);
//	
//  for(i=blk_addr*2048;i<(blk_addr+blk_len)*2048;i++)
//	{
//		FlashBuffer[i]=buf[i-2048*blk_addr];
//	}
//	
//	Addr.Page=0;
//	Addr .Block=0;
//	Addr .Zone =0;
//	NAND_EraseBlock(Addr);
//	
//	Addr.Page= 0;
//	Addr.Block=0;
//	Addr.Zone=0;
//	NAND_WritePage(FlashBuffer,Addr,64);

	NAND_ADDRESS Addr;
	
	uint32_t ZoneStart,BlockStart,PageStart;
	uint32_t ZoneEnd,BlockEnd,PageEnd;
	uint32_t i=0;
	
	ZoneStart = blk_addr/(64*2048);
	BlockStart = (blk_addr%(64*2048))/64;
	PageStart = (blk_addr%(64*2048))%64;
	
	ZoneEnd = (blk_addr+blk_len-1)/(64*2048);
	BlockEnd = ((blk_addr+blk_len-1)%(64*2048))/64;
	PageEnd = ((blk_addr+blk_len-1)%(64*2048))%64;
	
	for(Addr.Zone=ZoneStart;Addr.Zone<=ZoneEnd;Addr.Zone++)
	{
		for(Addr.Block=BlockStart;Addr.Block<=BlockEnd;Addr.Block++)
		{
			Addr.Page= 0;
			NAND_ReadPage(FlashBuffer,Addr,64);
			if((Addr.Block==BlockStart)&&(Addr.Block==BlockEnd))
			{
				for(i=PageStart*2048;i<(PageEnd+1)*2048;i++)
				{
					FlashBuffer[i]=buf[i-PageStart*2048];
				}
			}
			else if(Addr.Block==BlockStart)
			{
				for(i=PageStart*2048;i<64*2048;i++)
				{
					FlashBuffer[i]=buf[i-PageStart*2048];
				}
			}
			else if(Addr.Block==BlockEnd)
			{
				for(i=0;i<(PageEnd+1)*2048;i++)
				{
					FlashBuffer[i]=buf[i];
				}
			}
			else
			{
				for(i=0;i<64*2048;i++)
				{
					FlashBuffer[i]=buf[i];
				}
			}
			Addr.Page=0;
			NAND_EraseBlock(Addr);
			Addr.Page=0;
			NAND_WritePage(FlashBuffer,Addr,64);
		}
	}
	
  return (0);
}

/**
  * @brief  Return number of supported logical unit
  * @param  None
  * @retval number of logical unit
  */

int8_t STORAGE_GetMaxLun (void)
{
  return (STORAGE_LUN_NBR - 1);
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

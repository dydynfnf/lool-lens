#ifndef __NANDFLASH_H
#define __NANDFLASH_H
#include "stm32f4xx_fsmc.h"

#define Bank_NAND_ADDR  ((uint32_t)0x80000000) 

#define CMD_AREA                   (uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  /* A17 = ALE high */
#define DATA_AREA                  ((uint32_t)0x00000000) 

#define ADDR_1st_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)                 /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)        /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF0000) >> 16)     /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF000000) >> 24)   /* 4th addressing cycle */ 
#define ADDR_5th_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00000000) >> 32) /* 5th addressing cycle */ 

typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
	uint8_t Fifth_ID;
}NAND_IDTypeDef;

typedef struct 
{
  uint16_t Zone;
  uint16_t Block;
  uint16_t Page;
} NAND_ADDRESS;  

//状态
#define NAND_BUSY                  ((uint8_t)0x00)
#define NAND_ERROR                 ((uint8_t)0x01)
#define NAND_READY                 ((uint8_t)0x40)
#define NAND_VALID_ADDRESS         ((uint8_t)0x08)
#define NAND_INVALID_ADDRESS       ((uint8_t)0x04)
#define NAND_TIMEOUT_ERROR         ((uint8_t)0x10)

//MT29F4G08
#define NAND_PAGE_SIZE       ((uint16_t)0x0800) /* 2048 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE      ((uint16_t)0x0040) /* 64 pages per block */
#define NAND_ZONE_SIZE       ((uint16_t)0x0800) /* 2048 Block per zone */
#define NAND_SPARE_AREA_SIZE ((uint16_t)0x0040) /* last 64 bytes as spare area */
#define NAND_MAX_ZONE        ((uint16_t)0x0002) /* 2 zones of 2048 block */

#define ROW_ADDRESS (Address.Page + ((Address.Block<<1) + Address.Zone ) * NAND_BLOCK_SIZE) 
#define USB_ADDRESS  (Addr.Page + ((Addr.Block<<1) + Addr.Zone ) * NAND_BLOCK_SIZE)
#define NAND_FLASH_BUZY    (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0)


/****************Command******************/
//复位
#define NAND_CMD_RESET             ((uint8_t)0xFF)
//擦除指令
#define NAND_CMD_ERASE0            ((uint8_t)0x60)
#define NAND_CMD_ERASE1            ((uint8_t)0xD0)
//读取状态指令
#define NAND_CMD_STATUS            ((uint8_t)0x70)
//写数据指令
#define NAND_CMD_WRITE0            ((uint8_t)0x80)    
#define NAND_CMD_WRITE1            ((uint8_t)0x10)
//读数据指令
#define NAND_CMD_READ0            ((uint8_t)0x00)    
#define NAND_CMD_READ1            ((uint8_t)0x30)




void NAND_Init(void);
void NAND_ReadID(NAND_IDTypeDef* NAND_ID);
void NAND_Reset(void);
uint8_t NAND_EraseBlock(NAND_ADDRESS Address);
uint8_t NAND_ReadStatus(void);
uint8_t NAND_GetStatus(void);
uint8_t NAND_WritePage(uint8_t *Buffer, NAND_ADDRESS Address,uint32_t NumPageToWrite);
uint8_t NAND_ReadPage(uint8_t *Buffer, NAND_ADDRESS Address,uint32_t NumPageToRead);
void Nand_Block_Write(uint8_t *Buffer,uint32_t BLOCK_Addr,uint16_t Block_Len);
void Nand_Block_Read(uint8_t *Buffer,uint32_t BLOCK_Addr,uint16_t Block_Len);
uint8_t NAND_AddressIncrement(NAND_ADDRESS* Address);
void BadBlock_Check(uint8_t *WriteBuffer,uint8_t *ReadBuffer,uint16_t *BadBlock);

#endif



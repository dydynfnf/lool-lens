/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V3，MINI，V2，V2.1上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: mass_mal.c
* 内容简述:	
*       
*	SD卡和USB接口的驱动函数及接口函数

*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-28 sun68  创建该文件
*
*/

/* Includes ------------------------------------------------------------------*/

#include "platform_config.h" 
//#include "sdcard.h"	    
#include "mass_mal.h"
#include "stdio.h"
#include "demo.h"
#include "sdio_sdcard.h"

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
//#define BlockSize            128 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks1       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks1) >> 2)
uint32_t Mass_Memory_Size1;
uint32_t Mass_Block_Size1;
uint32_t Mass_Block_Count1;

__IO uint32_t Status = 0;
uint32_t Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
uint32_t Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
unsigned char dis_mem=0;

extern SD_CardInfo SDCardInfo;


/****************************************************************************
* 名    称：uint16_t MAL_Init(uint8_t lun)
* 功    能：存储介质初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t status = MAL_OK;

  switch (lun)
  {
    case 0:

      Status = SD_Init();
      Status = SD_GetCardInfo(&SDCardInfo);
      Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
      Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
      Status = SD_SetDeviceMode(SD_DMA_MODE);  
 
 	  if (Status == SD_OK)
  	  {	
        // 从地址0开始读取512字节 
    	Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  BlockSize); 
  	  }
  	  if (Status == SD_OK)
      {	 
         // 返回成功的话，串口输出SD卡测试成功信息 
//		 USART_OUT(USART1,"\r\nSD SDIO-4bit模式 测试TF卡读写成功！ \n ");
      } 
      break;
    default:
      return MAL_FAIL;
  }
  return status;
}

/****************************************************************************
* 名    称：uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
* 功    能：USB存储介质写驱动
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
	  //在指定地址连续写入指定长度的数据
      Status = SD_WriteBlock(Writebuff,Memory_Offset,  Transfer_Length);
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }      

      break;

    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/****************************************************************************
* 名    称：uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
* 功    能：USB存储介质读驱动
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
	  //从指定地址连续读出指定长度的数据
      Status = SD_ReadBlock(Readbuff,Memory_Offset,  Transfer_Length);
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }

      break;

    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/****************************************************************************
* 名    称：uint16_t MAL_GetStatus (uint8_t lun)
* 功    能：获得存储介质信息
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{


  uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
  if (lun == 0)
  {

    if (SD_Init() == SD_OK)
    {
      SD_GetCardInfo(&SDCardInfo);
      SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
      DeviceSizeMul = (SDCardInfo.SD_csd.DeviceSizeMul + 2);

      if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        Mass_Block_Count[0] = (SDCardInfo.SD_csd.DeviceSize + 1) * 1024;
      }
      else
      {
        NumberOfBlocks  = ((1 << (SDCardInfo.SD_csd.RdBlockLen)) / 512);
        Mass_Block_Count[0] = ((SDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
      }
      Mass_Block_Size[0]  = 512;

      Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16)); 
      Status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      }
       
      Status = SD_SetDeviceMode(SD_DMA_MODE);         
      if ( Status != SD_OK )
      {
        return MAL_FAIL;
      } 
      Mass_Memory_Size[0] = Mass_Block_Count[0] * Mass_Block_Size[0];
      Led_ON();
	  if(dis_mem==0){		//显示TF卡的容量	由于是周期性更新状态，所以，加了条件，只显示一次容量值
//	  	USART_OUT(USART1,"\r\n Micro SD卡的容量是  %d MBytes\n" ,Mass_Memory_Size[0]/1024/1024);
      	dis_mem=1;
	  }
	  return MAL_OK;
    }
  }
  Led_ON();
  return MAL_FAIL;
}
/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/

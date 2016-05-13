/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V3��MINI��V2��V2.1�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: mass_mal.c
* ���ݼ���:	
*       
*	SD����USB�ӿڵ������������ӿں���

*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-28 sun68  �������ļ�
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
* ��    �ƣ�uint16_t MAL_Init(uint8_t lun)
* ��    �ܣ��洢���ʳ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
        // �ӵ�ַ0��ʼ��ȡ512�ֽ� 
    	Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  BlockSize); 
  	  }
  	  if (Status == SD_OK)
      {	 
         // ���سɹ��Ļ����������SD�����Գɹ���Ϣ 
//		 USART_OUT(USART1,"\r\nSD SDIO-4bitģʽ ����TF����д�ɹ��� \n ");
      } 
      break;
    default:
      return MAL_FAIL;
  }
  return status;
}

/****************************************************************************
* ��    �ƣ�uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
* ��    �ܣ�USB�洢����д����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
	  //��ָ����ַ����д��ָ�����ȵ�����
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
* ��    �ƣ�uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
* ��    �ܣ�USB�洢���ʶ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{

  switch (lun)
  {
    case 0:
	  //��ָ����ַ��������ָ�����ȵ�����
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
* ��    �ƣ�uint16_t MAL_GetStatus (uint8_t lun)
* ��    �ܣ���ô洢������Ϣ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
	  if(dis_mem==0){		//��ʾTF��������	�����������Ը���״̬�����ԣ�����������ֻ��ʾһ������ֵ
//	  	USART_OUT(USART1,"\r\n Micro SD����������  %d MBytes\n" ,Mass_Memory_Size[0]/1024/1024);
      	dis_mem=1;
	  }
	  return MAL_OK;
    }
  }
  Led_ON();
  return MAL_FAIL;
}
/******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/

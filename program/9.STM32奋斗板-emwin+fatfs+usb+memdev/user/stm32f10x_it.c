/**
  ******************************************************************************
  * @file FSMC/SRAM/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_exti.h"
#include "sdio_sdcard.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"

/** @addtogroup StdPeriph_Examples
  * @{
  */

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
extern volatile int OS_TimeMS;
void SysTick_Handler(void)
{
	OS_TimeMS++;
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{	 

}

///******************************************************************************/
///*            STM32F10x Peripherals Interrupt Handlers                        */
///******************************************************************************/

//#ifndef STM32F10X_CL
///*******************************************************************************
//* Function Name  : USB_HP_CAN1_TX_IRQHandler
//* Description    : This function handles USB High Priority or CAN TX interrupts requests
//*                  requests.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void USB_HP_CAN1_TX_IRQHandler(void)
//{
//  CTR_HP();
//}

///*******************************************************************************
//* Function Name  : USB_LP_CAN1_RX0_IRQHandler
//* Description    : This function handles USB Low Priority or CAN RX0 interrupts
//*                  requests.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//  USB_Istr();
//}
//#endif /* STM32F10X_CL */

//#ifdef STM32F10X_HD
///*******************************************************************************
//* Function Name  : SDIO_IRQHandler
//* Description    : This function handles SDIO global interrupt request.
//*                  requests.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void SDIO_IRQHandler(void)
//{ 
//  /* Process All SDIO Interrupt Sources */
//  SD_ProcessIRQSrc();
//  
//}
//#endif /* STM32F10X_HD */

//#ifdef STM32F10X_CL
///*******************************************************************************
//* Function Name  : OTG_FS_IRQHandler
//* Description    : This function handles USB-On-The-Go FS global interrupt request.
//*                  requests.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void OTG_FS_IRQHandler(void)
//{
//  STM32_PCD_OTG_ISR_Handler(); 
//}
//#endif /* STM32F10X_CL */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

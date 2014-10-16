/**
  ******************************************************************************
  * @file    TscHal.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    11-July-2011
  * @brief   This file contains all the TouchScreen functions whose
  *          implementation depends on the TSC Controller used in your Design.
  *          You only need to change these functions implementations
  *          in order to reuse this code with other TSC Controller
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __TSC_HAL_H
#define __TSC_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "graphicObjectTypes.h"

#define TOUCH_SCREEN_CAPABILITY			(1)
#define FLASH_PAGE_SIZE          ((uint16_t)0x800)

#define	CHX 	0x90
#define	CHY 	0xd0

//#define TP_CS(x)	x ? GPIO_SetBits(GPIOA,GPIO_Pin_6): GPIO_ResetBits(GPIOA,GPIO_Pin_6)
//#define TP_IRQ          GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define TP_CS(x)	x ? GPIO_SetBits(GPIOD,GPIO_Pin_12): GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define TP_IRQ          GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)


typedef enum
{
  FAILED = 0,
  PASSED = !FAILED
} Flash_TestStatus;


FLASH_Status TSC_WriteDataToNVM(uint32_t FlashFree_Address, int32_t *Data, uint32_t Size);
void TSC_Init(void);
void TSC_NVIC_Configuration(void);
void TSC_InterruptConfig(void);
void TSC_FLASH_Unlock(void);
void TSC_FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status TSC_FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status TSC_FLASH_ProgramWord(uint32_t Address, uint32_t Data);
void Set_LastFlashMemoryAddress( uint32_t address);
void TSC_Read(void);

#ifdef __cplusplus
}
#endif

#endif /*__TSC_HAL_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

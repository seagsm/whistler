/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/

#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "board_GUI_primitives.h"
#include "systick.h"
#include "GLCD.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT
{
   uint16_t x;
   uint16_t y;
}Coordinate;


typedef struct Matrix
{
  long double An,
              Bn,
              Cn,
              Dn,
              En,
              Fn,
              Divider ;
} Matrix ;


#define TEXT_OF_CALLIBRATION_X_POSITION 120
#define TEXT_OF_CALLIBRATION_Y_POSITION 45


/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

/* Private define ------------------------------------------------------------*/
/* AD通道选择命令字和工作寄存器 */
#define	CHX 	0x90 	/* 通道Y+的选择控制字 */
#define	CHY 	0xd0	/* 通道X+的选择控制字 */


#define TP_CS(a)	if (a)	\
					GPIO_SetBits(GPIOD,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOD,GPIO_Pin_12)

#define TP_INT_IN   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)

/* Private function prototypes -----------------------------------------------*/
void TP_Init(void);
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void TouchPanel_Horizontal_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/



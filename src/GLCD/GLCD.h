/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.h
** Descriptions:		STM32 FSMC TFT²Ù×÷º¯Êý¿â
**
**------------------------------------------------------------------------------------------------------
** Created by:			poweravr
** Created date:		2010-11-7
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/

#ifndef __GLCD_H
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define __inline inline
/* Private define ------------------------------------------------------------*/

/* LCD Registers */
#define R0             0x00U
#define R1             0x01U
#define R2             0x02U
#define R3             0x03U
#define R4             0x04U
#define R5             0x05U
#define R6             0x06U
#define R7             0x07U
#define R8             0x08U
#define R9             0x09U
#define R10            0x0AU
#define R12            0x0CU
#define R13            0x0DU
#define R14            0x0EU
#define R15            0x0FU
#define R16            0x10U
#define R17            0x11U
#define R18            0x12U
#define R19            0x13U
#define R20            0x14U
#define R21            0x15U
#define R22            0x16U
#define R23            0x17U
#define R24            0x18U
#define R25            0x19U
#define R26            0x1AU
#define R27            0x1BU
#define R28            0x1CU
#define R29            0x1DU
#define R30            0x1EU
#define R31            0x1FU
#define R32            0x20U
#define R33            0x21U
#define R34            0x22U
#define R36            0x24U
#define R37            0x25U
#define R40            0x28U
#define R41            0x29U
#define R43            0x2BU
#define R45            0x2DU
#define R48            0x30U
#define R49            0x31U
#define R50            0x32U
#define R51            0x33U
#define R52            0x34U
#define R53            0x35U
#define R54            0x36U
#define R55            0x37U
#define R56            0x38U
#define R57            0x39U
#define R59            0x3BU
#define R60            0x3CU
#define R61            0x3DU
#define R62            0x3EU
#define R63            0x3FU
#define R64            0x40U
#define R65            0x41U
#define R66            0x42U
#define R67            0x43U
#define R68            0x44U
#define R69            0x45U
#define R70            0x46U
#define R71            0x47U
#define R72            0x48U
#define R73            0x49U
#define R74            0x4AU
#define R75            0x4BU
#define R76            0x4CU
#define R77            0x4DU
#define R78            0x4EU
#define R79            0x4FU
#define R80            0x50U
#define R81            0x51U
#define R82            0x52U
#define R83            0x53U
#define R96            0x60U
#define R97            0x61U
#define R106           0x6AU
#define R118           0x76U
#define R128           0x80U
#define R129           0x81U
#define R130           0x82U
#define R131           0x83U
#define R132           0x84U
#define R133           0x85U
#define R134           0x86U
#define R135           0x87U
#define R136           0x88U
#define R137           0x89U
#define R139           0x8BU
#define R140           0x8CU
#define R141           0x8DU
#define R143           0x8FU
#define R144           0x90U
#define R145           0x91U
#define R146           0x92U
#define R147           0x93U
#define R148           0x94U
#define R149           0x95U
#define R150           0x96U
#define R151           0x97U
#define R152           0x98U
#define R153           0x99U
#define R154           0x9AU
#define R157           0x9DU
#define R192           0xC0U
#define R193           0xC1U
#define R229           0xE5U

/* LCD color */
#define White          0xFFFFU
#define Black          0x0000U
#define Grey           0xF7DEU
#define Blue           0x001FU
#define Blue2          0x051FU
#define Red            0xF800U
#define Magenta        0xF81FU
#define Green          0x07E0U
#define Cyan           0x7FFFU
#define Yellow         0xFFE0U







#define ASCII_LIB      1

#define RGB565CONVERT(red, green, blue) (unsigned int) (((unsigned int)(((unsigned int)((red) >> 3))<<11))|((unsigned int)(((unsigned int)((green) >> 2))<< 5))|((unsigned int)((blue) >> 3)))

void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);


void LCD_Initialization(void);
void LCD_BackLight_Init(void);
void LCD_Clear(uint16_t Color);
uint16_t LCD_BGR2RGB(uint16_t color);
FunctionalState LCD_BackLight( uint8_t percent);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void LCD_DrawLine(int x1, int y1, int x2, int y2,uint16_t bkColor);
void LCD_SetWindows(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong);
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic[]);
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t str[],uint16_t Color, uint16_t bkColor);
void GUI_Text_Rotated(uint16_t Xpos, uint16_t Ypos, const char str[],uint16_t Color, uint16_t bkColor,uint8_t rotation);
void PutChar(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor);
void PutCharRotate(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor,uint8_t rotation);
void RotateCharBuffer(unsigned char* pBuffer);

/* Private function prototypes -----------------------------------------------*/
static void LCD_CtrlLinesConfig(void);
static void LCD_FSMCConfig(void);
static void LCD_Configuration(void);
static inline void LCD_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue);
static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg);
static __inline uint16_t LCD_ReadRAM(void);
static void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos);
static void LCD_Delay(uint16_t nCount);



#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






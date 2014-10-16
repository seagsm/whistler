#ifndef __LCD_HAL_H
#define __LCD_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "graphicObjectTypes.h"
#include "ssd1289.h"
   
typedef enum
{ _0_degree = 0,
  _90_degree,
  _180_degree,
  _270_degree
}LCD_Direction_TypeDef;

extern __IO uint16_t          GL_TextColor;
extern __IO uint16_t          GL_BackColor;

#define GL_OFF                0x00
#define GL_ON                 0x01

#define GL_White              White
#define GL_Black              Black
#define GL_Grey               Grey
#define GL_Blue               Blue
#define GL_Blue2              Blue2
#define GL_Red                Red
#define GL_Magenta            Magenta
#define GL_Green              Green
#define GL_Cyan               Cyan
#define GL_Yellow             Yellow

#define GL_Horizontal         0x00
#define GL_Vertical           0x01

#define GL_FONT_BIG           0x00
#define GL_FONT_SMALL         0x01
#define GL_FONT_BIG_WIDTH       16
#define GL_FONT_BIG_HEIGHT      24
#define GL_FONT_SMALL_WIDTH      8
#define GL_FONT_SMALL_HEIGHT    12

#define FirstPixel            0x01
#define MiddlePixel           0x02
#define LastPixel             0x04
#define SinglePixel           0x08

#define CursorColor           GL_Black

extern __IO uint8_t           GL_Font;
extern __IO uint8_t           GL_FontWidth;
extern __IO uint8_t           GL_FontHeight;
extern uint16_t               LCD_Height;
extern uint16_t               LCD_Width;

void GL_SetTextColor(__IO uint16_t TextColor);
void GL_SetBackColor(__IO uint16_t BackColor);
void GL_Clear(uint16_t Color);
void GL_LCD_DrawCharTransparent(uint16_t Xpos, uint16_t Ypos, const uint16_t *c); /* 16bit char */
void GL_LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii, bool Trasparent_Flag);
void GL_DisplayAdjStringLine(uint16_t Line, uint16_t Column, uint8_t *ptr, bool Transparent_Flag);
void GL_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void GL_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void GL_LCD_DrawRect(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width);
void GL_LCD_DrawCircle(uint8_t Xpos, uint16_t Ypos, uint16_t Radius);
void GL_DrawBMP(uint8_t* ptrBitmap);
void GL_SetFont(uint8_t uFont);
void GL_BackLightSwitch(uint8_t u8_State);
void GL_LCD_SPIConfig(void);
void GL_LCD_FSMCConfig(void);
void GL_LCD_Init(void);
void GL_LCD_WindowModeDisable(void);
void LCD_PutPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color, uint8_t PixelSpec);
void LCD_FillArea(uint16_t Xpos_Init, uint16_t Ypos_Init, uint16_t Height, uint16_t Width, uint16_t color);
uint16_t LCD_GetPixel(uint16_t Xpos, uint16_t Ypos);
void GL_LCD_DrawChar(uint8_t Xpos, uint16_t Ypos, const uint16_t *c);
uint16_t GL_LCD_ReadRAM(void);
void LCD_WriteRAMWord(uint16_t RGB_Code);

void LCD_Change_Direction(LCD_Direction_TypeDef Direction);
void LCD_WriteChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
void LCD_PrintChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void LCD_PrintStringLine(uint16_t Line, uint16_t Column, uint8_t *ptr);
void LCD_DrawMonoBMP(const uint8_t *Pict, uint16_t Xpos_Init, uint16_t Ypos_Init, uint16_t Height, uint16_t Width);
void LCD_DrawColorBMP(uint8_t* ptrBitmap, uint16_t Xpos_Init, uint16_t Ypos_Init, uint16_t Height, uint16_t Width);

#ifdef __cplusplus
}
#endif

#endif
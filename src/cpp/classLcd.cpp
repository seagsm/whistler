

#include "classLcd.h"

/* floatValue = static_cast<float>(intValue); */

Lcd::Lcd(uint8_t u8_Brightness, uint16_t u16_Color)
{
    LCD_Initialization();
    LCD_BackLight_Init();
    LCD_BackLight(u8_Brightness); /*Brightness in persentages. */
    LCD_Clear(u16_Color);
};

void Lcd::mLCD_WriteRAM_Prepare(void)
{
    LCD_WriteRAM_Prepare();
};

void Lcd::mLCD_WriteRAM(uint16_t RGB_Code)
{
    LCD_WriteRAM(RGB_Code);
};

void Lcd::mLCD_Initialization(void)
{
    LCD_Initialization();
};

void Lcd::mLCD_BackLight_Init(void)
{
    LCD_BackLight_Init();
};

void Lcd::mLCD_Clear(uint16_t color)
{
    LCD_Clear(color);
};

uint16_t Lcd::mLCD_BGR2RGB(uint16_t color)
{
    return(LCD_BGR2RGB(color));
};

FunctionalState Lcd::mLCD_BackLight( uint8_t percent)
{
    return(LCD_BackLight(percent));
};

uint16_t Lcd::mLCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
    return(LCD_GetPoint(Xpos, Ypos));
};

void Lcd::mLCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
    LCD_SetPoint(Xpos, Ypos, point);
};

void Lcd::mLCD_DrawLine(int x1, int y1, int x2, int y2,uint16_t bkColor)
{
    LCD_DrawLine(x1, y1, x2, y2, bkColor);
};

void Lcd::mLCD_SetWindows(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong)
{
    LCD_SetWindows(xStart, yStart, xLong, yLong);
};

void Lcd::mGUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t str[],uint16_t color, uint16_t bkColor)
{
    GUI_Text(Xpos, Ypos, str,color, bkColor);
};

void Lcd::mGUI_Text_Rotated(uint16_t Xpos, uint16_t Ypos, const char str[],uint16_t color, uint16_t bkColor,uint8_t rotation)
{
    GUI_Text_Rotated(Xpos, Ypos, str, color, bkColor, rotation);
};

void Lcd::mPutChar(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor)
{
    PutChar(Xpos, Ypos, c, charColor, bkColor);
};

void Lcd::mPutCharRotate(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor,uint8_t rotation)
{
    PutCharRotate(Xpos, Ypos, c, charColor, bkColor, rotation);
};

void Lcd::mRotateCharBuffer(unsigned char* pBuffer)
{
    RotateCharBuffer( pBuffer);
};





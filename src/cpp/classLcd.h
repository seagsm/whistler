#ifndef CLASS_LCD_H
#define CLASS_LCD_H 1

extern "C"
{
    #include "TouchPanel.h"
    #include "GLCD.h"
}

class Lcd
{
    public:
        Lcd(uint8_t u8_Brightness, uint16_t u16_Color);

        void mLCD_WriteRAM_Prepare(void);
        void mLCD_WriteRAM(uint16_t RGB_Code);

        void mLCD_Initialization(void);
        void mLCD_BackLight_Init(void);
        void mLCD_Clear(uint16_t color);
    uint16_t mLCD_BGR2RGB(uint16_t color);
FunctionalState mLCD_BackLight( uint8_t percent);
    uint16_t mLCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
        void mLCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
        void mLCD_DrawLine(int x1, int y1, int x2, int y2,uint16_t bkColor);
        void mLCD_SetWindows(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong);
        void mGUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t str[],uint16_t color, uint16_t bkColor);
        void mGUI_Text_Rotated(uint16_t Xpos, uint16_t Ypos, const char str[],uint16_t color, uint16_t bkColor,uint8_t rotation);
        void mPutChar(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor);
        void mPutCharRotate(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor,uint8_t rotation);
        void mRotateCharBuffer(unsigned char* pBuffer);

    protected:

    private:
};


#endif
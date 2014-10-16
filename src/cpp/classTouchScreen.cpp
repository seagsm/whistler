

#include "classTouchScreen.h"

/* floatValue = static_cast<float>(intValue); */

TouchScreen::TouchScreen(void)
{

};
void TouchScreen::Initialize(void)
{
    TP_Init();
};

void TouchScreen::mTouchPanel_Horizontal_Calibrate(char const *str, uint16_t TextColor, uint16_t Text_bkColor, uint16_t ScreenColor)
{
    TouchPanel_Horizontal_Calibrate(str, TextColor, Text_bkColor, ScreenColor);
};

FunctionalState TouchScreen::UpdateDisplayPoint(void)
{
        return(getDisplayPoint(&mPosition, Read_Ads7846(), &matrix ));
};

Coordinate TouchScreen::mgetDisplayPoint(void)
{
    return (mPosition);
};
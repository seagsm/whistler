

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

FunctionalState TouchScreen::mgetDisplayPoint(Coordinate* displayPtr)
{
        return(getDisplayPoint(displayPtr, Read_Ads7846(), &matrix )) ;
};        
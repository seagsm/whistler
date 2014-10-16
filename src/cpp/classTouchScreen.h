#ifndef CLASS_TOUCHSCREEN_H
#define CLASS_TOUCHSCREEN_H 1

extern "C" 
{
    #include "TouchPanel.h"
}

class TouchScreen
{ 
    public: 
            TouchScreen(void);
            void Initialize(void);
            void mTouchPanel_Horizontal_Calibrate(char const *str, uint16_t TextColor, uint16_t Text_bkColor, uint16_t ScreenColor); 
            FunctionalState mgetDisplayPoint(Coordinate* displayPtr);
        
    protected:
    
    private:
};


#endif
#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H 1

#include "classLcd.h"
#include "classDraw.h"

extern "C"
{
    #include "ts_button.h"
}

class Button
{
    public:
        GUI_BUTTON button;
        Button(void);
        void Initialization(
                                Lcd* lcd,
                                uint16_t Xpos,
                                uint16_t Ypos,
                                uint16_t height,
                                uint16_t width,
                                const char* Text,
                                void (*mOnTouch)(void),
                                void (*mOnUnTouch)(void)

                            );
        void OnDraw(uint8_t condition);
        void PressOnOffDraw(uint8_t condition);
        void OnTouchRead(uint16_t Xpos,uint16_t Ypos);
        void OnTouch(uint16_t Xpos,uint16_t Ypos);

    protected:

    private:

        Lcd* buttonLcd;
        Draw buttonDrawTool;
};


#endif
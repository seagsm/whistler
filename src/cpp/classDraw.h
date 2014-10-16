#ifndef CLASS_DRAW_H
#define CLASS_DRAW_H 1

#include "classLcd.h"

extern "C" 
{
    #include "board_GUI_primitives.h"
}

class Draw
{ 
    public: 
        Draw(void);
        void Initialization(Lcd* lcd);
        void X_Line(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t color);
        void Y_Line(uint16_t Xpos,uint16_t Ypos, uint16_t width, uint16_t color);
        void Rect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color);
        void FilledRect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color);
        void Circle(uint16_t Xpos, uint16_t Ypos, uint16_t radius,uint16_t color);
        void FullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color);
    
    protected:
    
    private:
    Lcd* lcdDraw;  

};


#endif
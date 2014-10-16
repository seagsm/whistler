
#include "classDraw.h"

/* floatValue = static_cast<float>(intValue); */

Draw::Draw(void)
{
  
};

void Draw::Initialization(Lcd* lcd)
{
    lcdDraw = lcd;
};

void Draw::X_Line(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t color)
{
    uint16_t counter = 0U;
    lcdDraw->mLCD_SetPoint(Xpos,Ypos,color);
    
    heigth --;
    lcdDraw->mLCD_WriteRAM_Prepare();
    while(counter < heigth)
    {  /* For line X work autoincrementing, so we dod not need address variable. */
        lcdDraw->mLCD_WriteRAM(color);
        counter++;
    }  
};

void Draw::Y_Line(uint16_t Xpos,uint16_t Ypos, uint16_t width, uint16_t color)
{
    uint16_t counter = 0U;
    while(counter < width)
    {
        lcdDraw->mLCD_SetPoint(Xpos,Ypos+counter,color);
        counter++;
    }
};

void Draw::Rect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color)
{
    X_Line(Xpos,Ypos,heigth,color);                     /* Draw left up line.*/
    Y_Line(Xpos + (heigth-1U),Ypos, width,color);       /* Draw up line from left to right. */
    X_Line(Xpos,Ypos+(width-1U),heigth,color);          /* Draw right up line.*/
    Y_Line(Xpos,Ypos, width,color);                     /* Draw down line from left to right. */
};

void Draw::FilledRect(uint16_t Xpos,uint16_t Ypos, uint16_t heigth, uint16_t width, uint16_t color)
{
    uint16_t u16_counter = 0U;
    
    while(u16_counter < width)
    {
        /* Draw line from down to up and move to left for drawing next line.*/
        X_Line(Xpos, Ypos + u16_counter, heigth, color);
        u16_counter++;
    }
};

void Draw::Circle(uint16_t Xpos, uint16_t Ypos, uint16_t radius,uint16_t color)
{
    int16_t i16_x,i16_y;
    int16_t f = 1 - (int16_t)radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * (int16_t)radius;
    int16_t x = 0;
    int16_t y = (int16_t)radius;
        
    lcdDraw->mLCD_SetPoint(Xpos, Ypos + radius,color);
    lcdDraw->mLCD_SetPoint(Xpos, Ypos - radius,color);
    lcdDraw->mLCD_SetPoint(Xpos + radius, Ypos,color);
    lcdDraw->mLCD_SetPoint(Xpos - radius, Ypos,color);

    while(x < y)
    {
	if(f >= 0)
	{
            y--;
            ddF_y += 2;
            f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;
		
        /*LCD_SetPoint(Xpos + x, Ypos + y,color); */
        i16_x = (int16_t)Xpos + x;
        i16_y = (int16_t)Ypos + y;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos - x, Ypos + y,color); */
        i16_x = (int16_t)Xpos - x;
        i16_y = (int16_t)Ypos + y;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos + x, Ypos - y,color); */
        i16_x = (int16_t)Xpos + x;
        i16_y = (int16_t)Ypos - y;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

        /* LCD_SetPoint(Xpos - x, Ypos - y,color); */
        i16_x = (int16_t)Xpos - x;
        i16_y = (int16_t)Ypos - y;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos + y, Ypos + x,color); */
        i16_x = (int16_t)Xpos + y;
        i16_y = (int16_t)Ypos + x;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos - y, Ypos + x,color); */
        i16_x = (int16_t)Xpos - y;
        i16_y = (int16_t)Ypos + x;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos + y, Ypos - x,color);*/
        i16_x = (int16_t)Xpos + y;
        i16_y = (int16_t)Ypos - x;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);

	/* LCD_SetPoint(Xpos - y, Ypos - x,color); */
        i16_x = (int16_t)Xpos - y;
        i16_y = (int16_t)Ypos - x;
	lcdDraw->mLCD_SetPoint((uint16_t)i16_x, (uint16_t)i16_y, color);
    }
};

void Draw::FullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t radius, uint16_t color)
{
    int16_t i16_x,i16_y, i16_r;
    int16_t f = 1 - (int16_t)radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * (int16_t)radius;
    int16_t x = 0;
    int16_t y = (int16_t)radius;

    /* Draw centrall horizontal line*/
    i16_x = (int16_t)Xpos + x;
    i16_y = (int16_t)Ypos - y;
    i16_r = 2*y;
    
    Y_Line((uint16_t)i16_x, (uint16_t)i16_y, (uint16_t)i16_r, color);

    while(x < y)
    {
	if(f >= 0)
	{
            y--;
            ddF_y += 2;
            f += ddF_y;
	}
	x++;
	ddF_x += 2;
	f += ddF_x;

        /* Draw horizontal line from center. */
        /* board_Draw_Y_Line(Xpos + x, Ypos - y, 2*y, color);*/
        i16_x = (int16_t)Xpos + x;
        i16_y = (int16_t)Ypos - y;
        i16_r = 2*y;     
        Y_Line((uint16_t)i16_x, (uint16_t)i16_y, (uint16_t)i16_r, color);   
	
        /* board_Draw_Y_Line(Xpos - x, Ypos - y, 2*y, color); */
        i16_x = (int16_t)Xpos - x;
        i16_y = (int16_t)Ypos - y;
        i16_r = 2*y;     
        Y_Line((uint16_t)i16_x, (uint16_t)i16_y, (uint16_t)i16_r, color);   
	
        /* draw horizontal line fron up and down. */
        /* board_Draw_Y_Line(Xpos + y, Ypos - x, 2*x, color); */
        i16_x = (int16_t)Xpos + y;
        i16_y = (int16_t)Ypos - x;
        i16_r = 2*x;     
        Y_Line((uint16_t)i16_x, (uint16_t)i16_y, (uint16_t)i16_r, color);   
	
        /* board_Draw_Y_Line(Xpos - y, Ypos - x, 2*x, color); */
        i16_x = (int16_t)Xpos - y;
        i16_y = (int16_t)Ypos - x;
        i16_r = 2*x;     
        Y_Line((uint16_t)i16_x, (uint16_t)i16_y, (uint16_t)i16_r, color);   
    }
    Circle(Xpos, Ypos, radius,color);
};











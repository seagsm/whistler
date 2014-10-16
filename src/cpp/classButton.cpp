

#include "classButton.h"

/* floatValue = static_cast<float>(intValue); */

Button::Button(void)
{
 
};

void Button::Initialization(
                                Lcd* lcd,
                                uint16_t Xpos,
                                uint16_t Ypos,
                                uint16_t height,
                                uint16_t width,
                                const char* Text,
                                void (*mOnTouch)(void),
                                void (*mOnUnTouch)(void)
                            )
{       /* Fill button structure. */
    buttonLcd = lcd;
    button.button_Xpos                 = Xpos;
    button.button_Ypos                 = Ypos;
    button.button_Heigth               = height;
    button.button_Width                = width;
    button.button_Border_Color         = sys_Gray;
    button.button_Color                = sys_Gray_1;
    button.button_Pressed_Color        = sys_DarkGray;
    button.button_UnPressed_Color      = sys_Gray_2;
    button.button_Border_Width         = 2U;
    button.button_Text.Text            = Text;
    button.button_Text.TextRotate      = 3U;
    button.button_Text.Text_Color      = sys_Black;
    button.button_Text.bg_Text_Color   = button.button_Color;
    button.fOnTouch                    = mOnTouch;   /* Pointer to process function. */
    button.fOnUntouch                  = mOnUnTouch; /* Pointer to process function. */

    buttonLcd = lcd;
    buttonDrawTool.Initialization(buttonLcd);
    
    
};

void Button::OnDraw(uint8_t condition)
{
    uint16_t x=0U, y=0U, h=0U, w=0U, bw=0U, tw=0U, counter=0U;

    x = button.button_Xpos;
    y = button.button_Ypos;
    h = button.button_Heigth;
    w = button.button_Width;
    bw= button.button_Border_Width;

    tw= (uint16_t)strlen((char const*)button.button_Text.Text);
    tw = tw * 8U;
    
    /* Drawing button border. */
    while(counter < bw)
    {
        buttonDrawTool.Rect(x + counter, y + counter, h - (uint16_t)(counter<<1), w -(uint16_t)(counter<<1), button.button_Border_Color);
        counter++;
    }
  
    /* Drawing button filled inside border. */
    buttonDrawTool.FilledRect(x + counter, y + counter, h - (uint16_t)(counter<<1), w -(uint16_t)(counter<<1), button.button_Color);
    
    /* Draw press unpress border. */
    PressOnOffDraw(condition);
  
    /* Print button text to center of button.*/
    GUI_Text_Rotated(
                        x+(h/2U)-8U,      /* Calculating button to text midle X position. */
                        y+(w/2U)-(tw/2U), /* Calculating button to text midle Y position. */
           (const char*)button.button_Text.Text,
                        button.button_Text.Text_Color,
                        button.button_Text.bg_Text_Color,
                        button.button_Text.TextRotate
                    );    
};

void Button::PressOnOffDraw(uint8_t condition)
{
    uint16_t x = 0U, y = 0U, h = 0U, w = 0U, bw = 0U, counter = 0U;
    GUI_COLOR color;

    x = button.button_Xpos;
    y = button.button_Ypos;
    h = button.button_Heigth;
    w = button.button_Width;
    bw= button.button_Border_Width;
    x = x + bw;
    y = y + bw;
    h = h - 2U * bw;
    w = w - 2U * bw;

    if(condition == 0U)
    {
        color = button.button_UnPressed_Color;
        button.button_Touch.Pressed = 0U;
        button.button_Touch.Press_Hold_Counter = 0U;
        button.button_Touch.JustReleased = 1U;
        button.button_Touch.Changed = 1U;
        button.button_Touch.Press_JitterCounter = 0U;
    }
    else
    {
        color = button.button_Pressed_Color;
        button.button_Touch.Pressed = 1U;
        button.button_Touch.JustPressed =1U;
        button.button_Touch.Press_JitterCounter = 0U;
        button.button_Touch.Changed = 1U;
    }
  
    /* Drawing button border. */
    while(counter < bw)
    {
        buttonDrawTool.Rect(x + counter, y + counter, h - (uint16_t)(counter<<1), w - (uint16_t)(counter<<1), color);
        counter++;
    }  
};

void Button::OnTouch(uint16_t Xpos,uint16_t Ypos)
{
     uint8_t result;
    
    /* Read Press button state. */
    result = (uint8_t)button.button_Touch.Pressed;
    
    OnTouchRead(Xpos,Ypos);
  
    if( result != button.button_Touch.Pressed)
    {
        PressOnOffDraw((uint8_t)button.button_Touch.Pressed);
        if(button.fOnTouch != 0U)
        {
            button.fOnTouch();
        }
    }
};


void Button::OnTouchRead(uint16_t Xpos,uint16_t Ypos)
{
    int a,b,c,d;
    
    a = ( button.button_Xpos <= Xpos);
    b = ( Xpos <= (button.button_Xpos + button.button_Heigth));
    c = ( button.button_Ypos <= Ypos);
    d = (Ypos <= (button.button_Ypos + button.button_Width));

    if(a&&b&&c&&d)
    {
        button.button_Touch.Press_JitterCounter++;
        button.button_Touch.Press_Hold_Counter++;
        button.button_Touch.UnPress_Counter = 0U;
        if(button.button_Touch.Press_JitterCounter >= 3U)
        {
            button.button_Touch.Pressed = 1U;
        }
    }
    else
    {
        button.button_Touch.Press_JitterCounter = 0U;
        button.button_Touch.UnPress_Counter++;
        
        if(button.button_Touch.UnPress_Counter > 100U)
        {
            button.button_Touch.Pressed = 0U;
            button.button_Touch.UnPress_Counter  = 0U;
        }
    }
};








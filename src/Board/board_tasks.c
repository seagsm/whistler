
#include "board_tasks.h"


const portTickType xDelay = 100 / portTICK_RATE_MS;



/* Task 1. */
void vTaskLED1(void *pvParameters)
{
    uint8_t text_buf[20];
    while(1)
    {
        sprintf((char*)text_buf,"                ");
        GUI_Text_Rotated(180, 50, text_buf,BG_SYSTEM_TEXT_COLOR, BG_SYSTEM_SCREEN_COLOR,3);
        sprintf((char*)text_buf,"Xpos=%d",touch_Xpos);
        GUI_Text_Rotated(180, 10, text_buf,BG_SYSTEM_TEXT_COLOR, BG_SYSTEM_SCREEN_COLOR,3);
        sprintf((char*)text_buf,"                ");
        GUI_Text_Rotated(200, 50, text_buf,BG_SYSTEM_TEXT_COLOR, BG_SYSTEM_SCREEN_COLOR,3);
        sprintf((char*)text_buf,"Ypos=%d",touch_Ypos);
        GUI_Text_Rotated(200, 10, text_buf,BG_SYSTEM_TEXT_COLOR, BG_SYSTEM_SCREEN_COLOR,3);
        vTaskDelay( xDelay );
    }
}

/* Task 2. */
void vTaskLED2(void *pvParameters) 
{
    while (1)
    {
        getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
        //touch_Xpos = display.x;
        //touch_Ypos = display.y;
        //TP_DrawPoint(display.x,display.y);
        //TP_DrawPoint(display.x,display.y+1);
        //TP_DrawPoint(display.x+1,display.y+1);
        //TP_DrawPoint(display.x+1,display.y);
        //printf("This is task 2\n");
        board_GUI_OnTouch(display.x,display.y);
        taskYIELD();
    }
}
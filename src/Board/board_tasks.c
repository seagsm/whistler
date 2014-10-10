
#include "board_tasks.h"


const portTickType xDelay = 100 / portTICK_RATE_MS;

/* Task 1. */
void vTaskSystem(void *pvParameters)
{
    uint8_t u8_state = 0;
    while(1)
    {  
        switch(u8_state)
        {
            case 0:
                LCD_Initializtion();
                break;
            case 1:
                LCD_BackLight_Init();
                break;
            case 2:    
                delay_init();
                break;
            case 3:    
                TP_Init();
                break;
            case 4:    
                LCD_BackLight( 99);/*Brightness in persentages. */
                break;
            case 5:
                LCD_Clear(Blue);
                board_screens_draw_start_screen();
                delay_us(3000000);
                break;
            case 6:
                TouchPanel_Horizontal_Calibrate("Touch crosshair to calibrate",White, Blue, Blue);
                break;
            case 7:
                board_GUI_Init();
                break;
            default:
                break;  
        }
        if(u8_state < 8)
        {
            u8_state++;
        }
        else
        {  
          
          
          
            taskYIELD();
        }    
    }
    vTaskDelete(NULL);        

}


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
        taskYIELD();
    }
    vTaskDelete(NULL);        
}

/* Task 2. */
void vTaskLED2(void *pvParameters) 
{
    while(1)
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
    vTaskDelete(NULL);        
}
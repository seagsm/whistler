
#include "board_screens.h"





void board_screens_draw_start_screen(void)
{


    GUI_Text_Rotated(180, 50, "START SCREEN",White, Blue,3);
    LCD_DrawLine(0, 0, 100, 100,White);
}


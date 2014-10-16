
/* Code style rules:                */
/* 1. Code should be an ideal.      */
/* 2. An ideal code does not exist. */
/*                 Vadym Volokitin. */

/* Inc printer stepper motor driver controller for Epson.*/

#include "main.h"
#include "sensor.h"

int main( void)
{
    Coordinate touchscreenPosition;
     
    /* Create and initialize LCD object. */
    Lcd boardLcd(99U, Blue);

    /* Create touch screen LCD object. */
    TouchScreen touchscreenLcd;
    
    /* Create interface element. */
    Button button_1;
    Draw   drawTool;

    gv_board_sys_tick_init();
    /* Init interrupt priority group. */
    NVIC_init();
    /*TODO: It should be moved to suitable place. */
    __enable_irq();    
    
    touchscreenLcd.Initialize();
    touchscreenLcd.mTouchPanel_Horizontal_Calibrate("Touch crosshair to calibrate", White, Blue, Blue);     
    
    button_1.Initialization(&boardLcd, 10, 10, 30, 73,"ON",0,0);    
    drawTool.Initialization(&boardLcd);
    
    button_1.OnDraw(0U);
    drawTool.FullCircle(100,50,20,Green);
    drawTool.X_Line( 60, 80, 50, Red);
    
    
    while(1U)
    {
        touchscreenLcd.mgetDisplayPoint( &touchscreenPosition);
        button_1.OnTouch(touchscreenPosition.x, touchscreenPosition.y);

        
    }
}












#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
uint32_t assert_failed(uint8_t* file, uint32_t line)
{
    /*  User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1U)
    {
        return(1U);
    }
}
#endif


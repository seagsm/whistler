#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f10x.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

#include "systick.h"
#include "GLCD.h"
#include "TouchPanel.h"

#include <stdio.h>
#include <string.h>
#include <math.h>


#include "board_GUI_primitives.h"
#include "board_GUI.h"
#include "board_tasks.h"
#include "board_screens.h"
#include "board_sys_tick.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */










#define M_PI 3.1415
#define SIGNAL_BUFFER_SIZE  400
#define SIGNAL_SAMPLES 96

#endif
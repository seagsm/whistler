#ifndef MAIN_H
#define MAIN_H 1

extern "C" {
#include "stm32f10x.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <misc.h>

#include "board_system_type.h"
#include "board_sys_tick.h"

#include "TouchPanel.h"
#include "GLCD.h"
  //#include "LcdHal.h"
  //#include "uiframework.h"

}

#include "classLcd.h"
#include "classTouchScreen.h"
#include "classButton.h"
#include "classDraw.h"

#include "ButtonEvents.h"

/* Checking for libs parameters. */
/* void assert_failed(uint8_t* file, uint32_t line); */

extern       Draw   drawTool;



#endif
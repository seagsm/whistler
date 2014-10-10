#ifndef BOARD_TASKS_H
#define BOARD_TASKS_H 1


#include "FreeRTOS.h"
#include "task.h"

#include "GLCD.h"
#include "TouchPanel.h"

#include "board_GUI_primitives.h"
#include "board_GUI.h"

#include "systick.h"
#include <stdio.h>



void vTaskLED2(void *pvParameters);
void vTaskLED1(void *pvParameters);

#endif
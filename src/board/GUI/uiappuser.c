/*******************************************************************************
* @file		: uiappuser.c
* @author	: IMS Systems LAB & Technical Marketing
* @version	: V1.0.0
* @date		: 6-June-2012
* @brief	: Event Handlers
*******************************************************************************/
/* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
*******************************************************************************
*/ 
/*includes----------------------------------------------------------------------*/
#include "uiframework.h"
#include "uiappuser.h"
#include "LcdHal.h"
/** @addtogroup Embedded_GUI_Example
  * @{
  */

/** @defgroup User_Functions
  * @brief Contains event handlers for controls to execute the user code
  * @{
  */

/* External variables ----------------------------------------------------------*/
/* Private typedef -------------------------------------------------------------*/
/* Private defines -------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------*/
/* Private functions -----------------------------------------------------------*/

/**
  * @brief Slidebar dragged event handler
  * @note This function is called when user drags the slideBar BacklightBar.
  * @param None
  * @retval None
  */
void TestScreen_BacklightBar_ValueChanged()
{
}
/**
  * @brief Switch toggle event handler
  * @note This function is called when user toggles the switch BacklightSwitch
  * @param None
  * @retval None
  */
void TestScreen_BacklightSwitch_Toggle()
{
}
/**
  * @brief Option button selected event handler
  * @note This function gets called when user selects option DesignOptionButton01
  * @param None
  * @retval None
  */
void TestScreen_DesignOptionButton01_Selected()
{
}
/**
  * @brief Option button selected event handler
  * @note This function gets called when user selects option DesignOptionButton02
  * @param None
  * @retval None
  */
void TestScreen_DesignOptionButton02_Selected()
{
}
/**
  * @brief Option button selected event handler
  * @note This function gets called when user selects option DesignOptionButton03
  * @param None
  * @retval None
  */
void TestScreen_DesignOptionButton03_Selected()
{
}
/**
  * @brief Button click event handler
  * @note This function gets called when user clicks  DesignButton02.
  * @param None
  * @retval None
  */
void TestScreen_DesignButton02_Click()
{
	Show_graphtest();
}
/**
  * @brief Button click event handler
  * @note This function gets called when user clicks  DesignButton03.
  * @param None
  * @retval None
  */
void TestScreen_DesignButton03_Click()
{
}

/**
  * @brief Button click event handler
  * @note This function gets called when user clicks  CloseBtn.
  * @param None
  * @retval None
  */
void graphtest_CloseBtn_Click()
{
}

/**
  * @}
  */

/**
  * @}
  */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/ 


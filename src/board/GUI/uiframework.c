/*******************************************************************************
* @file		: uiframework.c
* @author	: IMS Systems LAB & Technical Marketing
* @version	: V1.0.0
* @date		: 6-June-2012
* @brief	: Screen Code
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
#include "float.h"
#include "LcdHal.h"
#include "TscHal.h"
#include "pictures.h"

GL_Page_TypeDef *CurrentScreen;

/** @addtogroup Embedded_GUI_Example
  * @{
  */

/** @defgroup UI_Framework
  * @brief This file contains functions to configure the screens and controls
  * @{
  */

/* External variables ----------------------------------------------------------*/
/* Private typedef -------------------------------------------------------------*/
/* Private defines -------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
GL_Page_TypeDef TestScreen;
GL_Page_TypeDef graphtest;
/* Private function prototypes -------------------------------------------------*/
/* Private functions -----------------------------------------------------------*/
/**
  * @brief Create and configure screen object
  * @note This control creates the screen object and configures contained controls
  * @param None
  * @retval None
  */
void Create_TestScreen() 
{ 
	GL_RadioButtonGrp_TypeDef* Group01=NewRadioButtonGrp(1);

	GL_RadioButtonGrp_TypeDef* Colors=NewRadioButtonGrp(2);


	GL_PageControls_TypeDef* DesignLabel02= NewLabel(3,"STM32 GUI LIB",GL_HORIZONTAL,GL_FONT_BIG,DesignLabel02Color);
	GL_PageControls_TypeDef* DesignLabel03= NewLabel(4,"on HY-32D",GL_HORIZONTAL,GL_FONT_BIG,DesignLabel03Color);
	GL_PageControls_TypeDef* DesignLabel04= NewLabel(5,"by DarkEN_ANGEL",GL_HORIZONTAL,GL_FONT_SMALL,DesignLabel04Color);
	GL_PageControls_TypeDef* BacklightBar= NewSlidebar(6,"Backlight",GL_HORIZONTAL,TestScreen_BacklightBar_ValueChanged);
	GL_PageControls_TypeDef* BacklightSwitch= NewSwitch(7,"On/Off","",TestScreen_BacklightSwitch_Toggle);
	GL_PageControls_TypeDef* DesignLabel01= NewLabel(8,"COLOR",GL_HORIZONTAL,GL_FONT_SMALL,DesignLabel01Color);
	GL_PageControls_TypeDef* DesignButton02= NewButton(9,"BTN1",TestScreen_DesignButton02_Click);
	GL_PageControls_TypeDef* DesignButton03= NewButton(10,"BTN2",TestScreen_DesignButton03_Click);
	GL_PageControls_TypeDef* DesignOptionButton01 = RADIO_BUTTON_ADD(Colors,"Red",TestScreen_DesignOptionButton01_Selected);
	GL_PageControls_TypeDef* DesignOptionButton02 = RADIO_BUTTON_ADD(Colors,"Green",TestScreen_DesignOptionButton02_Selected);
	GL_PageControls_TypeDef* DesignOptionButton03 = RADIO_BUTTON_ADD(Colors,"Blue",TestScreen_DesignOptionButton03_Selected);
	Create_PageObj( &TestScreen ); 
	AddPageControlObj(263,8,DesignLabel02,&TestScreen);
	AddPageControlObj(231,32,DesignLabel03,&TestScreen);
	AddPageControlObj(219,56,DesignLabel04,&TestScreen);
	AddPageControlObj(217,200,BacklightBar,&TestScreen);
	AddPageControlObj(191,160,BacklightSwitch,&TestScreen);
	AddPageControlObj(303,80,DesignOptionButton01,&TestScreen);
	AddPageControlObj(303,104,DesignOptionButton02,&TestScreen);
	AddPageControlObj(303,128,DesignOptionButton03,&TestScreen);
	AddPageControlObj(179,104,DesignLabel01,&TestScreen);
	AddPageControlObj(55,72,DesignButton02,&TestScreen);
	AddPageControlObj(55,104,DesignButton03,&TestScreen);
}


/**
  * @brief Create and configure screen object
  * @note This control creates the screen object and configures contained controls
  * @param None
  * @retval None
  */
void Create_graphtest() 
{ 

	uint16_t DataPointsCount2=24;
	const uint16_t data_points2[] = {300, 700, 320, 650, 288, 622, 167, 512, 218, 932, 314, 804, 199, 787, 299, 923, 388, 917, 201, 753, 277, 912, 326, 799};
	GL_PageControls_TypeDef* TestGraph= NewGraphChart(11,"X","Y",(int16_t *)data_points2, DataPointsCount2, TRUE);
	GL_PageControls_TypeDef* CloseBtn= NewButton(12,"Close",graphtest_CloseBtn_Click);
	Create_PageObj( &graphtest ); 
	AddPageControlObj((uint16_t)(0), (uint16_t)(0),TestGraph,&graphtest);
	AddPageControlObj(63,208,CloseBtn,&graphtest);
}




/**
  * @brief Show screen object
  * @note This control displayed the specified screen object
  * @param None
  * @retval None
  */
void Show_TestScreen()
{ 
	if(CurrentScreen!=GL_NULL)
	{
		CurrentScreen->ShowPage(CurrentScreen, FALSE);
	}
	GL_Clear(16904);
	GL_SetTextColor(64608);
	TestScreen.ShowPage(&TestScreen, TRUE);
	CurrentScreen=&TestScreen; 
} 

/**
  * @brief Show screen object
  * @note This control displayed the specified screen object
  * @param None
  * @retval None
  */
void Show_graphtest()
{ 
	if(CurrentScreen!=GL_NULL)
	{
		CurrentScreen->ShowPage(CurrentScreen, FALSE);
	}
	GL_Clear(27469);
	GL_SetTextColor(63488);
	graphtest.ShowPage(&graphtest, TRUE);
	CurrentScreen=&graphtest; 
} 

/**
  * @brief Controls home screen
  * @param None
  * @retval None
  */
void Show_HomeScreen()
{
	Create_TestScreen();
	Create_graphtest();
	CurrentScreen = &TestScreen;
	GL_Clear(16904);
	GL_SetTextColor(64608);
	TestScreen.ShowPage(&TestScreen, TRUE);
}

/**
  * @}
  */

/**
  * @}
  */


/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/ 


/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.c
** Descriptions:		STM32 FSMC TFT操作函数库
**
**------------------------------------------------------------------------------------------------------
** Created by:			poweravr
** Created date:		2010-11-7
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h"
#include "AsciiLib.h"
#include <math.h>

/* Private variables ---------------------------------------------------------*/
static uint16_t DeviceCode;
static uint16_t TimerPeriod = 0U;
static uint16_t Channel2Pulse = 1000U;

/* Private define ------------------------------------------------------------*/
/* 使用总线方式时定义地址 */
/* 挂在不同的BANK,使用不同地址线时请自行换算地址 */
#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */


/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

  /* PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9),
     PE.13(D10), PE.14(D11), PE.15(D12) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                 GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PD.00(D2), PD.01(D3), PD.04(RD), PD.5(WR), PD.7(CS), PD.8(D13), PD.9(D14),
     PD.10(D15), PD.11(RS) PD.14(D0) PD.15(D1) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |
                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                                 GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
  /* FSMC读速度设置 */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 30U;  /* 地址建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0U;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 30U;	   /* 数据建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
  /* FSMC写速度设置 */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 2U;   /* 地址建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0U;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 2U;	   /* 数据建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00U;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static inline void LCD_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_RAM);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/* static __inline void LCD_WriteRAM_Prepare(void) */
__inline void LCD_WriteRAM_Prepare(void)
{
  LCD_REG = (unsigned short)R34;
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/* static __inline void LCD_WriteRAM(uint16_t RGB_Code) */
__inline void LCD_WriteRAM(uint16_t RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD_RAM = RGB_Code;
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
* Attention		 : None
*******************************************************************************/
static __inline uint16_t LCD_ReadRAM(void)
{
  volatile uint16_t dummy;
  /* Write 16-bit Index (then Read Reg) */
  LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD_RAM;

  return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
{
  if(DeviceCode==0x8989U)
  {
    LCD_WriteReg(0x004eU, Xpos); /* Row */
    LCD_WriteReg(0x004fU, Ypos); /* Line */
  }
  else if(DeviceCode==0x9919U)
  {
    LCD_WriteReg(0x004eU, Xpos); /* Row */
    LCD_WriteReg(0x004fU, Ypos); /* Line */
  }
  else
  {
    LCD_WriteReg(0x0020U, Xpos); /* Row */
    LCD_WriteReg(0x0021U, Ypos); /* Line */
  }
}

/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Delay(uint16_t nCount)
{
 uint16_t TimingDelay;
 while(nCount--)
   {
    for(TimingDelay=0U;TimingDelay<10000U;TimingDelay++)
    {}
   }
}

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initialization(void)
{
  LCD_Configuration();
  LCD_Delay(5U);  /* delay 50 ms */
  DeviceCode = LCD_ReadReg(0x0000U);		/* 读取屏ID	*/
  if( (DeviceCode==0x9325U) || (DeviceCode==0x9328U))	/* 不同屏驱动IC 初始化不同 */
  {
    LCD_WriteReg(0x00e7U,0x0010U);
    LCD_WriteReg(0x0000U,0x0001U);  	/* start internal osc */
    LCD_WriteReg(0x0001U,0x0100U);
    LCD_WriteReg(0x0002U,0x0700U); 	/* power on sequence */
	LCD_WriteReg(0x0003U,(1U<<12U)|(1U<<5U)|(1U<<4U)|(0U<<3U) ); 	/* importance */
    LCD_WriteReg(0x0004U,0x0000U);
    LCD_WriteReg(0x0008U,0x0207U);
    LCD_WriteReg(0x0009U,0x0000U);
    LCD_WriteReg(0x000aU,0x0000U); 	/* display setting */
    LCD_WriteReg(0x000cU,0x0001U);	/* display setting */
    LCD_WriteReg(0x000dU,0x0000U);
    LCD_WriteReg(0x000fU,0x0000U);
    /* Power On sequence */
    LCD_WriteReg(0x0010U,0x0000U);
    LCD_WriteReg(0x0011U,0x0007U);
    LCD_WriteReg(0x0012U,0x0000U);
    LCD_WriteReg(0x0013U,0x0000U);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0010U,0x1590U);
    LCD_WriteReg(0x0011U,0x0227U);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0012U,0x009cU);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0013U,0x1900U);
    LCD_WriteReg(0x0029U,0x0023U);
    LCD_WriteReg(0x002bU,0x000eU);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0020U,0x0000U);
    LCD_WriteReg(0x0021U,0x0000U);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0030U,0x0007U);
    LCD_WriteReg(0x0031U,0x0707U);
    LCD_WriteReg(0x0032U,0x0006U);
    LCD_WriteReg(0x0035U,0x0704U);
    LCD_WriteReg(0x0036U,0x1f04U);
    LCD_WriteReg(0x0037U,0x0004U);
    LCD_WriteReg(0x0038U,0x0000U);
    LCD_WriteReg(0x0039U,0x0706U);
    LCD_WriteReg(0x003cU,0x0701U);
    LCD_WriteReg(0x003dU,0x000fU);
    LCD_Delay(5U);  /* delay 50 ms */
    LCD_WriteReg(0x0050U,0x0000U);
    LCD_WriteReg(0x0051U,0x00efU);
    LCD_WriteReg(0x0052U,0x0000U);
    LCD_WriteReg(0x0053U,0x013fU);
    LCD_WriteReg(0x0060U,0xa700U);
    LCD_WriteReg(0x0061U,0x0001U);
    LCD_WriteReg(0x006aU,0x0000U);
    LCD_WriteReg(0x0080U,0x0000U);
    LCD_WriteReg(0x0081U,0x0000U);
    LCD_WriteReg(0x0082U,0x0000U);
    LCD_WriteReg(0x0083U,0x0000U);
    LCD_WriteReg(0x0084U,0x0000U);
    LCD_WriteReg(0x0085U,0x0000U);

    LCD_WriteReg(0x0090U,0x0010U);
    LCD_WriteReg(0x0092U,0x0000U);
    LCD_WriteReg(0x0093U,0x0003U);
    LCD_WriteReg(0x0095U,0x0110U);
    LCD_WriteReg(0x0097U,0x0000U);
    LCD_WriteReg(0x0098U,0x0000U);
    /* display on sequence */
    LCD_WriteReg(0x0007U,0x0133U);

    LCD_WriteReg(0x0020U,0x0000U);  /* 行首址0 */
    LCD_WriteReg(0x0021U,0x0000U);  /* 列首址0 */
  }
  else if((DeviceCode==0x9320U) || (DeviceCode==0x9300U))
  {
    LCD_WriteReg(0x00U,0x0000U);
	LCD_WriteReg(0x01U,0x0100U);	/* Driver Output Contral */
	LCD_WriteReg(0x02U,0x0700U);	/* LCD Driver Waveform Contral */
	LCD_WriteReg(0x03U,0x1018U);	/* Entry Mode Set */

	LCD_WriteReg(0x04U,0x0000U);	/* Scalling Contral */
    LCD_WriteReg(0x08U,0x0202U);	/* Display Contral */
	LCD_WriteReg(0x09U,0x0000U);	/* Display Contral 3.(0x0000) */
	LCD_WriteReg(0x0aU,0x0000U);	/* Frame Cycle Contal.(0x0000) */
    LCD_WriteReg(0x0cU,(1U<<0U));	/* Extern Display Interface Contral */
	LCD_WriteReg(0x0dU,0x0000U);	/* Frame Maker Position */
	LCD_WriteReg(0x0fU,0x0000U);	/* Extern Display Interface Contral 2. */

    LCD_Delay(10U);  /* delay 100 ms */
	LCD_WriteReg(0x07U,0x0101U);	/* Display Contral */
    LCD_Delay(10U);  /* delay 100 ms */

	LCD_WriteReg(0x10U,(1U<<12U)|(0U<<8U)|(1U<<7U)|(1U<<6U)|(0U<<4U));	/* Power Control 1.(0x16b0)	*/
	LCD_WriteReg(0x11U,0x0007U);								/* Power Control 2 */
	LCD_WriteReg(0x12U,(1U<<8U)|(1U<<4U)|(0U<<0U));				/* Power Control 3.(0x0138)	*/
	LCD_WriteReg(0x13U,0x0b00U);								/* Power Control 4 */
	LCD_WriteReg(0x29U,0x0000U);								/* Power Control 7 */

	LCD_WriteReg(0x2bU,(1U<<14U)|(1U<<4U));

	LCD_WriteReg(0x50U,0U);       /* Set X Start */
	LCD_WriteReg(0x51U,239U);	    /* Set X End */
	LCD_WriteReg(0x52U,0U);	    /* Set Y Start */
	LCD_WriteReg(0x53U,319U);	    /* Set Y End */

	LCD_WriteReg(0x60U,0x2700U);	/* Driver Output Control */
	LCD_WriteReg(0x61U,0x0001U);	/* Driver Output Control */
	LCD_WriteReg(0x6aU,0x0000U);	/* Vertical Srcoll Control */

	LCD_WriteReg(0x80U,0x0000U);	/* Display Position? Partial Display 1 */
	LCD_WriteReg(0x81U,0x0000U);	/* RAM Address Start? Partial Display 1 */
	LCD_WriteReg(0x82U,0x0000U);	/* RAM Address End-Partial Display 1 */
	LCD_WriteReg(0x83U,0x0000U);	/* Displsy Position? Partial Display 2 */
	LCD_WriteReg(0x84U,0x0000U);	/* RAM Address Start? Partial Display 2 */
	LCD_WriteReg(0x85U,0x0000U);	/* RAM Address End? Partial Display 2 */

    LCD_WriteReg(0x90U,(0U<<7U)|(16U<<0U));	/* Frame Cycle Contral.(0x0013)	*/
	LCD_WriteReg(0x92U,0x0000U);	/* Panel Interface Contral 2.(0x0000) */
	LCD_WriteReg(0x93U,0x0001U);	/* Panel Interface Contral 3. */
    LCD_WriteReg(0x95U,0x0110U);	/* Frame Cycle Contral.(0x0110)	*/
	LCD_WriteReg(0x97U,(0U<<8U));
	LCD_WriteReg(0x98U,0x0000U);	/* Frame Cycle Contral */

    LCD_WriteReg(0x07U,0x0173U);
  }
  else if(DeviceCode==0x9331U)
  {
	LCD_WriteReg(0x00E7U, 0x1014U);
	LCD_WriteReg(0x0001U, 0x0100U);   /* set SS and SM bit */
	LCD_WriteReg(0x0002U, 0x0200U);   /* set 1 line inversion */
	LCD_WriteReg(0x0003U, 0x1030U);   /* set GRAM write direction and BGR=1 */
	LCD_WriteReg(0x0008U, 0x0202U);   /* set the back porch and front porch */
        LCD_WriteReg(0x0009U, 0x0000U);   /* set non-display area refresh cycle ISC[3:0] */
	LCD_WriteReg(0x000AU, 0x0000U);   /* FMARK function */
	LCD_WriteReg(0x000CU, 0x0000U);   /* RGB interface setting */
	LCD_WriteReg(0x000DU, 0x0000U);   /* Frame marker Position */
	LCD_WriteReg(0x000FU, 0x0000U);   /* RGB interface polarity */
	/* Power On sequence */
	LCD_WriteReg(0x0010U, 0x0000U);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
	LCD_WriteReg(0x0011U, 0x0007U);   /* DC1[2:0], DC0[2:0], VC[2:0] */
	LCD_WriteReg(0x0012U, 0x0000U);   /* VREG1OUT voltage	*/
	LCD_WriteReg(0x0013U, 0x0000U);   /* VDV[4:0] for VCOM amplitude */
        LCD_Delay(20U);                  /* delay 200 ms */
	LCD_WriteReg(0x0010U, 0x1690U);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
	LCD_WriteReg(0x0011U, 0x0227U);   /* DC1[2:0], DC0[2:0], VC[2:0] */
        LCD_Delay(5U);                   /* delay 50 ms */
	LCD_WriteReg(0x0012U, 0x000CU);   /* Internal reference voltage= Vci	*/
        LCD_Delay(5U);                    /* delay 50 ms */
	LCD_WriteReg(0x0013U, 0x0800U);   /* Set VDV[4:0] for VCOM amplitude */
	LCD_WriteReg(0x0029U, 0x0011U);   /* Set VCM[5:0] for VCOMH */
	LCD_WriteReg(0x002BU, 0x000BU);   /* Set Frame Rate */
        LCD_Delay(5U);                   /* delay 50 ms */
	LCD_WriteReg(0x0020U, 0x0000U);   /* GRAM horizontal Address */
	LCD_WriteReg(0x0021U, 0x0000U);   /* GRAM Vertical Address */
	/* Adjust the Gamma Curve */
	LCD_WriteReg(0x0030U, 0x0000U);
	LCD_WriteReg(0x0031U, 0x0106U);
	LCD_WriteReg(0x0032U, 0x0000U);
	LCD_WriteReg(0x0035U, 0x0204U);
	LCD_WriteReg(0x0036U, 0x160AU);
	LCD_WriteReg(0x0037U, 0x0707U);
	LCD_WriteReg(0x0038U, 0x0106U);
	LCD_WriteReg(0x0039U, 0x0707U);
	LCD_WriteReg(0x003CU, 0x0402U);
	LCD_WriteReg(0x003DU, 0x0C0FU);
	/* Set GRAM area */
	LCD_WriteReg(0x0050U, 0x0000U);   /* Horizontal GRAM Start Address */
	LCD_WriteReg(0x0051U, 0x00EFU);   /* Horizontal GRAM End Address */
	LCD_WriteReg(0x0052U, 0x0000U);   /* Vertical GRAM Start Address */
	LCD_WriteReg(0x0053U, 0x013FU);   /* Vertical GRAM Start Address */
	LCD_WriteReg(0x0060U, 0x2700U);   /* Gate Scan Line */
	LCD_WriteReg(0x0061U, 0x0001U);   /*  NDL,VLE, REV */
	LCD_WriteReg(0x006AU, 0x0000U);   /* set scrolling line */
	/* Partial Display Control */
	LCD_WriteReg(0x0080U, 0x0000U);
	LCD_WriteReg(0x0081U, 0x0000U);
	LCD_WriteReg(0x0082U, 0x0000U);
	LCD_WriteReg(0x0083U, 0x0000U);
	LCD_WriteReg(0x0084U, 0x0000U);
	LCD_WriteReg(0x0085U, 0x0000U);
	/* Panel Control */
	LCD_WriteReg(0x0090U, 0x0010U);
	LCD_WriteReg(0x0092U, 0x0600U);
	LCD_WriteReg(0x0007U,0x0021U);
    LCD_Delay(5U);                   /* delay 50 ms */
	LCD_WriteReg(0x0007U,0x0061U);
    LCD_Delay(5U);                   /* delay 50 ms */
	LCD_WriteReg(0x0007U,0x0133U);    /* 262K color and display ON */
    LCD_Delay(5U);                   /* delay 50 ms */
  }
  else if(DeviceCode==0x9919U)
  {
    /* POWER ON &RESET DISPLAY OFF */
	LCD_WriteReg(0x28U,0x0006U);
	LCD_WriteReg(0x00U,0x0001U);
	LCD_WriteReg(0x10U,0x0000U);
	LCD_WriteReg(0x01U,0x72efU);
	LCD_WriteReg(0x02U,0x0600U);
	LCD_WriteReg(0x03U,0x6a38U);
	LCD_WriteReg(0x11U,0x6874U);
	LCD_WriteReg(0x0fU,0x0000U);    /* RAM WRITE DATA MASK */
	LCD_WriteReg(0x0bU,0x5308U);    /* RAM WRITE DATA MASK */
	LCD_WriteReg(0x0cU,0x0003U);
	LCD_WriteReg(0x0dU,0x000aU);
	LCD_WriteReg(0x0eU,0x2e00U);
	LCD_WriteReg(0x1eU,0x00beU);
	LCD_WriteReg(0x25U,0x8000U);
	LCD_WriteReg(0x26U,0x7800U);
	LCD_WriteReg(0x27U,0x0078U);
	LCD_WriteReg(0x4eU,0x0000U);
	LCD_WriteReg(0x4fU,0x0000U);
	LCD_WriteReg(0x12U,0x08d9U);
	/* Adjust the Gamma Curve */
	LCD_WriteReg(0x30U,0x0000U);
	LCD_WriteReg(0x31U,0x0104U);
	LCD_WriteReg(0x32U,0x0100U);
    LCD_WriteReg(0x33U,0x0305U);
    LCD_WriteReg(0x34U,0x0505U);
	LCD_WriteReg(0x35U,0x0305U);
    LCD_WriteReg(0x36U,0x0707U);
    LCD_WriteReg(0x37U,0x0300U);
	LCD_WriteReg(0x3aU,0x1200U);
	LCD_WriteReg(0x3bU,0x0800U);
    LCD_WriteReg(0x07U,0x0033U);
  }
  else if(DeviceCode==0x1505U)
  {
    /* second release on 3/5  ,luminance is acceptable,water wave appear during camera preview */
    LCD_WriteReg(0x0007U,0x0000U);
    LCD_Delay(5U);                   /* delay 50 ms */
    LCD_WriteReg(0x0012U,0x011CU);    /* why need to set several times?	*/
    LCD_WriteReg(0x00A4U,0x0001U);    /* NVM */
    LCD_WriteReg(0x0008U,0x000FU);
    LCD_WriteReg(0x000AU,0x0008U);
    LCD_WriteReg(0x000DU,0x0008U);
    /* GAMMA CONTROL */
    LCD_WriteReg(0x0030U,0x0707U);
    LCD_WriteReg(0x0031U,0x0007U);
    LCD_WriteReg(0x0032U,0x0603U);
    LCD_WriteReg(0x0033U,0x0700U);
    LCD_WriteReg(0x0034U,0x0202U);
    LCD_WriteReg(0x0035U,0x0002U);
    LCD_WriteReg(0x0036U,0x1F0FU);
    LCD_WriteReg(0x0037U,0x0707U);
    LCD_WriteReg(0x0038U,0x0000U);
    LCD_WriteReg(0x0039U,0x0000U);
    LCD_WriteReg(0x003AU,0x0707U);
    LCD_WriteReg(0x003BU,0x0000U);
    LCD_WriteReg(0x003CU,0x0007U);
    LCD_WriteReg(0x003DU,0x0000U);
    LCD_Delay(5U);                   /* delay 50 ms */
    LCD_WriteReg(0x0007U,0x0001U);
    LCD_WriteReg(0x0017U,0x0001U);    /* Power supply startup enable */
    LCD_Delay(5U);                   /* delay 50 ms */
    /* power control */
    LCD_WriteReg(0x0010U,0x17A0U);
    LCD_WriteReg(0x0011U,0x0217U);    /* reference voltage VC[2:0]   Vciout = 1.00*Vcivl */
    LCD_WriteReg(0x0012U,0x011EU);    /* Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?	*/
    LCD_WriteReg(0x0013U,0x0F00U);    /* VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl */
    LCD_WriteReg(0x002AU,0x0000U);
    LCD_WriteReg(0x0029U,0x000AU);    /* Vcomh = VCM1[4:0]*Vreg1out    gate source voltage?? */
    LCD_WriteReg(0x0012U,0x013EU);    /* power supply on */
    /* Coordinates Control */
    LCD_WriteReg(0x0050U,0x0000U);
    LCD_WriteReg(0x0051U,0x00EFU);
    LCD_WriteReg(0x0052U,0x0000U);
    LCD_WriteReg(0x0053U,0x013FU);
    /* Pannel Image Control */
    LCD_WriteReg(0x0060U,0x2700U);
    LCD_WriteReg(0x0061U,0x0001U);
    LCD_WriteReg(0x006AU,0x0000U);
    LCD_WriteReg(0x0080U,0x0000U);
    /* Partial Image Control */
    LCD_WriteReg(0x0081U,0x0000U);
    LCD_WriteReg(0x0082U,0x0000U);
    LCD_WriteReg(0x0083U,0x0000U);
    LCD_WriteReg(0x0084U,0x0000U);
    LCD_WriteReg(0x0085U,0x0000U);
    /* Panel Interface Control */
    LCD_WriteReg(0x0090U,0x0013U);      /* frenqucy */
    LCD_WriteReg(0x0092U,0x0300U);
    LCD_WriteReg(0x0093U,0x0005U);
    LCD_WriteReg(0x0095U,0x0000U);
    LCD_WriteReg(0x0097U,0x0000U);
    LCD_WriteReg(0x0098U,0x0000U);

    LCD_WriteReg(0x0001U,0x0100U);
    LCD_WriteReg(0x0002U,0x0700U);
    LCD_WriteReg(0x0003U,0x1030U);
    LCD_WriteReg(0x0004U,0x0000U);
    LCD_WriteReg(0x000CU,0x0000U);
    LCD_WriteReg(0x000FU,0x0000U);
    LCD_WriteReg(0x0020U,0x0000U);
    LCD_WriteReg(0x0021U,0x0000U);
    LCD_WriteReg(0x0007U,0x0021U);
    LCD_Delay(20U);                   /* delay 200 ms */
    LCD_WriteReg(0x0007U,0x0061U);
    LCD_Delay(20U);                   /* delay 200 ms */
    LCD_WriteReg(0x0007U,0x0173U);
    LCD_Delay(20U);                   /* delay 200 ms */
  }
  else if(DeviceCode==0x8989U)
  {
    LCD_WriteReg(0x0000U,0x0001U);    LCD_Delay(5U);   /* 打开晶振 */
    LCD_WriteReg(0x0003U,0xA8A4U);    LCD_Delay(5U);
    LCD_WriteReg(0x000CU,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x000DU,0x080CU);    LCD_Delay(5U);
    LCD_WriteReg(0x000EU,0x2B00U);    LCD_Delay(5U);
    LCD_WriteReg(0x001EU,0x00B0U);    LCD_Delay(5U);
    LCD_WriteReg(0x0001U,0x2B3FU);    LCD_Delay(5U);   /* 驱动输出控制320*240 0x2B3F */
    LCD_WriteReg(0x0002U,0x0600U);    LCD_Delay(5U);
    LCD_WriteReg(0x0010U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0011U,0x6070U);    LCD_Delay(5U);   /* 定义数据格式 16位色 横屏 0x6070 */
    LCD_WriteReg(0x0005U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0006U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0016U,0xEF1CU);    LCD_Delay(5U);
    LCD_WriteReg(0x0017U,0x0003U);    LCD_Delay(5U);
    LCD_WriteReg(0x0007U,0x0133U);    LCD_Delay(5U);
    LCD_WriteReg(0x000BU,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x000FU,0x0000U);    LCD_Delay(5U);   /* 扫描开始地址 */
    LCD_WriteReg(0x0041U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0042U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0048U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0049U,0x013FU);    LCD_Delay(5U);
    LCD_WriteReg(0x004AU,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x004BU,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0044U,0xEF00U);    LCD_Delay(5U);
    LCD_WriteReg(0x0045U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0046U,0x013FU);    LCD_Delay(5U);
    LCD_WriteReg(0x0030U,0x0707U);    LCD_Delay(5U);
    LCD_WriteReg(0x0031U,0x0204U);    LCD_Delay(5U);
    LCD_WriteReg(0x0032U,0x0204U);    LCD_Delay(5U);
    LCD_WriteReg(0x0033U,0x0502U);    LCD_Delay(5U);
    LCD_WriteReg(0x0034U,0x0507U);    LCD_Delay(5U);
    LCD_WriteReg(0x0035U,0x0204U);    LCD_Delay(5U);
    LCD_WriteReg(0x0036U,0x0204U);    LCD_Delay(5U);
    LCD_WriteReg(0x0037U,0x0502U);    LCD_Delay(5U);
    LCD_WriteReg(0x003AU,0x0302U);    LCD_Delay(5U);
    LCD_WriteReg(0x003BU,0x0302U);    LCD_Delay(5U);
    LCD_WriteReg(0x0023U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0024U,0x0000U);    LCD_Delay(5U);
    LCD_WriteReg(0x0025U,0x8000U);    LCD_Delay(5U);
    LCD_WriteReg(0x004fU,0U);        /* 行首址0 */
    LCD_WriteReg(0x004eU,0U);        /* 列首址0 */
  }
  LCD_Delay(5U);  /* delay 50 ms */
}

/******************************************************************************
* Function Name  : LCD_SetWindows
* Description    : Sets Windows Area.
* Input          : - StartX: Row Start Coordinate
*                  - StartY: Line Start Coordinate
*				   - xLong:
*				   - yLong:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetWindows(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong)
{
  LCD_SetCursor(xStart, yStart);
  LCD_WriteReg(0x0050U, xStart);         
  LCD_WriteReg(0x0051U, xStart + xLong - 1U);
  LCD_WriteReg(0x0052U, yStart);         
  LCD_WriteReg(0x0053U, yStart + yLong - 1U);
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
  uint32_t index=0U;
  LCD_SetCursor(0U,0U);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index = 0U; index < 76800U; index++)
  {
      LCD_RAM=Color;
  }
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : 获取指定座标的颜色值
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
    uint16_t u16_return = 0U;
    LCD_SetCursor(Xpos,Ypos);
    if( (DeviceCode==0x7783U) || (DeviceCode==0x4531U) || (DeviceCode==0x8989U) )
    {  
        u16_return =  LCD_ReadRAM();
    }      
    else
    {
        u16_return =  LCD_BGR2RGB(LCD_ReadRAM());
    }
    return(u16_return);    
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 在指定座标画点
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
    if ( ( Xpos > 239U ) ||( Ypos > 319U ) )
    {   
    }
    else
    {  
        LCD_SetCursor(Xpos,Ypos);
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(point);
    }
}

/******************************************************************************
* Function Name  : LCD_DrawPicture
* Description    : 在指定坐标范围显示一副图片
* Input          : - StartX: Row Start Coordinate
*                  - StartY: Line Start Coordinate
*				   - EndX: Row End Coordinate
*				   - EndY: Line End Coordinate
* Output         : None
* Return         : None
* Attention		 : 图片取模格式为水平扫描，16位颜色模式
*******************************************************************************/
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic[])
{
  uint16_t  i;
  LCD_SetCursor(StartX,StartY);
  LCD_WriteRAM_Prepare();
  for (i=0U; i < (EndX*EndY); i++)
  {
      LCD_WriteRAM(*pic[i]);
  }
}


/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : 画一条直线
* Input          : - x1: 行座标开始
*                  - y1: 列座标开始
*				   - x2: 行座标结束
*				   - y2: 列座标结束
*				   - bkColor: 背景颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_DrawLine(int x1, int y1, int x2, int y2,uint16_t bkColor)
{
    int x,y,dx,dy,Dx,Dy,e,i;
    
    Dx = x2-x1;
    Dy = y2-y1;

    dx=(int)fabs( (double)x2 - (double)x1 );
    dy=(int)fabs( (double)y2 - (double)y1 );
    x=x1;
    y=y1;
    if(dy>dx)
    {
        e=-dy;
        for(i = 0; i < dy; i++)
        {
            LCD_SetPoint((uint16_t)x,(uint16_t)y,bkColor);
            if(Dy >= 0)
            {
                y++;
            }      
            else
            {
                y--;
            }  
            e+=2*dx;
            if(e >= 0)
            {
                if(Dx >= 0)
                {  
                  x++;
                }  
                else
                {  
                  x--;
                }  
                e-=2*dy;
            }
        }
    }
    else
    {
        e=-dx;
        for(i=0;i<dx;i++)
        {
            LCD_SetPoint((uint16_t)x, (uint16_t)y, bkColor);
            if(Dx >= 0)
            {
                x++;
            }    
            else 
            {
                x--;
            }    
            e+=2*dy;
            
            if(e>=0)
            {
                if(Dy>=0)
                {
                    y++;
                }    
                else
                {
                    y--;
                }    
                e-=2*dx;
            }
        }
    }
}


#if ASCII_LIB > 0
/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标
*                  - Ypos: 垂直坐标
*				   - c: 显示的字符
*				   - charColor: 字符颜色
*				   - bkColor: 背景颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor)
{
  unsigned short i = 0U;
  unsigned short j = 0U;
  unsigned char buffer[16];
  unsigned char tmp_char = 0U;
  
  GetASCIICode(buffer,c);
  
  for (i=0U; i < 16U; i++)
  {
    tmp_char=buffer[i];
    for (j=0U; j<8U; j++)
    {
      if ( ((tmp_char >> 7U-j) & 0x01U) == 0x01U)
        {
          LCD_SetPoint(Xpos+j,Ypos+i,charColor);
        }
        else
        {
          LCD_SetPoint(Xpos+j,Ypos+i,bkColor);
        }
    }
  }
}


/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标
*                  - Ypos: 垂直坐标
*				   - c: 显示的字符
*				   - charColor: 字符颜色
*				   - bkColor: 背景颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutCharRotate(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor,uint8_t rotation)
{
  unsigned short i = 0U;
  unsigned short j = 0U;
  unsigned char buffer[16];
  unsigned char tmp_char = 0U;
  
  GetASCIICode(buffer,c);  /* 取字模数据 */

  switch (rotation)
  {
    case 1 :
      for (i=0U; i < 16U; i++ )
      {
        tmp_char=buffer[i];/* 15 - i -> up direction */
        for (j=0U; j < 8U; j++)
        {
          if ( ((tmp_char >> 7U-j) & 0x01U) == 0x01U)
          {
            LCD_SetPoint(Xpos+i,Ypos+j,charColor);  /* 字符颜色 */
          }
          else
          {
            LCD_SetPoint(Xpos+i,Ypos+j,bkColor);  /* 背景颜色 */
          }
        }
      }
      break;
    case 2 :
      for (i=0U; i < 16U; i++)
      {
        tmp_char=buffer[15U-i];/* 15 - i -> up direction */
        for (j=0U; j < 8U; j++)
        {
          if ( ((tmp_char >> 7U-j) & 0x01U) == 0x01U)
          {
            LCD_SetPoint(Xpos+j,Ypos+i,charColor);  /* 字符颜色 */
          }
          else
          {
            LCD_SetPoint(Xpos+j,Ypos+i,bkColor);  /* 背景颜色 */
          }
        }
      }
      break;
    case 3 :
      for (i=0U; i < 16U; i++)
      {
        tmp_char=buffer[15U- i];/* 15 - i -> up direction */
        for (j=0U;j < 8U; j++)
        {
          if ( ((tmp_char >> 7U-j) & 0x01U) == 0x01U)
          {
            LCD_SetPoint(Xpos+i,Ypos+j,charColor);  /* 字符颜色 */
          }
          else
          {
            LCD_SetPoint(Xpos+i,Ypos+j,bkColor);  /* 背景颜色 */
          }
        }
      }
      break;
    default:
      for (i=0U; i < 16U; i++)
      {
        tmp_char=buffer[15U - i];/* 15 - i -> up direction */
        for (j=0U; j < 8U; j++)
        {
          if ( ((tmp_char >> 7U-j) & 0x01U) == 0x01U)
          {
            LCD_SetPoint(Xpos+j,Ypos+i,charColor);  /* 字符颜色 */
          }
          else
          {
            LCD_SetPoint(Xpos+j,Ypos+i,bkColor);  /* 背景颜色 */
          }
        }
      }
      break;
  }/* switch*/
}
/******************************************************************************
* Function Name  : GUI_Text
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标
*				   - str: 字符串
*				   - charColor: 字符颜色
*				   - bkColor: 背景颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t str[],uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    uint32_t u32_tmp_i = 0U;

    do
    {
    
        /* TempChar=*str++; */
        TempChar = str[u32_tmp_i];
        u32_tmp_i++; 
        PutChar(Xpos,Ypos,TempChar,Color,bkColor);
        if (Xpos < 232U)
        {
            Xpos += 8U;
        }
        else if (Ypos < 304U)
        {
            Xpos  = 0U;
            Ypos += 16U;
        }
        else
        {
            Xpos = 0U;
            Ypos = 0U;
        }
    }
    while (str[u32_tmp_i]!=0U);
}

/******************************************************************************
* Function Name  : GUI_Text_Rotated
* Description    : print text to LCD in rotated dirrection(support dir 3 only)
* Input          : - Xpos:
*                  - Ypos:
*				   - str: text string
*				   - charColor: text color
*				   - bkColor: scren color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text_Rotated(uint16_t Xpos, uint16_t Ypos, const char str[],uint16_t Color, uint16_t bkColor,uint8_t rotation)
{
    uint8_t TempChar;
    uint32_t u32_tmp_i = 0U;
    
    rotation = 3U;/* This function suport dir 3 only. */
    do
    {
        /* TempChar=*str++; */
        TempChar = str[u32_tmp_i];
        u32_tmp_i++; 

        PutCharRotate(Xpos, Ypos, TempChar, Color,bkColor, rotation);

        if (Ypos < 304U)
        {
            Ypos += 8U;
        }
        else if(Xpos < 232U)
        {
            Ypos  = 0U;
            Xpos -= 16U;
        }
        else
        {
            Xpos = 0U;
            Ypos = 0U;
        }
    }
    while (str[u32_tmp_i]!=0U);
}

#endif


/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* Input          : - color: BRG 颜色值
* Output         : None
* Return         : RGB 颜色值
* Attention		 : 内部函数调用
*******************************************************************************/
uint16_t LCD_BGR2RGB(uint16_t color)
{
  uint16_t  r, g, b, rgb;

  b = ( color >> 0 )  & 0x1fU;
  g = ( color >> 5 )  & 0x3fU;
  r = ( color >> 11 ) & 0x1fU;

  rgb =  (uint16_t)(b << 11U) + (uint16_t)(g << 5U) + (uint16_t)(r << 0U);

  return( rgb );
}

/******************************************************************************
* Function Name  : LCD_BackLight_Init
* Description    : LCD_BackLight Initializtion
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_BackLight_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /*GPIOB Configuration:  PB5(TIM3 CH2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);	 /* 管脚重引射 */

  /* -----------------------------------------------------------------------
    TIM3CLK = 36 MHz, Prescaler = 35, TIM3 counter clock = 1 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 1 KHz.
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100
  ----------------------------------------------------------------------- */
   /* 频率为 1MHz TIM3 counter clock = 1MHz */
  TimerPeriod = (uint16_t) (SystemCoreClock / 1000000U) - 1U;

  /* 输出频率=时钟/(ARR+1)，所以将输出一个 1Mhz/(999 + 1 )=1kHz 频率	 */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0U;  /* 使用系统基础时钟 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0U;
  TIM_TimeBaseStructure.TIM_Prescaler = TimerPeriod;  /* 时钟预分频 */
  TIM_TimeBaseStructure.TIM_Period = 999U;   /* TIM3 ARR Register */
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* TIM_Pulse用来控制占空比，他实际影响TIM的CCR2寄存器，程序中可随时更改该寄存器的值，可随时更改占空比。占空比=(CCRx/ARR)*100。*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* 输出模式 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; /* 占空比参数 */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);    /* 使能TIM3在CCR2的预装储存器 */

  TIM_ARRPreloadConfig(TIM3, ENABLE);  /* 使能 ARR装载 */
  TIM_Cmd(TIM3, ENABLE);			   /* 使能TIM3 */

}
/******************************************************************************
* Function Name  : LCD_BackLight
* Description    : 调整液晶背光
* Input          : - percent: 背光亮度百分比
* Output         : None
* Return         : 返回1成功 返回0失败
* Attention		 : None
*******************************************************************************/
FunctionalState LCD_BackLight( uint8_t percent)
{
    FunctionalState fs_x; 
    if( percent <= 100U)
    {
        Channel2Pulse = (uint16_t)percent * 10U;
	LCD_BackLight_Init();
        fs_x = ENABLE;
    }
    else
    {
        fs_x = DISABLE;
    }  
    return(fs_x);
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


#include "ssd1289.h"
#include <math.h>

void LCD_Init(void);
void LCD_Config(void);

//static u16 Channel2Pulse = 1000;

//static gl_sFONT *LCD_Currentfonts;

#define LCD_REG              (*((volatile unsigned short *) 0x60000000))
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000))

static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

static uint16_t TimerPeriod = 0U;
static uint16_t Channel2Pulse = 1000U;

void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

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

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;//1;   /* 地址建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 10;//1;	   /* 数据建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

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
//	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* Enable FSMC Bank4_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_Setup(void)
{
  //LCD_BacklightConfig();
  LCD_CtrlLinesConfig();
  LCD_FSMCConfig();
  LCD_Init();
}

void LCD_DeInit(void)
{
  // to add some code
}

void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{
  LCD_REG = LCD_Reg;
  LCD_RAM = LCD_RegValue;
}

uint16_t LCD_ReadReg(u8 LCD_Reg)
{
  LCD_REG = LCD_Reg;
  return (LCD_RAM);
}

void LCD_WriteRAM_Prepare(void)
{
  LCD_REG = R34;
}

void LCD_WriteRAM(uint16_t RGB_Code)
{
  LCD_RAM = RGB_Code;
}

uint16_t LCD_ReadRAM(void)
{
  volatile uint16_t dummy;
  LCD_REG = R34; /* Select GRAM Reg */
  dummy = LCD_RAM;

  return LCD_RAM;
}

void LCD_Delay(u16 nCount)
{
 uint16_t TimingDelay;
 while(nCount--)
   {
    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
   }
}

void LCD_Init()
{
  LCD_Delay(5);

  GPIO_ResetBits(GPIOC, GPIO_Pin_0);
  LCD_Delay(100);
  GPIO_SetBits(GPIOC, GPIO_Pin_0);
  LCD_Delay(100);
  u16 DeviceCode = LCD_ReadReg(0x0000);
  if(DeviceCode==0x8989)
  {
    LCD_WriteReg(0x0000,0x0001);    LCD_Delay(5);
    LCD_WriteReg(0x0003,0xA8A4);    LCD_Delay(5);
    LCD_WriteReg(0x000C,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x000D,0x080C);    LCD_Delay(5);
    LCD_WriteReg(0x000E,0x2B00);    LCD_Delay(5);
    LCD_WriteReg(0x001E,0x00B0);    LCD_Delay(5);
    LCD_WriteReg(0x0001,0x2B3F);    LCD_Delay(5);
    LCD_WriteReg(0x0002,0x0600);    LCD_Delay(5);
    LCD_WriteReg(0x0010,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0011,0x6030);    LCD_Delay(5);
    LCD_WriteReg(0x0005,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0006,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0016,0xEF1C);    LCD_Delay(5);
    LCD_WriteReg(0x0017,0x0003);    LCD_Delay(5);
    LCD_WriteReg(0x0007,0x0133);    LCD_Delay(5);
    LCD_WriteReg(0x000B,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x000F,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0041,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0042,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0048,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0049,0x013F);    LCD_Delay(5);
    LCD_WriteReg(0x004A,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x004B,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0044,0xEF00);    LCD_Delay(5);
    LCD_WriteReg(0x0045,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0046,0x013F);    LCD_Delay(5);
    LCD_WriteReg(0x0030,0x0707);    LCD_Delay(5);
    LCD_WriteReg(0x0031,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0032,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0033,0x0502);    LCD_Delay(5);
    LCD_WriteReg(0x0034,0x0507);    LCD_Delay(5);
    LCD_WriteReg(0x0035,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0036,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0037,0x0502);    LCD_Delay(5);
    LCD_WriteReg(0x003A,0x0302);    LCD_Delay(5);
    LCD_WriteReg(0x003B,0x0302);    LCD_Delay(5);
    LCD_WriteReg(0x0023,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0024,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0025,0x8000);    LCD_Delay(5);
    LCD_WriteReg(0x004f,0);
    LCD_WriteReg(0x004e,0);
  }
  LCD_Delay(5);  /* delay 50 ms */
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_WriteReg(0x004e, Xpos); /* Row */
  LCD_WriteReg(0x004f, Ypos); /* Line */
}

void LCD_SetDisplayWindow(u16 Xpos, u16 Ypos, u16 Width, u16 Height)
{
  u16 x2 = Xpos - Width + 1;
  u16 y2 = Ypos - Height + 1;
  LCD_WriteReg(0x0044, (Xpos << 8) | x2);    // Source RAM address window
  LCD_WriteReg(0x0045, y2);    // Gate RAM address window
  LCD_WriteReg(0x0046, Ypos);    // Gate RAM address window
  LCD_SetCursor(Xpos, Ypos);
}

void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(239, 319, 240, 320);
  LCD_WriteReg(R11, 0x6018);
}

void LCD_SetPixel(u16 Xpos, u16 Ypos, u16 color)
{
  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(color);
}

void LCD_Clear(uint16_t Color)
{
  uint32_t index=0;
  LCD_SetDisplayWindow(239, 319, 240, 320);
  LCD_SetCursor(0,0);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)
   {
     LCD_RAM=Color;
   }
}

uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetCursor(Xpos,Ypos);
  return LCD_ReadRAM();
}

void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
  if ( ( Xpos > 239 ) ||( Ypos > 319 ) ) return;
  LCD_SetCursor(Xpos,Ypos);
  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}

void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}

void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

void LCD_DrawLine(u8 Xpos, u16 Ypos, u16 Length, u8 Direction)
{
  uint16_t newentrymod;

  if (Direction == LCD_DIR_VERTICAL)
    newentrymod = 0x6030;   // we want a 'vertical line'
  else
    newentrymod = 0x6018;   // we want a 'horizontal line'

  LCD_WriteReg(R11, newentrymod);
  LCD_SetCursor(Xpos, Ypos);
  LCD_WriteRAM_Prepare();

  while (Length--) {
    LCD_WriteRAM(TextColor);
  }
  LCD_WriteReg(R11, 0x6018);
}


void LCD_PowerOn(void)
{
  //
}

void LCD_DisplayOn(void)
{
  u16 dummy = LCD_ReadReg(0x0007);
  dummy |= 0x0033;
  LCD_WriteReg(0x0007, dummy);
  LCD_BackLight(100);
}

void LCD_DisplayOff(void)
{
  u16 dummy = LCD_ReadReg(0x0007);
  dummy &= ~0x0033;
  LCD_WriteReg(0x0007, dummy);
  LCD_BackLight(0);
}

/*void LCD_BacklightConfig(void)
{

}

FunctionalState LCD_BackLight(uint8_t percent)
{
  if( percent > 0 && percent <= 100)
  {
    return ENABLE;
  }
  else
    return DISABLE;
}*/

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

void LCD_BackLight( uint8_t percent)
{

    if( percent <= 100U)
    {
        Channel2Pulse = (uint16_t)percent * 10U;
	    LCD_BackLight_Init();
    }
}

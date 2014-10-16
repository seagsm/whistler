#include "TscHal.h"
#include "touchscreen.h"

extern __IO uint32_t u32_TSXCoordinate;
extern __IO uint32_t u32_TSYCoordinate;

extern uint32_t TSC_Value_X;
extern uint32_t TSC_Value_Y;

extern uint32_t EndAddr;
extern uint32_t CalibrationAddr;
extern __IO uint8_t calibration_done;
extern __IO uint8_t touch_done;

/* Private function prototypes -----------------------------------------------*/
extern void GL_Delay(uint32_t nTime);

#if TOUCH_SCREEN_CAPABILITY
/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
static void TSC_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

#if 0
  /* TP_CS pc6*/
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* TP_IRQ Pc4*/
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif

  /* TP_CS */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
  /* TP_IRQ */
  GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  TP_CS(1);
}

void TSC_NVIC_Configuration(void)
{
  //NVIC_SetPriority(EXTI4_IRQn, 10);
  //NVIC_EnableIRQ(EXTI4_IRQn);
  NVIC_SetPriority(EXTI15_10_IRQn, 10);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void TSC_EXTI_Configuration(void)
{
  EXTI_DeInit();
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource13);
  EXTI_InitTypeDef EXTI_InitStruct;
  //EXTI_InitStruct.EXTI_Line = EXTI_Line4;
  EXTI_InitStruct.EXTI_Line = EXTI_Line13;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStruct);
  //EXTI_ClearITPendingBit(EXTI_Line4);
  EXTI_ClearITPendingBit(EXTI_Line13);

}

#endif

void TSC_Init(void)
{
#if TOUCH_SCREEN_CAPABILITY
  SPI_InitTypeDef  SPI_InitStructure;

  TSC_GPIO_Configuration();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  SPI_Cmd(SPI1, ENABLE);

  TSC_EXTI_Configuration();
  TSC_NVIC_Configuration();
#endif
}

/**
  * @brief  Write Data in data buffer to Flash.
  * @param  FlashFree_Address: Page address
  * @param  Data: pointer to data buffer
  * @param  Size: data buffer size in bytes
  * @retval FLASH programming status
  */
FLASH_Status TSC_WriteDataToNVM(uint32_t FlashFree_Address, int32_t *Data, uint32_t Size)
{
  FLASH_Status TSC_FlashStatus = FLASH_COMPLETE;

#if TOUCH_SCREEN_CAPABILITY

  uint32_t words = (Size/sizeof(uint32_t)) + ((Size%sizeof(uint32_t))?1:0);
  uint32_t index = 0;
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();
  /* Clear All pending flags */
  FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  /* Erase Last Flash Page */
  TSC_FlashStatus = FLASH_ErasePage( FlashFree_Address );

  for(index = 0; index < words; index++)
  {
     /* Writing calibration parameters to the Flash Memory */
    TSC_FlashStatus = FLASH_ProgramWord( FlashFree_Address, Data[index]);
    /* Increasing Flash Memory Page Address */
    FlashFree_Address = FlashFree_Address + 4;
  }
#endif
  return TSC_FlashStatus;
}



/**
  * @brief  Set the last Flash Memory address
  * @param  address: Pointer to the penultimate memory page
  * @retval None
  */
void Set_LastFlashMemoryAddress( uint32_t address)
{
  EndAddr = address;
  /* Calculate the address of the Penultimate Flash Memory Page, where the calibration parameters will be saved. */
  CalibrationAddr = (uint32_t)(EndAddr - 0x800);
}

static void DelayUS(vu32 cnt)
{
  uint16_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12;
     while (us--)
     {
       ;
     }
  }
}

static void WR_CMD (uint8_t cmd)
{
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI1,cmd);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  SPI_I2S_ReceiveData(SPI1);
}

static int RD_AD(void)
{
  unsigned short buf,temp;
  /* Wait for SPI1 Tx buffer empty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI1 data */
  SPI_I2S_SendData(SPI1,0x0000);
  /* Wait for SPI3 data reception */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI1 received data */
  temp=SPI_I2S_ReceiveData(SPI1);
  buf=temp<<8;
  DelayUS(1);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI1 data */
  SPI_I2S_SendData(SPI1,0x0000);
  /* Wait for SPI1 data reception */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI1 received data */
  temp=SPI_I2S_ReceiveData(SPI1);
  buf |= temp;
  buf>>=3;
  buf&=0xfff;
  return buf;
}


int Read_X(void)
{
  int i;
  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHX);
  DelayUS(1);
  i=RD_AD();
  TP_CS(1);
  return i;
}

int Read_Y(void)
{
  int i;
  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHY);
  DelayUS(1);
  i=RD_AD();
  TP_CS(1);
  return i;
}


void TP_GetAdXY(u32 *x, u32 *y)
{
  int adx,ady;
  adx=Read_X();
  DelayUS(1);
  ady=Read_Y();
  *x=(u32)adx;
  *y=(u32)ady;
}


//void EXTI4_IRQHandler(void)
//{
//  if (EXTI_GetITStatus(EXTI_Line4) != RESET)
//  {
//    TSC_Read();
//    EXTI_ClearITPendingBit(EXTI_Line4);
//  }
//}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    TSC_Read();
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
}






void TSC_Read(void)
{
#if TOUCH_SCREEN_CAPABILITY
  if (TP_IRQ != SET)
  {
    TSC_Value_X = 0x00;
    TSC_Value_Y = 0x00;

    TP_GetAdXY(&TSC_Value_X, &TSC_Value_Y);

    u32_TSXCoordinate = getDisplayCoordinateX( TSC_Value_X, TSC_Value_Y );
    u32_TSYCoordinate = getDisplayCoordinateY( TSC_Value_X, TSC_Value_Y );

    touch_done = 1;
  }
#endif
}

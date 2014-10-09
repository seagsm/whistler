/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			WM8731.C
** Descriptions:		WM8731����
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2010-11-17
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
#include "WM8731.h"
#include "SCCB.h"
#define MASTER_ON 1

#if 0
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : EEPROM�ܽ�����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  /* Configure I2C2 pins: PB10->SCL and PB11->SDA */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : I2C_delay
* Description    : �ӳ�ʱ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_delay(void)
{
   uint8_t i=5; /* ��������Ż��ٶ�,��������͵�5����д�� */
   while(i)
   {
     i--;
   }
}

/*******************************************************************************
* Function Name  : I2C_Start
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static FunctionalState I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	I2C_delay();
	if(SDA_read) return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	SDA_L;
	I2C_delay();
	return ENABLE;
}

/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : None
* Input          : None
* Output         : None
* Return         : ����Ϊ:=1��ACK,=0��ACK
* Attention		 : None
*******************************************************************************/
static FunctionalState I2C_WaitAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

 /*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : ���ݴӸ�λ����λ
* Input          : - SendByte: ���͵�����
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_SendByte(uint8_t SendByte)
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;
      else
        SDA_L;
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}

#endif

/*******************************************************************************
* Function Name  : WM8731_Send
* Description    : д����
* Input          : - SendByte: ��д������
*           	   - WriteAddress: ��д���ַ
*                  - DeviceAddress: ��������
* Output         : None
* Return         : ����Ϊ:=1�ɹ�д��,=0ʧ��
* Attention		 : None
*******************************************************************************/
static FunctionalState WM8731_Send(uint8_t WriteAddress, uint8_t SendByte , uint8_t DeviceAddress)
{
    if(!I2C_Start())return DISABLE;
    I2C_SendByte(DeviceAddress & 0xFF);        /* ����������ַ */
    if(!I2C_WaitAck()){I2C_Stop(); return DISABLE;}
    I2C_SendByte( WriteAddress );
    I2C_WaitAck();
    I2C_SendByte(SendByte);
    I2C_WaitAck();
    I2C_Stop();
	/* ע�⣺��Ϊ����Ҫ�ȴ�д�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)	*/
    /* Systick_Delay_1ms(10); */
    return ENABLE;
}

/*******************************************************************************
* Function Name  : codec_send
* Description    : None
* Input          : - s_data: ��д������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void codec_send(uint16_t s_data)
{
  while( !WM8731_Send( (s_data >> 8) & 0xFF ,s_data & 0xFF ,  ADDR_WM8731S) );
}

/*******************************************************************************
* Function Name  : WM8731_Init
* Description    : WM8731��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void WM8731_Init(void)
{
  /*
  Bits[15:9] control address bits.
  Bits[8:0]  control data bits.
  */
   I2C_Configuration();
   codec_send( RESET_REGISTER ); /* reset */
   codec_send( LEFT_HEADPHONE_OUT  | (1<<8) | (1<<7) | (0x79<<0) ); /* headphone out	*/
   codec_send( RIGHT_HEADPHONE_OUT | (1<<8) | (1<<7) | (0x79<<0) );
   codec_send( ANALOGUE_AUDIO_PATH_CONTROL | DAC_SEL | MUTEMIC );
   //codec_send( DIGITAL_AUDIO_PATH_CONTRL | (2<<1) ); /* 44.1kHz	*/
   codec_send( DIGITAL_AUDIO_PATH_CONTRL | (0<<1) ); /* correction disabled	*/

#if MASTER_ON  /* Set to master mode.*/
   codec_send( DIGITAL_AUDIO_INTERFACE_FORMAT | (1<<6)|(0<<3)|(0<<2)|(2<<0) ); /* set 16 bits format	*/
#endif
   codec_send( POWER_DOWN_COTROL | (0<<4) | (0<<3) | (1<<2) | (1<<1) | (1<<0));	/* powerup */

#if MASTER_ON
   codec_send( SAMPLING_CONTROL | NORMAL_MODE | BOSR_NORMAL_256FS |(0x07)<< 2 );/* 96000 sampling rate.*/
 #else
   codec_send( SAMPLING_CONTROL | NORMAL_MODE | BOSR_NORMAL_256FS );
#endif
   //codec_send( ACTIVE_CONTROL | ACTIVE );
}

void WM8731_Active(void)
{
   codec_send( ACTIVE_CONTROL | ACTIVE );
}

/*******************************************************************************
* Function Name  : WM8731_Vol
* Description    : WM8731��������
* Input          : - vol: 47-127  47���� 127�������
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void WM8731_Vol(uint8_t vol)
{
   codec_send(LEFT_HEADPHONE_OUT  | (1<<8) | (1<<7) | (vol<<0));
   codec_send(RIGHT_HEADPHONE_OUT | (1<<8) | (1<<7) | (vol<<0));

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

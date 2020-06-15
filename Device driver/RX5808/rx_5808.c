/***********************************************************************
*Copyright (C) 2020 ���ڹ����Ƽ����޹�˾�з���
* All rights reserved.

*File name: �� rx_5808.c
*Version:    ��0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              rx_5808ģ�������������
*History:  

************************************************************************/

#include "rx_5808.h"

static void sendBit(UINT8 value);
static void sendBits(UINT32 bits, UINT8 count);
static void sendRegister(UINT8 address, UINT32 tempdata);

#define SPI_ADDRESS_SYNTH_A 0x01
#define SPI_ADDRESS_POWER   0x0A

#define Write 1
#define Read  0

/**************************************************************************
              ��������Ĵ����ı�Ƶ��
              ����SPI�����������е�25λ��������4λ��ַλ��1λ��/д����λ��20λ����λ��ɡ����ݴ���������LSB��
			  |A0 A1 A2 A3 | ��R/W�� |    D0~D20   |
			  |    ��ַ	   |  ��дλ |	20������λ |
			  4 bits - address
              1 bit  - read/write enable
              20 bits - data

              Ƶ�ʺϳɼĴ�����ַ (Synth Register B) is 0x1
              ���ݸ�ʽ (LSB)
			  Ƶ�ʺϳ���Ĭ��Ƶ����5865MHz
              7 bits - A counter divider ratio
              1 bit - seperator
              12 bits - N counter divder ratio

              N��A�ļ��㹫ʽ
              F_lo = 2 * (N * 32 + A) * (F_osc / R)
				where:
              F_osc = 8 Mhz
              R = 8

              Refer to RTC6715 datasheet for further details.
***************************************************************************/


/***********************************************************
*name:    setSynthRegisterB(UINT value)
*input:   UINT value   //A/N 
*output:  non
*update:  2020-06-15

*description:   ��Synthesizer Register B�Ĵ���д��A/Nֵ���ı�Ƶ��
*history:
************************************************************/
void setSynthRegisterB(UINT value) 
{
    sendRegister(SPI_ADDRESS_SYNTH_A, value);
}

/***********************************************************
*name:    setPowerDownRegister(UINT32 value) 
*input:   UINT32 value 
*output:  non
*update:  2020-06-15

*description:   ���ù��ʼĴ���
*history:
************************************************************/
void setPowerDownRegister(UINT32 value) 
{
	sendRegister(SPI_ADDRESS_POWER, value);
}


/***********************************************************
*name:    sendRegister(UINT8 address, UINT32 tempdata) 
*input:   UINT8 address:�Ĵ�����ַ   UINT32 tempdata:Ҫд���ֵ
*output:  non
*update:  2020-06-15

*description:   ��ָ���Ĵ���дֵ
*history:
************************************************************/
static void sendRegister(UINT8 address, UINT32 tempdata)
{
	spi_cs_low();
	sendBits(address, 4);
	sendBit(Write);	
	sendBits(tempdata,20);
	spi_cs_high();
	spi_clk_low();
	spi_sda_low();
}

/***********************************************************
*name:    sendBits(UINT32 bits,UINT8 count)
*input:   UINT32 bits:Ҫд���ֵ   UINT8 count:���ݵĳ���
*output:  non
*update:  2020-06-15

*description:   д����Bit
*history:
************************************************************/
static void sendBits(UINT32 bits,UINT8 count)
{
	UINT8 i;
	for(i = 0 ; i < count; i++)
	{
		sendBit(bits & 0x01);
		bits >>= 1;
	}
}

/***********************************************************
*name:    sendBit(UINT8 value)
*input:   UINT8 value:д�뵥��Bit
*output:  non
*update:  2020-06-15

*description:   д�뵥��bit
*history:
************************************************************/
static void sendBit(UINT8 value)
{
	spi_clk_low();
	SPI_Delay(10);
	value ? spi_sda_high() : spi_sda_low();
	SPI_Delay(10);
	spi_clk_high();
	SPI_Delay(10);
	spi_clk_low();
	SPI_Delay(10);
}

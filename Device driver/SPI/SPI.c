/***********************************************************************
*Copyright (C) 2020 ���ڹ����Ƽ����޹�˾�з���
* All rights reserved.

*File name: �� SPI.c
*Version:    ��0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              SPI��������
*History:  

************************************************************************/

#include "SPI.h"

/***********************************************************
*name:    SPI_Delay()
*input:   UINT delayTime   // ȷ����ʱʱ��
*output:  non
*update:  2020-06-15

*description:   SPI ��ʱ����
*history:
************************************************************/
void SPI_Delay(UINT delayTime)
{
	while(delayTime)
	{
		delayTime = delayTime - 1;
	}
}

/***********************************************************
*name:    SPI_WriteByte(UINT8 tempdata)
*input:   UINT8 tempdata   //��Ҫ���͵�һ���ֽ�
*output:  non
*update:  2020-06-15

*description:   SPI���ͳ���
*history:
************************************************************/
void SPI_WriteByte(UINT8 tempdata)
{
	int i;
	for(i=7 ; i>=0 ; i--)
	{
		if((tempdata>>i)&0x01)
		{
			spi_sda_high();
		}
		else
		{
			spi_sda_low();
		}
		spi_clk_high();
		SPI_Delay(10);
		spi_clk_low();
		SPI_Delay(10);
	}
}

/***********************************************************
*name:    SPI_ReadByte(void)
*input:   non
*output:  UINT8 ���ؽ��յ���һ���ֽ�
*update:  2020-06-15

*description:   SPI���ͳ���
*history:
************************************************************/
UINT8 SPI_ReadByte(void)
{
	int i;
	UINT8 recv = 0;
	for(i = 7 ; i >= 0 ; i--)
	{
		spi_clk_high();
		recv = recv << 1;
		recv = recv|(spi_sda_in() ? 1 : 0);
		spi_clk_low();
	}
	return recv;
}

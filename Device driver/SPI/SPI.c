/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 SPI.c
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              SPI驱动程序。
*History:  

************************************************************************/

#include "SPI.h"

/***********************************************************
*name:    SPI_Delay()
*input:   UINT delayTime   // 确定延时时间
*output:  non
*update:  2020-06-15

*description:   SPI 延时程序
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
*input:   UINT8 tempdata   //需要发送的一个字节
*output:  non
*update:  2020-06-15

*description:   SPI发送程序
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
*output:  UINT8 返回接收到的一个字节
*update:  2020-06-15

*description:   SPI发送程序
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

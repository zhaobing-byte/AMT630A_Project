/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 rx_5808.c
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              rx_5808模块相关驱动函数
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
              发送命令到寄存器改变频率
              三线SPI序列数据序列的25位数据流由4位地址位、1位读/写控制位和20位数据位组成。数据传输首先是LSB。
			  |A0 A1 A2 A3 | （R/W） |    D0~D20   |
			  |    地址	   |  读写位 |	20个数据位 |
			  4 bits - address
              1 bit  - read/write enable
              20 bits - data

              频率合成寄存器地址 (Synth Register B) is 0x1
              数据格式 (LSB)
			  频率合成器默认频率是5865MHz
              7 bits - A counter divider ratio
              1 bit - seperator
              12 bits - N counter divder ratio

              N和A的计算公式
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

*description:   向Synthesizer Register B寄存器写入A/N值，改变频率
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

*description:   设置功率寄存器
*history:
************************************************************/
void setPowerDownRegister(UINT32 value) 
{
	sendRegister(SPI_ADDRESS_POWER, value);
}


/***********************************************************
*name:    sendRegister(UINT8 address, UINT32 tempdata) 
*input:   UINT8 address:寄存器地址   UINT32 tempdata:要写入的值
*output:  non
*update:  2020-06-15

*description:   向指定寄存器写值
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
*input:   UINT32 bits:要写入的值   UINT8 count:数据的长度
*output:  non
*update:  2020-06-15

*description:   写入多个Bit
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
*input:   UINT8 value:写入单个Bit
*output:  non
*update:  2020-06-15

*description:   写入单个bit
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

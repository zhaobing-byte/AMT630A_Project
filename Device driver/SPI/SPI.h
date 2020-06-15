/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 SPI.h
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              SPI驱动程序。
*History:  

************************************************************************/
#ifndef _SPI_H_
#define _SPI_H_

#include "DataType.h"
#include "SystemConfig.h"
#include "AMTIO_Lib_Interface.h"
//SPI 片选宏定义
#define spi_cs_high()                 hw_spi_setSEL_output(1)
#define spi_cs_low()                  hw_spi_setSEL_output(0)

//SPI 数据引脚高低置位
#define spi_sda_high()                hw_spi_setSDA_output(1)
#define spi_sda_low()                 hw_spi_setSDA_output(0)

//SPI 数据引脚输入
#define spi_sda_in()                  hw_spi_getSDA_input()

//SPI 时钟高低引脚置位
#define spi_clk_high()                hw_spi_setCLK_output(1)
#define spi_clk_low()                 hw_spi_setCLK_output(0)
              
void SPI_Delay(UINT delayTime);
void SPI_WriteByte(UINT8 tempdata);
UINT8 SPI_ReadByte(void);

#endif
/***********************************************************************
*Copyright (C) 2020 ���ڹ����Ƽ����޹�˾�з���
* All rights reserved.

*File name: �� SPI.h
*Version:    ��0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              SPI��������
*History:  

************************************************************************/
#ifndef _SPI_H_
#define _SPI_H_

#include "DataType.h"
#include "SystemConfig.h"
#include "AMTIO_Lib_Interface.h"
//SPI Ƭѡ�궨��
#define spi_cs_high()                 hw_spi_setSEL_output(1)
#define spi_cs_low()                  hw_spi_setSEL_output(0)

//SPI �������Ÿߵ���λ
#define spi_sda_high()                hw_spi_setSDA_output(1)
#define spi_sda_low()                 hw_spi_setSDA_output(0)

//SPI ������������
#define spi_sda_in()                  hw_spi_getSDA_input()

//SPI ʱ�Ӹߵ�������λ
#define spi_clk_high()                hw_spi_setCLK_output(1)
#define spi_clk_low()                 hw_spi_setCLK_output(0)
              
void SPI_Delay(UINT delayTime);
void SPI_WriteByte(UINT8 tempdata);
UINT8 SPI_ReadByte(void);

#endif
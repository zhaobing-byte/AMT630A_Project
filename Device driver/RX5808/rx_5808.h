/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 rx_5808.h
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-15

*Description:
              rx_5808模块相关驱动函数
*History:  

************************************************************************/

#ifndef _RX_5808_H_
#define	_RX_5808_H_

#include "SPI.h"

void setSynthRegisterB(UINT value); 
void setPowerDownRegister(UINT32 value); 
#endif
/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 channels.h
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-16

*Description:
              处理RX_5808接收机通道设置函数。
*History:  

************************************************************************/
#ifndef __CHANNELS_H_
#define __CHANNELS_H_

#include "DataType.h"
#include "rx_5808.h"

#ifdef USE_LBAND
	#define CHANNELS_SIZE 48
#else 
	#define CHANNELS_SIZE 40
#endif

#define QUIET_FR_LIMIT_LOW 30
#define QUIET_FR_LIMIT_MEDIUM 70



typedef struct
{
     UINT8 RF_Number;
	 UINT8 RSSI;
}FrequencyPointStatus;


UINT  getSynthRegisterB(UINT8 index);
UINT  getFrequency(UINT8 index);
char *getName(UINT8 index);
UINT8 getOrderedIndex(UINT8 index);	
UINT8 getOrderedIndexFromIndex(UINT8 index);				   
UINT8 getCurrentAdcRssiValue(void);
UINT8 FastSearchFrequency(void);
UINT8 FastScanFrequency(void);
void setFrequencyPointStatus(UINT8 LashFRNumber,UINT8 LashFRrssi);
void getFrequencyPointStatus(FrequencyPointStatus *LastFrStatus);

#endif
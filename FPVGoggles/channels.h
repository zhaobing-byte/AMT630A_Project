/***********************************************************************
*Copyright (C) 2020 ���ڹ����Ƽ����޹�˾�з���
* All rights reserved.

*File name: �� channels.h
*Version:    ��0.1
*Author:       zhaobing
*update:       2020-06-16

*Description:
              ����RX_5808���ջ�ͨ�����ú�����
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

UINT  getSynthRegisterB(UINT8 index);
UINT  getFrequency(UINT8 index);
char *getName(UINT8 index);
UINT8 getOrderedIndex(UINT8 index);	
UINT8 getOrderedIndexFromIndex(UINT8 index);				   
UINT8 getCurrentAdcRssiValue(void);
UINT8 FastSearchFrequency(void);

#endif
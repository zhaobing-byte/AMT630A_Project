/***********************************************************************
*Copyright (C) 2020 深圳哈鸣科技有限公司研发部
* All rights reserved.

*File name: 　 channels.c
*Version:    　0.1
*Author:       zhaobing
*update:       2020-06-16

*Description:
              处理RX_5808接收机通道设置函数。
*History:  

************************************************************************/
#include "channels.h"
#include "systemConfig.h"
#include "global.h"
#include "AdcKeyPad.h"
#include "AMT_Reg.h"
#include "AMT_Drv.h"
#include "Delay.h"
#include "Draw_osd.h"
#include "Debug.h"
#include "DataType.h"
#include "MsgMap.h"
#include "Debug.h"
#include "Mcu.h"
#include "Osd_menu.h"
#include "Buzzer.h"

#define RSSI_CH             CH1              //RX_RSSI_ADC 的值在通道1上采集
#define USE_LBAND  
static FrequencyPointStatus lastFrstatus;
static UINT CODE channelTable[] = {
	
	#define _CHANNEL_REG_FLO(f) ((f-479)/2)
	#define _CHANNEL_REG_N(f)   (_CHANNEL_REG_FLO(f) / 32)
	#define _CHANNEL_REG_A(f)   (_CHANNEL_REG_FLO(f) % 32)
	#define CHANNEL_REG(f)     (_CHANNEL_REG_N(f) << 7) | _CHANNEL_REG_A(f)
	// A
	CHANNEL_REG(5865),
    CHANNEL_REG(5845),
    CHANNEL_REG(5825),
    CHANNEL_REG(5805),
    CHANNEL_REG(5785),
    CHANNEL_REG(5765),
    CHANNEL_REG(5745),
    CHANNEL_REG(5725),
	// B
    CHANNEL_REG(5733),
    CHANNEL_REG(5752),
    CHANNEL_REG(5771),
    CHANNEL_REG(5790),
    CHANNEL_REG(5809),
    CHANNEL_REG(5828),
    CHANNEL_REG(5847),
    CHANNEL_REG(5866),

	// E
    CHANNEL_REG(5705),
    CHANNEL_REG(5685),
    CHANNEL_REG(5665),
    CHANNEL_REG(5645),
    CHANNEL_REG(5885),
    CHANNEL_REG(5905),
    CHANNEL_REG(5925),
    CHANNEL_REG(5945),

	// F
    CHANNEL_REG(5740),
    CHANNEL_REG(5760),
    CHANNEL_REG(5780),
    CHANNEL_REG(5800),
    CHANNEL_REG(5820),
    CHANNEL_REG(5840),
    CHANNEL_REG(5860),
    CHANNEL_REG(5880),

	// C / R
    CHANNEL_REG(5658),
    CHANNEL_REG(5695),
    CHANNEL_REG(5732),
    CHANNEL_REG(5769),
    CHANNEL_REG(5806),
    CHANNEL_REG(5843),
    CHANNEL_REG(5880),
    CHANNEL_REG(5917)

	#ifdef USE_LBAND
    ,
    CHANNEL_REG(5362),
    CHANNEL_REG(5399),
    CHANNEL_REG(5436),
    CHANNEL_REG(5473),
    CHANNEL_REG(5510),
    CHANNEL_REG(5547),
    CHANNEL_REG(5584),
    CHANNEL_REG(5621),
    #endif

	#undef _CHANNEL_REG_FLO
	#undef _CHANNEL_REG_A
	#undef _CHANNEL_REG_N
	#undef CHANNEL_REG
};

// Channels with their Mhz Values
static CODE UINT channelFreqTable[] = {
    5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // A
    5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // B
    5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // E
    5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // F / Airwave
    5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917  // C / Immersion Raceband
#ifdef USE_LBAND
    ,
    5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621  // D / 5.3
#endif
};



// Encode channel names as an 8-bit value where:
//      0b00000111 = channel number (zero-indexed)
//      0b11111000 = channel letter (offset from 'A' character)
static UINT8 CODE channelNames[] = {
    #define _CHANNEL_NAMES(l) (UINT8) ((l - 65) << 3)
    #define CHANNEL_NAMES(l) \
    _CHANNEL_NAMES(l) | 0, \
    _CHANNEL_NAMES(l) | 1, \
    _CHANNEL_NAMES(l) | 2, \
    _CHANNEL_NAMES(l) | 3, \
    _CHANNEL_NAMES(l) | 4, \
    _CHANNEL_NAMES(l) | 5, \
    _CHANNEL_NAMES(l) | 6, \
    _CHANNEL_NAMES(l) | 7

    CHANNEL_NAMES('A'),
    CHANNEL_NAMES('B'),
    CHANNEL_NAMES('E'),
    CHANNEL_NAMES('F'), // a.k.a Airwave
    CHANNEL_NAMES('R')  // C / Immersion Raceband

    #ifdef USE_LBAND
        ,
        CHANNEL_NAMES('L')
    #endif

    #undef CHANNEL_NAMES
    #undef _CHANNEL_NAMES
};

// All Channels of the above List ordered by Mhz
static UINT8 CODE channelFreqOrderedIndex[] = {
    #ifdef USE_LBAND
        40, // 5362
        41, // 5399
        42, // 5436
        43, // 5473
        44, // 5510
        45, // 5547
        46, // 5584
        47, // 5621
        19, // 5645
        32, // 5658
        18, // 5665
        17, // 5685
        33, // 5695
        16, // 5705
         7, // 5725
        34, // 5732
         8, // 5733
        24, // 5740
         6, // 5745
         9, // 5752
        25, // 5760
         5, // 5765
        35, // 5769
        10, // 5771
        26, // 5780
         4, // 5785
        11, // 5790
        27, // 5800
         3, // 5805
        36, // 5806
        12, // 5809
        28, // 5820
         2, // 5825
        13, // 5828
        29, // 5840
        37, // 5843
         1, // 5845
        14, // 5847
        30, // 5860
         0, // 5865
        15, // 5866
        31, // 5880
        38, // 5880
        20, // 5885
        21, // 5905
        39, // 5917
        22, // 5925
        23  // 5945
    #else
        19, //5645
        32, //5658
        18, //5665
        17, //5685
        33, //5695
        16, //5705
         7, //5725
        34, //5732
         8, //5733
        24, //5740
         6, //5745
         9, //5752
        25, //5760
         5, //5765
        35, //5769
        10, //5771
        26, //5780
         4, //5785
        11, //5790
        27, //5800
         3, //5805
        36, //5806
        12, //5809
        28, //5820
         2, //5825
        13, //5828
        29, //5840
        37, //5843
         1, //5845
        14, //5847
        30, //5860
         0, //5865
        15, //5866
        31, //5880
        38, //5880
        20, //5885
        21, //5905
        39, //5917
        22, //5925
        23, //5945
    #endif
};


static UINT8 CODE channelIndexToOrderedIndex[] = {
    #ifdef USE_LBAND
        39,
        36,
        32,
        28,
        25,
        21,
        18,
        14,
        16,
        19,
        23,
        26,
        30,
        33,
        37,
        40,
        13,
        11,
        10,
         8,
        43,
        44,
        46,
        47,
        17,
        20,
        24,
        27,
        31,
        34,
        38,
        41,
         9,
        12,
        15,
        22,
        29,
        35,
        42,
        45,
         0,
         1,
         2,
         3,
         4,
         5,
         6,
         7
    #else
        31,
        28,
        24,
        20,
        17,
        13,
        10,
         6,
         8,
        11,
        15,
        18,
        22,
        25,
        29,
        32,
         5,
         3,
         2,
         0,
        35,
        36,
        38,
        39,
         9,
        12,
        16,
        19,
        23,
        26,
        30,
        33,
         1,
         4,
         7,
        14,
        21,
        27,
        34,
        37
    #endif
};

/*************************
*函数名字：map 
*函数功能：数的区间映射
*参    数：Oxy：需要映射的数  Omin：映射前区间的最小值 Omax：映射前区间的最大值 
							  Nmin: 映射后区间的最小值 Nmax：映射后区间的最小值
*返 回 值：Nxy:映射后的数
**************************/
//int map(float Oxy, float Omin, float Omax, float Nmin, float Nmax)
//{
//	int Nxy;
//	Nxy = (Nmax-Nmin)/(Omax-Omin)*(Oxy-Omin)+Nmin;
//	return Nxy;
//}

const UINT getSynthRegisterB(UINT8 index) 
{
	return channelTable[index];
}

const UINT getFrequency(UINT8 index) 
{
    return channelFreqTable[index];
}

// Returns channel name as a string.
//      dest[] must be at least 3-bytes.
char nameBuffer[3];

const char *getName(UINT8 index)
{
    UINT8 encodedName = channelNames[index];

    nameBuffer[0] = 65 + (encodedName >> 3);
    nameBuffer[1] = 48 + (encodedName & (255 >> (8 - 3))) + 1;
    nameBuffer[2] = '\0';											    

    return nameBuffer;
}

UINT8 getOrderedIndex(UINT8 index) 
{
	return channelFreqOrderedIndex[index];
}


const UINT8 getOrderedIndexFromIndex(UINT8 index) 
{
    return channelIndexToOrderedIndex[index];
}


static UINT POS_GetBestAdcRssiVal( UINT * AdcBuf, UCHAR Length)
{
	return POS_GetBestAdcKeyVal(AdcBuf,Length);
}

UINT8 getCurrentAdcRssiValue(void)
{
	static UINT8 XDATA CurrentRSSIVal = 0;
	static UINT  XDATA RSSIAdcBuf[] = {0XFFFF,0XFFFF,0XFFFF,0XFFFF};   //ADC 采样零时缓冲区
	static UINT  XDATA CurrentAdcVal;             //记录当前的ADC值
	static UCHAR Index;
	
	for(Index = 0 ; Index <= 4 ; Index ++)
	{
		RSSIAdcBuf[Index] = POS_EnableChipAdc(RSSI_CH);
		DelayMs(1);
	}	
	CurrentAdcVal = POS_GetBestAdcRssiVal(RSSIAdcBuf,4);
	CurrentRSSIVal = (99.0)/(2200.0-500.0)*(CurrentAdcVal-500.0);
	if(CurrentRSSIVal > 99.0)
	{
		CurrentRSSIVal = 99.0;
	}
	return CurrentRSSIVal;
}

UINT8 FastSearchFrequency(void)
{
	UINT8 CurrentRssiMax = 0;
	UINT8 CurrentRssi = 0;
	UINT8 ChannelIndex;
	UINT8 MAX_RSSI_CH_FLG = 0;
	for(ChannelIndex = 0 ; ChannelIndex < 48 ; ChannelIndex++)  
	{
		
			setSynthRegisterB(getSynthRegisterB(ChannelIndex));      //设置频点
			DelayMs(20);
			CurrentRssi =  getCurrentAdcRssiValue();
			OSD_SetFrequencyRssiMark(ChannelIndex,CurrentRssi);
			if(CurrentRssi >= CurrentRssiMax)
			{
				CurrentRssiMax = CurrentRssi;
				MAX_RSSI_CH_FLG = ChannelIndex;
			} 
			OsdDrawStr(12,7,COLOR(CYAN,BLACK),"searching...");
	}
	BuzzerOn(1);
	setSynthRegisterB(getSynthRegisterB(MAX_RSSI_CH_FLG));	      //跳到当前空间RSSI最大的频点
	OSD_SetFrequencyMark(MAX_RSSI_CH_FLG,COLOR(RED,BLACK));
	
	return MAX_RSSI_CH_FLG;
}






UINT8 FastScanFrequency(void)
{
	UINT8 CurrentRssiMax = 0;
	UINT8 CurrentRssi = 0;
	UINT8 ChannelIndex;
	UINT8 MAX_RSSI_CH_FLG = 0;
	for(ChannelIndex = 0 ; ChannelIndex < 48 ; ChannelIndex++)  
	{
			setSynthRegisterB(getSynthRegisterB(ChannelIndex));      //设置频点
			DelayMs(20);
			CurrentRssi =  getCurrentAdcRssiValue();
			if(CurrentRssi >= CurrentRssiMax)
			{
				CurrentRssiMax = CurrentRssi;
				MAX_RSSI_CH_FLG = ChannelIndex;
			} 
			OSD_SetFrequencyRssiMark(ChannelIndex,CurrentRssi);
			OsdDrawStr(12,7,COLOR(CYAN,BLACK),"scanning...");			
	}
	BuzzerOn(2);
	setSynthRegisterB(getSynthRegisterB(MAX_RSSI_CH_FLG));	      //跳到当前空间RSSI最大的频点
	OSD_SetFrequencyMark(MAX_RSSI_CH_FLG,COLOR(RED,BLACK));
	return MAX_RSSI_CH_FLG;
}


void setFrequencyPointStatus(UINT8 LashFRNumber,UINT8 LashFRrssi)
{
	lastFrstatus.RF_Number =  LashFRNumber;
	lastFrstatus.RSSI = LashFRrssi;
}

void getFrequencyPointStatus(FrequencyPointStatus *LastFrStatus)
{
	LastFrStatus->RF_Number = lastFrstatus.RF_Number;
	LastFrStatus->RSSI = lastFrstatus.RSSI;
}
/*****************************************
			迭代搜频率
*****************************************/





/****************************************
			绿色频点扫描
*****************************************/






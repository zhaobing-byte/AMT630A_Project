#include "battAdc.h"
#include "global.h"
#include "systemConfig.h"
#include "Debug.h"
#include "BatteryAdc.h"
#include "AMT_Reg.h"
#include "AMT_Drv.h"
#include "Delay.h"


#define BATT_CH CH2
/*************************************************************************
name:    GetBestBatteryAdcVal(UCHAR* AdcBuf, UCHAR Length)
input:   AdcBuf  //���˲�������ָ�롣
         Length  //���˲������ݳ��ȡ�
output:	 ����ƽ������ƽ���˲�������ݡ�
update�� 2012-10-30
state��  try out
description: ����ƽ������ƽ���˲�������ݡ�

*History:  yshuizhou  2012/10/30   0.1    build  this  moudle
*************************************************************************/
static UINT GetBestBatteryAdcVal( UINT * AdcBuf, UCHAR Length)
{
	UCHAR XDATA i,j,index;
	UINT XDATA s_offset1 = 0,s_offset2 = 0;
	
	 for(i=0; i<Length; i++)
	 {
	    s_offset1 += Abs(AdcBuf[i], AdcBuf[0]);
	 }
	index = 0;
	for(i=1; i<Length; i++)
	{
	    for(j=0; j<Length;j++)
	    {
	        s_offset2 += Abs(AdcBuf[i], AdcBuf[j]);
	    }
	    if(s_offset1 > s_offset2)
	    {
	        s_offset1 = s_offset2;
		    index = i;
	    }
	    s_offset2 = 0;	
	}	
	return AdcBuf[index];
}

UINT16 GetBatteryVol(void)
{
	static UINT XDATA CurrentAdcVal;
	static UINT  XDATA BattAdcBuf[] = {0XFFFF,0XFFFF,0XFFFF,0XFFFF};   //ADC ������ʱ������
	static UCHAR Index;
	UINT16 BATTVOL;
	for(Index = 0 ; Index <= 4 ; Index ++)
	{
		BattAdcBuf[Index] = POS_EnableChipAdc(BATT_CH);
		DelayMs(1);
	}
	CurrentAdcVal = GetBestBatteryAdcVal(BattAdcBuf,4);
	BATTVOL = 6.6*CurrentAdcVal/4096.0*100;
	return BATTVOL;
}


































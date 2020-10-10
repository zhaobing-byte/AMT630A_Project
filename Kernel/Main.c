/***********************************************************************
*Copyright (C) 2005 ���ڰ��ƴ������޹�˾��ƷӦ��һ��
* All rights reserved.

*File name: �� Main.c
*Version:    ��Ver-2.0
*Author:       cjinzong
*update:       2010-05-27

*Description:
               �����ϵͳ���������п�ܡ�
*History:  
   
************************************************************************/
#define _MAIN_C_

#include "DataType.h"
#include "SystemConfig.h"
#include "Global.h"
#include "MsgMap.h"
#include "Delay.h"
#include "SysPower.h"
#include "GetSysMsg.h"
#include "EventHandle.h"
#include "Debug.h"
#include "Mcu.h"
#include "Interrupt.h"
#include "Osd_menu.h"
#include "AMT_Reg.h"
#include "AMT_Drv.h"
#include "AMT_Mcu.h"
#include "channels.h"
#include "Draw_osd.h"
#include "ConfigLcdPara.h"
UINT16 BattVol;

UINT16 UpdataBattVol(void)
{
	return BattVol;
}

void main(void)																 										   
{
	UINT16 draw_osd_loop_count = 0;
	UINT16 batt_updata_loop_count = 0;
    DisableWatchdog();
		
	InitSystem();  
	//SetPWM1(800,50);
	printfStr("<<<<<<<<<<<<<<  Welcom to ARK >>>>>>>>>>>>\n");
	
	MenuProcessKey(COMD_SelPowerOnMode);
	
	printfStr("<<<<<<<<<<<<<<  Enter main loop >>>>>>>>>>>>\n");	
	printfStr("test ADC TP3_KEY :   \n");
	BRIGHT_REG    =  DataCurve(BRIGHT_MIN,g_ucbrightness,BRIGHT_MAX, 60,MAX_VALUE);                 //����
	CONTRAST_REG  =  DataCurve(CONTRAST_MIN,g_ucContrast,CONTRAST_MAX,50,MAX_VALUE);                //�Աȶ�
	SATURATION_REG=  DataCurve(SATURATION_MIN,g_ucSaturation,SATURATION_MAX,50,MAX_VALUE);          //ɫ��
	TINT_REG      =  TintCurve(50,MAX_VALUE);                                                       //ɫ�� 
	while(1)
	{   
	    MSG curMsg;

		POS_ClearWatchDog();	 
	    /**********SPI��������VCOM����*********/
		#ifdef SpiInitPannelEn
	    #ifdef SpiAdjRegEn 
		ES = ON;
		POS_AdjustSpiReg();
		#endif
		#endif
		
        /**********�û�������Ϣ����*********/
		curMsg = POS_GetUserInputMsg(g_bGetUserInputFlg); 
		if(curMsg != MSG_NULL)
		{
			OsdMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/**********�����ź���Ϣ����*********/
		curMsg = POS_GetSignalMsg(g_bGetSignalFlg);
        if(curMsg != MSG_NULL)
		{
			SignalMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}

		/**********ͼ����ʽ��Ϣ����*********/
		curMsg = POS_GetColorSysMsg(g_bGetColorSysFlg);
        if(curMsg != MSG_NULL)
		{
			ColorSysMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/**********ʱ����Ϣ����*********/
		curMsg = POS_GetTimeMsg(g_bGetTimeFlg);
        if(curMsg != MSG_NULL)
		{
			TimeMsgHandle(curMsg); 
			curMsg = MSG_NULL;
		}         
		
		/**********���ADC��Ϣ����*********/
        curMsg = POS_GetBatteryAdcMsg(g_bGetBatteryAdcFlg);
		if(curMsg != MSG_NULL)
		{
			BatteryAdcMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/*********�û��Զ�����Ϣ����********/
		curMsg = POS_GetUserDefinedMsg(g_bGetUserDefFlg); 
		if(curMsg != MSG_NULL)
		{
			UserDefinedMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}

		/**********BT��Ϣ����*********/
        curMsg = POS_GetBTMsg(g_bGetBTFlg);
		if(curMsg != MSG_NULL)
		{
			BTMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
	    /**********�޷�������Ϣ����*********/
		curMsg = POS_GetMiscMsg(g_bGetMiscFlg); 
		if(curMsg != MSG_NULL)
		{
			MiscMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		draw_osd_loop_count++;
		batt_updata_loop_count++;

		if(batt_updata_loop_count > 2000)
		{
			BattVol = GetBatteryVol();
			//��Ҫ����#include "ConfigLcdPara.h"
			
			batt_updata_loop_count = 0;
		}
		if(draw_osd_loop_count > 200)
		{
			FpvOsdUpdata();
			draw_osd_loop_count = 0;
		}
	}
	return;
}

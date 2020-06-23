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
void main(void)																 										   
{
	UINT8 RSSI;
	UINT16 loop_count = 0;
	UINT8 i= 0;
    DisableWatchdog();
		
	InitSystem();  
	printfStr("<<<<<<<<<<<<<<  Welcom to ARK >>>>>>>>>>>>\n");
	
	MenuProcessKey(COMD_SelPowerOnMode);
	
	printfStr("<<<<<<<<<<<<<<  Enter main loop >>>>>>>>>>>>\n");	
	printfStr("test ADC TP3_KEY :   \n");
	
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
		loop_count++;
		if(loop_count > 200)
		{
			if(get_menu_status())
			{
				RSSI = (99.0)/(1920.0-500.0)*(POS_EnableChipAdc(CH1)-500);
				if(RSSI >= 99)
				{	
					RSSI = 99;
				}
				XBYTE[0XFB05]=0X41;                //��BLOCK0	
				OsdBlockEnable(0);                 //ʹ��ѡ��Ŀ�
				OsdConfigWndSize(0x14,0x0D);       //���ÿ��С	
				OsdConfigWndPosition(260,100);     //���ÿ��λ��
				XBYTE[0XFB2A]=0X12;                //ǰ����ɫ�ǵ�ɫ��2��������ɫ�ǵ�ɫ��1
				XBYTE[0XFB56]=0X00; 
				XBYTE[0XFB57]=0X00;                //����ɫ
				OsdDrawNum(1,15,GREEN,RSSI);
				OsdDrawStr(1,14,GREEN," \xAF");
				OsdDrawGuage(3,1,99,GREEN,RSSI);
				OsdDrawStr(1,1,GREEN,"BAND:");
				OsdDrawStr(1,6,GREEN,getName(getRFTabRow()*8+getRFTabLine()));
				OsdDrawStr(1,8,GREEN," ");
				OsdDrawNum(1,9,GREEN,getFrequency(getRFTabRow()*8+getRFTabLine()));
			  	OsdDrawStr(4,3,GREEN,"\xAF");
			  	OsdDrawStr(4,4,GREEN,"1 2 3 4 5 6 7 8");
			  	OsdDrawStr(5,2,GREEN,"A\xAF");
				
				OsdDrawStr(5,1,GREEN," ");
				OsdDrawStr(6,1,GREEN," ");
				OsdDrawStr(7,1,GREEN," ");
				OsdDrawStr(8,1,GREEN," ");
				OsdDrawStr(9,1,GREEN," ");
				OsdDrawStr(10,1,GREEN," ");
				OsdDrawStr(getRFTabRow()+5,1,GREEN,"+");
				
			  	OsdDrawStr(6,2,GREEN,"B\xAF");
			  	OsdDrawStr(7,2,GREEN,"E\xAF");
			  	OsdDrawStr(8,2,GREEN,"F\xAF");
			  	OsdDrawStr(9,2,GREEN,"R\xAF");
			  	OsdDrawStr(10,2,GREEN,"L\xAF");
			  	OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5");
				if(1 == get_cursor_line())
				{
					//OsdDrawStr(12,get_cursor_line()+3,GREEN,"+");
				}
				else
				{
					//OsdDrawStr(12,get_cursor_line()+5,GREEN,"+");
				}
				loop_count = 0;
			}
			else
			{
				OsdBlockHide(0);
			}
		}
	}
	return;
}

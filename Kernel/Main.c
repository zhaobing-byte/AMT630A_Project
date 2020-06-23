/***********************************************************************
*Copyright (C) 2005 深圳艾科创新有限公司产品应用一部
* All rights reserved.

*File name: 　 Main.c
*Version:    　Ver-2.0
*Author:       cjinzong
*update:       2010-05-27

*Description:
               这个是系统的整个运行框架。
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
	    /**********SPI驱动屏的VCOM调节*********/
		#ifdef SpiInitPannelEn
	    #ifdef SpiAdjRegEn 
		ES = ON;
		POS_AdjustSpiReg();
		#endif
		#endif
		
        /**********用户输入消息处理*********/
		curMsg = POS_GetUserInputMsg(g_bGetUserInputFlg); 
		if(curMsg != MSG_NULL)
		{
			OsdMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/**********输入信号消息处理*********/
		curMsg = POS_GetSignalMsg(g_bGetSignalFlg);
        if(curMsg != MSG_NULL)
		{
			SignalMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}

		/**********图像制式消息处理*********/
		curMsg = POS_GetColorSysMsg(g_bGetColorSysFlg);
        if(curMsg != MSG_NULL)
		{
			ColorSysMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/**********时间消息处理*********/
		curMsg = POS_GetTimeMsg(g_bGetTimeFlg);
        if(curMsg != MSG_NULL)
		{
			TimeMsgHandle(curMsg); 
			curMsg = MSG_NULL;
		}         
		
		/**********电池ADC消息处理*********/
        curMsg = POS_GetBatteryAdcMsg(g_bGetBatteryAdcFlg);
		if(curMsg != MSG_NULL)
		{
			BatteryAdcMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
		/*********用户自定义消息处理********/
		curMsg = POS_GetUserDefinedMsg(g_bGetUserDefFlg); 
		if(curMsg != MSG_NULL)
		{
			UserDefinedMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}

		/**********BT消息处理*********/
        curMsg = POS_GetBTMsg(g_bGetBTFlg);
		if(curMsg != MSG_NULL)
		{
			BTMsgHandle(curMsg);
			curMsg = MSG_NULL;
		}
		
	    /**********无法分类消息处理*********/
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
				XBYTE[0XFB05]=0X41;                //打开BLOCK0	
				OsdBlockEnable(0);                 //使能选择的块
				OsdConfigWndSize(0x14,0x0D);       //设置块大小	
				OsdConfigWndPosition(260,100);     //设置块的位置
				XBYTE[0XFB2A]=0X12;                //前景颜色是调色盘2，背景颜色是调色盘1
				XBYTE[0XFB56]=0X00; 
				XBYTE[0XFB57]=0X00;                //背景色
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

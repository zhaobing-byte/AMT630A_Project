#include "Draw_osd.h"
#include "channels.h"
#include "systemConfig.h"
#include "global.h"
#include "AdcKeyPad.h"
#include "AMT_Reg.h"
#include "AMT_Drv.h"
#include "Delay.h"
#include "DataType.h"
#include "MsgMap.h"
#include "Debug.h"
#include "Mcu.h"
#include "Osd_menu.h"

UINT8 DrawFrePointCursor(UINT8 FreNumber);

UINT8 Draw_Fre_point_inf_OSD(void)
{
	UINT8 RSSI;
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
		OsdDrawStr(2,1,GREEN,"&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1");
		OsdDrawGuage(3,1,99,GREEN,RSSI);
		OsdDrawStr(1,1,GREEN,"BAND:");
		OsdDrawStr(1,6,GREEN,getName(getFreNumber()));
		OsdDrawStr(1,8,GREEN," ");
		OsdDrawNum(1,9,GREEN,getFrequency(getFreNumber()));
	  	OsdDrawStr(4,3,GREEN,"\xAF");
	  	OsdDrawStr(4,4,GREEN,"1 2 3 4 5 6 7 8");
		DrawFrePointCursor(getFreNumber());
	  	OsdDrawStr(5,2,GREEN, "A\xAF");	
	  	OsdDrawStr(6,2,GREEN, "B\xAF");
	  	OsdDrawStr(7,2,GREEN, "E\xAF");
	  	OsdDrawStr(8,2,GREEN, "F\xAF");
	  	OsdDrawStr(9,2,GREEN, "R\xAF");
	  	OsdDrawStr(10,2,GREEN,"L\xAF");
	}
	else
	{
		OsdBlockHide(0);
	}
	return 0;	
}

UINT8 DrawFrePointCursor(UINT8 FreNumber)
{
	UINT8 row,line;      //�У��б��
	if(FreNumber > 47 || FreNumber < 0)         //���Ƶ�ʱ�Ų���0~47 ������ı�Ų��Ϸ�
	{
		FreNumber = 0;
	}

	row  = FreNumber / 8;
	line = FreNumber % 8; 

	switch (row)
	{
		case 0:
			OsdDrawStr(5,1,GREEN,"+");OsdDrawStr(6,1,GREEN," ");OsdDrawStr(7,1,GREEN," ");OsdDrawStr(8,1,GREEN," ");OsdDrawStr(9,1,GREEN," ");OsdDrawStr(10,1,GREEN," ");
			break;
		case 1:
			OsdDrawStr(5,1,GREEN," ");OsdDrawStr(6,1,GREEN,"+");OsdDrawStr(7,1,GREEN," ");OsdDrawStr(8,1,GREEN," ");OsdDrawStr(9,1,GREEN," ");OsdDrawStr(10,1,GREEN," ");
			break;
		case 2:
			OsdDrawStr(5,1,GREEN," ");OsdDrawStr(6,1,GREEN," ");OsdDrawStr(7,1,GREEN,"+");OsdDrawStr(8,1,GREEN," ");OsdDrawStr(9,1,GREEN," ");OsdDrawStr(10,1,GREEN," ");
			break;
		case 3:
			OsdDrawStr(5,1,GREEN," ");OsdDrawStr(6,1,GREEN," ");OsdDrawStr(7,1,GREEN," ");OsdDrawStr(8,1,GREEN,"+");OsdDrawStr(9,1,GREEN," ");OsdDrawStr(10,1,GREEN," ");
			break;
		case 4:
			OsdDrawStr(5,1,GREEN," ");OsdDrawStr(6,1,GREEN," ");OsdDrawStr(7,1,GREEN," ");OsdDrawStr(8,1,GREEN," ");OsdDrawStr(9,1,GREEN,"+");OsdDrawStr(10,1,GREEN," ");
			break;
		case 5:
			OsdDrawStr(5,1,GREEN," ");OsdDrawStr(6,1,GREEN," ");OsdDrawStr(7,1,GREEN," ");OsdDrawStr(8,1,GREEN," ");OsdDrawStr(9,1,GREEN," ");OsdDrawStr(10,1,GREEN,"+");
			break;
		default:
			break;
	}
	
	switch (line)
	{
		case 0:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5");
			break;
		case 1:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5");
			break;
		case 2:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5");
			break;
		case 3:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5");
			break;
		case 4:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5");
			break;
		case 5:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5");
			break;
		case 6:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5");
			break;
		case 7:
			OsdDrawStr(11,1,GREEN,"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0");
			break;
		default:
			break;		
	}
	return 0;
}

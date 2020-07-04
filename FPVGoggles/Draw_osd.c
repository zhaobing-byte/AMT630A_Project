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



UINT8 FpvOsdUpdata(void)
{
	UINT8 RSSI;
	if(get_menu_status())
	{
	    RSSI = getCurrentAdcRssiValue();
		setFrequencyPointStatus(getFreNumber(),RSSI);       //记录当前频点的通道编号以及RSSI
		OsdDrawStr(1,6,COLOR(CYAN,BLACK),getName(getFreNumber()));
		OsdDrawNum(1,9,COLOR(CYAN,BLACK),getFrequency(getFreNumber()));
		OsdDrawNum(1,15,COLOR(WHITE,BLACK),RSSI);
		OsdDrawStr(1,13,COLOR(WHITE,BLACK),"   \xAF");
		OsdDrawGuage(3,1,99,COLOR(WHITE,BLACK),RSSI);	
	  	OsdDrawStr(4,3,COLOR(WHITE,BLACK),"\xAF");
		OsdDrawStr(12,6,COLOR(WHITE,BLACK),"               ");
		if(UpdataBattVol() > 350)
		{
			DrawBattVol(12,1,COLOR(GREEN,BLACK),UpdataBattVol());
		}
		else
		{
			DrawBattVol(12,1,COLOR(RED,BLACK),UpdataBattVol());
		}
		OSD_SetFrequencyMark(getFreNumber(),COLOR(WHITE,BLACK));
		OsdBlockShow(0);
	}
	else
	{
		//RefreshCursorPanel();
		OsdBlockHide(0);
	}
	return 0;	
}

void DrawFPVOsdWind(void)
{
	OsdBlockEnable(0);                 //使能选择的块
	OsdConfigWndSize(0x14,0x0E);       //设置块大小	
	OsdConfigBlockColor(BLACK);
	OsdConfigWndPosition(260,100);     //设置块的位置
	OsdDrawStr(0,0,COLOR(WHITE,BLACK),"                     ");
	OsdDrawStr(1,1,COLOR(WHITE,BLACK),"BAND:");
	OsdDrawStr(1,8,COLOR(WHITE,BLACK)," ");
	OsdDrawStr(1,13,COLOR(WHITE,BLACK),"   \xAF");
	OsdDrawStr(1,19,COLOR(WHITE,BLACK),"  ");
	OsdDrawStr(2,1,COLOR(WHITE,BLACK),"&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1  ");
	OsdDrawStr(4,0,COLOR(WHITE,BLACK),"   ");
	OsdDrawStr(4,3,COLOR(WHITE,BLACK),"\xAF");
	OsdDrawStr(4,4,COLOR(WHITE,BLACK),"1 2 3 4 5 6 7 8   ");
	OsdDrawStr(5,0,COLOR(WHITE,BLACK)," ");
	OsdDrawStr(5,2,COLOR(WHITE,BLACK), "A\xAF");	
	OsdDrawStr(6,0,COLOR(WHITE,BLACK), "  B\xAF");
	OsdDrawStr(7,0,COLOR(WHITE,BLACK), "  E\xAF");
	OsdDrawStr(8,0,COLOR(WHITE,BLACK), "  F\xAF");
	OsdDrawStr(9,2,COLOR(WHITE,BLACK), "R\xAF");
	OsdDrawStr(10,2,COLOR(WHITE,BLACK),"L\xAF");
	OsdDrawStr(5,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(6,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(7,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(8,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(9,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(10,4,COLOR(WHITE,BLACK),"                ");
	OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
	OsdDrawStr(12,0,COLOR(WHITE,BLACK),"                             ");
	OsdDrawStr(13,0,COLOR(WHITE,BLACK),"                             ");
}


UINT8 DrawFrePointCursor(UINT8 FreNumber)
{
	UINT8 row,line;      //行，列标号
	if(FreNumber > 47 || FreNumber < 0)         //如果频率编号不在0~47 所输入的编号不合法
	{
		FreNumber = 0;
	}

	row  = FreNumber / 8;
	line = FreNumber % 8; 

	switch (row)
	{
		case 0:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK),"+");OsdDrawStr(6,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(7,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(8,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(9,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(10,1,COLOR(WHITE,BLACK)," ");
			break;
		case 1:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(6,1,COLOR(WHITE,BLACK),"+");OsdDrawStr(7,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(8,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(9,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(10,1,COLOR(WHITE,BLACK)," ");
			break;
		case 2:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(6,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(7,1,COLOR(WHITE,BLACK),"+");OsdDrawStr(8,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(9,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(10,1,COLOR(WHITE,BLACK)," ");
			break;
		case 3:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(6,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(7,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(8,1,COLOR(WHITE,BLACK),"+");OsdDrawStr(9,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(10,1,COLOR(WHITE,BLACK)," ");
			break;
		case 4:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(6,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(7,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(8,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(9,1,COLOR(WHITE,BLACK),"+");OsdDrawStr(10,1,COLOR(WHITE,BLACK)," ");
			break;
		case 5:
			OsdDrawStr(5,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(6,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(7,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(8,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(9,1,COLOR(WHITE,BLACK)," ");OsdDrawStr(10,1,COLOR(WHITE,BLACK),"+");
			break;
		default:
			break;
	}
	
	switch (line)
	{
		case 0:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
			break;
		case 1:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
			break;
		case 2:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
			break;
		case 3:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
			break;
		case 4:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5\xB5\xB5 ");
			break;
		case 5:
			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5\xB5\xB5 ");
			break;
		case 6:
 			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0\xB5\xB5 ");
 			break;
 		case 7:
 			OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5&\0 ");
			break;
		default:
			break;		
	}
	return 0;
}

void OSD_SetFrequencyMark(UINT8 FrNumber,UCHAR fbColor)
{
	UINT8 row,line;      //行，列标号
	row  = FrNumber / 8;
	line = FrNumber % 8; 
	switch (line)
	{
		case 0:
			line = line + 4;
			break;
		case 1:
			line = line + 3 + 2;
			break;
		case 2:
			line = line + 3 + 3;
			break;
		case 3:
			line = line + 3 + 4;
			break;
		case 4:
			line = line + 3 + 5;
			break;
		case 5:
			line = line + 3 + 6;
			break;
		case 6:
			line = line + 3 + 7;
			break;
		case 7:
			line = line + 3 + 8;
			break;	
	}
	OsdDrawStr(5+row,line,fbColor,"&\2 ");
}

void OSD_ResetFrequencyMark(UINT8 FrNumber)
{
	UINT8 row,line;      //行，列标号
	row  = FrNumber / 8;
	line = FrNumber % 8; 
	switch (line)
	{
		case 0:
			line = line + 4;
			break;
		case 1:
			line = line + 3 + 2;
			break;
		case 2:
			line = line + 3 + 3;
			break;
		case 3:
			line = line + 3 + 4;
			break;
		case 4:
			line = line + 3 + 5;
			break;
		case 5:
			line = line + 3 + 6;
			break;
		case 6:
			line = line + 3 + 7;
			break;
		case 7:
			line = line + 3 + 8;
			break;	
	}
	OsdDrawStr(5+row,line,COLOR(WHITE,BLACK),"&\2 ");
}


void OSD_SetFrequencyRssiMark(UINT8 FrNumber,UINT8 RSSI)
{
	if(RSSI <= QUIET_FR_LIMIT_LOW)
	{
		OSD_SetFrequencyMark(FrNumber,COLOR(GREEN,BLACK));
	}
	
	if(QUIET_FR_LIMIT_LOW < RSSI && RSSI <= QUIET_FR_LIMIT_MEDIUM)
	{
		OSD_SetFrequencyMark(FrNumber,COLOR(YELLOW,BLACK));
	}

	if(QUIET_FR_LIMIT_MEDIUM < RSSI)
	{
		OSD_SetFrequencyMark(FrNumber,COLOR(RED,BLACK));
	}
}

void OSD_ResetFrequencyRssiMark(UINT8 FrNumber)
{


}

/*UINT8 DrawBattVol(void)
{
	//OsdBlockClear(0);
	OsdBlockEnable(1);
	OsdConfigWndSize(0x05,0x01);         //设置块大小	
	OsdConfigWndPosition(30,22);         //设置块的位置
	//OsdConfigBlockColor(RED);
	OsdDrawStr(0,0,COLOR(RED,TRANSPARENCE),"hello");   //TRANSPARENCE 透明
	OsdBlockShow(1);
	return 0;
}*/

void DrawBattVol(UCHAR starRow,UCHAR starLine,UCHAR fbColor, UINT16 VOL)
{
	 UCHAR CODE DatBuf[] = {'0','1','2','3','4','5','6','7','8','9'};
	 UCHAR XDATA DispValBuf[6] = "";
	 DispValBuf[0]=DatBuf[VOL/100];
	 DispValBuf[1]= '.';
	 DispValBuf[2]=DatBuf[(VOL%100)/10];
	 DispValBuf[3]=DatBuf[((VOL%100)%10)];
	 DispValBuf[4] = 'V';
	 DispValBuf[5] = '\0';
	 OsdDrawStr(starRow,starLine,fbColor,DispValBuf);
}





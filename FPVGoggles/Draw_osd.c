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


UINT8 Draw_Fre_point_inf_OSD(void)
{
	UINT8 RSSI;
	if(get_menu_status())
	{
	    RSSI = getCurrentAdcRssiValue();
		OsdBlockEnable(0);                 //使能选择的块
		OsdConfigWndSize(0x14,0x0E);       //设置块大小	
		OsdConfigBlockColor(BLACK);
		OsdConfigWndPosition(260,100);     //设置块的位置
		OsdDrawStr(0,0,COLOR(WHITE,BLACK),"                     ");
		OsdDrawStr(1,1,COLOR(WHITE,BLACK),"BAND:");
		OsdDrawStr(1,6,COLOR(CYAN,BLACK),getName(getFreNumber()));
		OsdDrawStr(1,8,COLOR(WHITE,BLACK)," ");
		OsdDrawNum(1,9,COLOR(CYAN,BLACK),getFrequency(getFreNumber()));
		OsdDrawNum(1,15,COLOR(WHITE,BLACK),RSSI);
		OsdDrawStr(1,13,COLOR(WHITE,BLACK),"   \xAF");
		OsdDrawStr(1,19,COLOR(WHITE,BLACK),"  ");
		OsdDrawStr(2,1,COLOR(WHITE,BLACK),"&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1&\1  ");
		OsdDrawGuage(3,1,99,COLOR(WHITE,BLACK),RSSI);	
		OsdDrawStr(4,0,COLOR(WHITE,BLACK),"   ");
	  	OsdDrawStr(4,3,COLOR(WHITE,BLACK),"\xAF");
	  	OsdDrawStr(4,4,COLOR(WHITE,BLACK),"1 2 3 4 5 6 7 8   ");
		//DrawFrePointCursor(getFreNumber());
		OsdDrawStr(5,0,COLOR(WHITE,BLACK)," ");
	  	OsdDrawStr(5,2,COLOR(WHITE,BLACK), "A\xAF                  ");	
	  	OsdDrawStr(6,2,COLOR(WHITE,BLACK), "B\xAF                  ");
	  	OsdDrawStr(7,2,COLOR(WHITE,BLACK), "E\xAF                  ");
	  	OsdDrawStr(8,2,COLOR(WHITE,BLACK), "F\xAF                 ");
	  	OsdDrawStr(9,2,COLOR(WHITE,BLACK), "R\xAF                 ");
	  	OsdDrawStr(10,2,COLOR(WHITE,BLACK),"L\xAF                 ");
		OsdDrawStr(12,0,COLOR(WHITE,BLACK),"                             ");
		OsdDrawStr(11,1,COLOR(WHITE,BLACK),"\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5\xB5 ");
		OsdDrawStr(13,0,COLOR(WHITE,BLACK),"                             ");
		DrawBattVol(12,2,COLOR(GREEN,BLACK),UpdataBattVol());
		OSD_SetFrequencyMark(getFreNumber());
		OsdBlockShow(0);
	}
	else
	{
		OsdBlockHide(0);
	}
	return 0;	
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

void OSD_SetFrequencyMark(UINT8 FrNumber)
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
	OsdDrawStr(5+row,line,COLOR(WHITE,WHITE)," ");
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
	OsdDrawStr(5+row,line,COLOR(WHITE,BLACK)," ");
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





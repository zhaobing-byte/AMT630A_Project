#include "Buzzer.h"
#include "DataType.h"
#include "systemConfig.h"
#include "global.h"
#include "delay.h"
#include "debug.h"
#include "AMT_Reg.h"
#include "AMT_Drv.h"
#include "AMT_Mcu.h"


void Buzzer_on(UINT8 tone)
{
	switch(tone)
	{
		case 1:
			SetPWM1(800,50);
			break;
		case 2:
			SetPWM1(900,50);
			break;
		case 3:
			SetPWM1(800,50);
			break;
		case 4:
			SetPWM1(800,50);
			break;
		case 5:
			SetPWM1(800,50);
			break;
		case 6:
			SetPWM1(800,50);
			break;
		case 7:
			SetPWM1(800,50);
			break;
		default:
			break;
		
	}
}

void Buzzer_off(void)
{
	PWM_CTRL &= ~0x02;   
}


void BuzzerOn(UINT8 Buzzertime)
{
	UCHAR XDATA i;

  	EA = OFF;
    for(i = 0;i<Buzzertime;i++)
    {
      Buzzer_on(2);
      DelayMs(80);
      Buzzer_off();    
      DelayMs(80);
    }
    EA = ON;
}
//void Buzzer_On(UINT8 Buzzertime,UINT8 tone)
//{
//   UCHAR XDATA i;
//
//   EA = OFF;
//   for(i = 0;i<Buzzertime;i++)
//   {
//      Buzzer_on(tone);
//      DelayMs(80);
//      Buzzer_off();
//      DelayMs(80);
//   }
//   EA = ON;
//}





















































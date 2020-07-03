#ifndef __DRAW_OSD_H_
#define __DRAW_OSD_H_

#include "DataType.h"
#include "rx_5808.h"

UINT8 Draw_Fre_point_inf_OSD(void);
UINT8 DrawFrePointCursor(UINT8 FreNumber);
void OSD_SetFrequencyMark(UINT8 FrNumber);
void OSD_ResetFrequencyMark(UINT8 FrNumber);
void DrawBattVol(UCHAR starRow,UCHAR starLine,UCHAR fbColor, UINT16 VOL);
#endif 
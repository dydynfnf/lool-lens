#include "demo.h"

extern u32 FrameBuffer1[224*3][900];
extern u32 FrameBuffer2[224*3][900];
extern u32 MEM[224][900];
extern u8 MEM_EN;

void nav(u32 vscreen_off)
{
	GUI_SelectLayer(1);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);
	
	GUI_SelectLayer(0);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);

	GUI_SetTextMode(GUI_TM_TRANS);
	
	GUI_SelectLayer(0);
	_GUI_SetColor(GUI_WHITE);
	_ShowXBF("0:/CenturyGothic_32pt.xbf");
	GUI_DispStringAt("27",10,vscreen_off+5);
	
	_ShowXBF("0:/CenturyGothic_12pt.xbf");
	GUI_DispStringAt("km/h",65,vscreen_off+30);
	GUI_DispStringAt("15min",25,vscreen_off+85);
	
	GUI_SetPenSize(4);
	_GUI_SetColor((0<<16) | (128<<8) | 255);
	GUI_DrawLine(0,vscreen_off+70,80,vscreen_off+70);
	
	_GUI_SetColor((200<<16) | (200<<8) | 200);
	GUI_DrawLine(80,vscreen_off+70,110,vscreen_off+70);
	
	DMA2D_Display();
	
}

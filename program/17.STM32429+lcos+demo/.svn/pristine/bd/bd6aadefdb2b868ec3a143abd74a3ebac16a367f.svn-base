#include "demo.h"

void time(u32 vscreen_off)
{
	GUI_SelectLayer(1);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);
	
	GUI_SelectLayer(0);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);
	
	GUI_SetTextMode(GUI_TM_TRANS);

	_ShowPNG("0:/weather/2.png",50,vscreen_off+25);
	_ShowPNG("0:/weather/11.png",165,vscreen_off+25);
	_ShowPNG("0:/weather/13.png",165,vscreen_off+65);
	
	_GUI_SetColor(GUI_WHITE);
	_ShowXBF("0:/yahei_12pt_B.xbf");
	GUI_DispStringAt("7℃ ",195,vscreen_off+30);
	GUI_DispStringAt("北京",195,vscreen_off+70);
	
	_ShowXBF("0:/yahei_48pt.xbf");
	GUI_DispStringAt("18   30",40,vscreen_off+140);
	GUI_DispStringAt(":",135,vscreen_off+135);
	
	_GUI_SetColor(0x00AAAAAA);
	_ShowXBF("0:/yahei_12pt_B.xbf");
	GUI_DispStringAt("2015/03/07",45,vscreen_off+115);
	GUI_DispStringAt("星期六 ",185,vscreen_off+115);
	
	DMA2D_Display();
}

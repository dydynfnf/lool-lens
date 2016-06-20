#include "demo.h"

void weather(u32 vscreen_off)
{
	u32 i=0;
	
	GUI_SelectLayer(1);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);
	
	GUI_SelectLayer(0);
	_GUI_SetColor(GUI_BLACK);
	GUI_FillRect(0,vscreen_off,299,vscreen_off+223);
	
	GUI_SetTextMode(GUI_TM_TRANS);

	_GUI_SetColor((30<<16) | (23<<8) | 85);
	GUI_FillRect(0,vscreen_off+120,135,vscreen_off+160);
	
	_GUI_SetColor((90<<16) | (50<<8) | 156);
	GUI_FillRect(0,vscreen_off+160,135,vscreen_off+223);
	
	_GUI_SetColor((245<<16) | (245<<8) | 245);
	GUI_FillRect(135,vscreen_off,299,vscreen_off+223);
	
	_ShowPNG("0:/weather/2+.png",5,vscreen_off+15);
	_ShowPNG("0:/weather/4-.png",140,vscreen_off+120);
	_ShowPNG("0:/weather/5-.png",190,vscreen_off+120);
	_ShowPNG("0:/weather/1-.png",250,vscreen_off+120);
	
	_GUI_SetColor(GUI_WHITE);
	_ShowXBF("0:/yahei_12pt.xbf");
	GUI_DispStringAt("北京 ",45,vscreen_off+130);
	GUI_DispStringAt("星期六 ",40,vscreen_off+190);
	GUI_DispStringAt("2015/03/07",20,vscreen_off+170);
	
	_GUI_SetColor((58<<16) | (16<<8) | 116);
	_ShowXBF("0:/CenturyGothic_32pt.xbf");
	GUI_DispStringAt("8~27°C",145,vscreen_off+30);
	
	_ShowXBF("0:/CenturyGothic_12pt.xbf");
	GUI_DispStringAt("7°C",140,vscreen_off+175);
	GUI_DispStringAt("15°C",140,vscreen_off+195);
	GUI_DispStringAt("5°C",195,vscreen_off+175);
	GUI_DispStringAt("13°C",195,vscreen_off+195);
	GUI_DispStringAt("7°C",250,vscreen_off+175);
	GUI_DispStringAt("18°C",250,vscreen_off+195);
	
	DMA2D_Display();
	}
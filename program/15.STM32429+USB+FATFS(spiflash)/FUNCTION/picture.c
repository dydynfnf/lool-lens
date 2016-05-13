#include "picture.h"

FIL file,fdst;

void _ShowJPG(const char * sFilename, int x0, int y0)
{
	GUI_HMEM hMem;
	char *_acBuffer2;
	u8 result;
	u32 bw;

	hMem = GUI_ALLOC_AllocZero(1024*512);

	_acBuffer2 = GUI_ALLOC_h2p(hMem);

	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ);
	if (result != FR_OK)
	{
		return;
	}

	result = f_read(&file, _acBuffer2, file.fsize, &bw);
	if (result != FR_OK)
	{
		return;
	}

	GUI_JPEG_Draw(_acBuffer2,file.fsize,x0,y0);

	GUI_ALLOC_Free(hMem);
	f_close(&file);
}

void _ShowPNG(const char * sFilename, int x0, int y0)
{
	GUI_HMEM hMem;
	char *_acBuffer2;
	u8 result;
	u32 bw;

	hMem = GUI_ALLOC_AllocZero(1024*512);

	_acBuffer2 = GUI_ALLOC_h2p(hMem);

	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ);
	if (result != FR_OK)
	{
		return;
	}

	result = f_read(&file, _acBuffer2, file.fsize, &bw);
	if (result != FR_OK)
	{
		return;
	}

	GUI_PNG_Draw(_acBuffer2,file.fsize,x0,y0);

	GUI_ALLOC_Free(hMem);
	f_close(&file);
}

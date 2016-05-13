#include "font.h"

GUI_XBF_DATA XBF_Data;
GUI_FONT     XBF_Font;
FIL          Fontfile;

int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer)
{
	FIL *FontFile;
	u8 result;
	u32 bw;
	 
	FontFile = (FIL *)pVoid;
	 
	 
	result =f_lseek(FontFile, Off);
	if (result != FR_OK) 
	{
		return 1; 
	}
 
	result = f_read(FontFile, pBuffer, NumBytes, &bw);
	if (result != FR_OK) 
	{
		return 1; 
	}
	 
	return 0;
 
}

void _ShowXBF(void) 
{
	u8 result;
	result = f_open(&Fontfile, "yahei83.xbf", FA_OPEN_EXISTING | FA_READ);
	if (result != FR_OK)
	{
	return;
	}

	GUI_XBF_CreateFont(&XBF_Font,       // Pointer to GUI_FONT structure in RAM(3)
	&XBF_Data,         // Pointer to GUI_XBF_DATA structure in RAM
	 GUI_XBF_TYPE_PROP_AA4_EXT, // Font type to be created
	 _cbGetData,        // Pointer to callback function
	&Fontfile);        // Pointer to be passed to GetData function
}

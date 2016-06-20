#ifndef __FONT_H
#define __FONT_H	

#include "stm32f4xx.h"
#include "GUI.h"
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer);
void _ShowXBF(const char * sFilename);

#endif

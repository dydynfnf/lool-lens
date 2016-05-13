#ifndef __PICTURE_H
#define __PICTURE_H	

#include "stm32f4xx.h"
#include "GUI.h"
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

void _ShowJPG(const char * sFilename, int x0, int y0);
void _ShowPNG(const char * sFilename, int x0, int y0);
int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer);
void _ShowXBF(void);

#endif

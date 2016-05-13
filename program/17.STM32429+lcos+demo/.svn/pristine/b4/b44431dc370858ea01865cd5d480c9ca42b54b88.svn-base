#ifndef __DEMO_H
#define __DEMO_H

#include "lcos_300_224.h"
#include "GUI.h"
#include "picture.h"
#include "font.h"

#define	Up_Vscreen 0
#define	Middle_Vscreen 224
#define	Down_Vscreen 448

void time(u32 vscreen_off);
void weather(u32 vscreen_off);
void nav(u32 vscreen_off);

void _GUI_SetBkColor(u32 color);
void _GUI_SetColor(u32 color);
void move_to_vscreen(u32 vscreen_rsc_off,u32 vscreen_des_off);
void copy_from_LCD(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1);
void perspective(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1,u32 a,u32 b,u32 h);
void turn_over(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1);
void rotate_line(GUI_POINT * p_ref,GUI_POINT * p_origin,GUI_POINT * p_rotated,I16 count,I16 ang);
void draw_nav_line(GUI_POINT * p_nav_line,I16 count,u32 color,u8 width);
void draw_nav_arrow(int x,int y);
void draw_nav_destination(int x,int y);

#endif
#include "demo.h"
#include "math.h"

extern u32 FrameBuffer0[224*3][900];
extern u32 FrameBuffer1[224*3][900];
extern u32 MEM[224][900];

u32 _COLOR;

/*
设置背景色
*/

void _GUI_SetBkColor(u32 color)
{
	_COLOR=color;
	GUI_SetBkColor(color);
}

/*
设置前景色
*/

void _GUI_SetColor(u32 color)
{
	_COLOR=color;
	GUI_SetColor(color);
}

/*
虚拟屏幕移动
从vscreen_src_off移动到vscreen_dst_off
*/

void move_to_vscreen(u32 vscreen_src_off,u32 vscreen_dst_off)
{	
	u32 i = 0;
	u32 k=0;
	u32 speed=30;
	
	//图像上移
	if(vscreen_src_off<=vscreen_dst_off)
	{
		//匀速过程
		for(i=vscreen_src_off;i<vscreen_dst_off-63;i+=speed)
		{
			GUI_SelectLayer(1);
			GUI_SetOrg(0,i);
			GUI_SelectLayer(0);
			GUI_SetOrg(0,i);
			
			DMA2D_Display();
		}
		i=vscreen_dst_off-63;
		GUI_SetOrg(0,i);
		DMA2D_Display();
		//减速过程
		for(speed=5;(speed<=5)&&(speed>=1);speed-=2)
		{
			for(k=0;k<7;k++)
			{
				i+=speed;
				
				GUI_SelectLayer(1);
				GUI_SetOrg(0,i);
				GUI_SelectLayer(0);
				GUI_SetOrg(0,i);
				
				DMA2D_Display();
			}
		}
	}
	//图像下移
	else
	{
		//匀速过程
		for(i=vscreen_src_off;i>vscreen_dst_off+63;i-=speed)
		{
			GUI_SelectLayer(1);
			GUI_SetOrg(0,i);
			GUI_SelectLayer(0);
			GUI_SetOrg(0,i);
			
			DMA2D_Display();
		}
		i=vscreen_dst_off+63;
		GUI_SetOrg(0,i);
		DMA2D_Display();
		//减速过程
		for(speed=5;(speed<=5)&&(speed>=1);speed-=2)
		{
			for(k=0;k<7;k++)
			{
				i-=speed;
				
				GUI_SelectLayer(1);
				GUI_SetOrg(0,i);
				GUI_SelectLayer(0);
				GUI_SetOrg(0,i);
				
				DMA2D_Display();
			}
		}
	}
}

/*
拷贝LCD图像到内存MEM中
layer,图像在LCD的层
x0,y0,x1,y1,图像在LCD的位置
*/

void copy_from_LCD(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1)
{
	u32 i,j,px;
	u32 x=x1-x0+1,y=y1-y0+1;
	//layer0
	if(layer==0)
	{
		for(j=0;j<y;j++)
		{
			for(i=0;i<x;i++)
			{
				for(px=0;px<3;px++)
				{
					MEM[j][3*i+px]=FrameBuffer0[y0+j][3*(x0+i)+px];
				}
			}
		}
	}
	//layer1
	else if(layer==1)
	{
		for(j=0;j<y;j++)
		{
			for(i=0;i<x;i++)
			{
				for(px=0;px<3;px++)
				{
					MEM[j][3*i+px]=FrameBuffer1[y0+j][3*(x0+i)+px];
				}
			}
		}
	}
}

/*
内存MEM中的图像做透视 并拷贝到LCD
layer,图像在LCD的层
x0,y0,x1,y1,图像在LCD的位置
a,b,h,透视梯形的上底 下底 高
*/

void perspective(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1,u32 a,u32 b,u32 h)
{
	u32 x=x1-x0+1,y=y1-y0+1;
	u32 i,j,px;
	u32 x_num,x_s,x_e,y_s,y_e;
	//layer0
	if(layer==0)
	{
		y_s=(y-h)/2;
		y_e=y-(y-h)/2;
		for(j=0;j<y;j++)//行循环
		{
			if(j>=y_s && j<=y_e)
			{
				if(b>a)
				{
					x_num=a+(j-y_s)*(b-a)/h;
				}
				else
				{
					x_num=a-(j-y_s)*(a-b)/h;
				}
				x_s=(x-x_num)/2;
				x_e=x-(x-x_num)/2;
				for(i=0;i<x;i++)//列循环
				{
					if(i>=x_s && i<=x_e)
					{
						for(px=0;px<3;px++)
						{
							FrameBuffer0[y0+j][3*(x0+i)+px]=MEM[(j-y_s)*y/h][3*((i-x_s)*x/x_num)+px];
						}
					}
					else
					{
						for(px=0;px<3;px++)
						{
							FrameBuffer0[y0+j][3*(x0+i)+px]=0;
						}
					}
				}
			}
			else
			{
				for(i=0;i<x;i++)
				{
					for(px=0;px<3;px++)
					{
						FrameBuffer0[y0+j][3*(x0+i)+px]=0;
					}
				}
			}
		}
	}
	//layer1
	else if(layer==1)
	{
		y_s=(y-h)/2;
		y_e=y-(y-h)/2;
		for(j=0;j<y;j++)//行循环
		{
			if(j>=y_s && j<=y_e)
			{
				if(b>a)
				{
					x_num=a+(j-y_s)*(b-a)/h;
				}
				else
				{
					x_num=a-(j-y_s)*(a-b)/h;
				}
				x_s=(x-x_num)/2;
				x_e=x-(x-x_num)/2;
				for(i=0;i<x;i++)//列循环
				{
					if(i>=x_s && i<=x_e)
					{
						for(px=0;px<3;px++)
						{
							FrameBuffer1[y0+j][3*(x0+i)+px]=MEM[(j-y_s)*y/h][3*((i-x_s)*x/x_num)+px];
						}
					}
					else
					{
						for(px=0;px<3;px++)
						{
							FrameBuffer1[y0+j][3*(x0+i)+px]=0;
						}
					}
				}
			}
			else
			{
				for(i=0;i<x;i++)
				{
					for(px=0;px<3;px++)
					{
						FrameBuffer1[y0+j][3*(x0+i)+px]=0;
					}
				}
			}
		}
	}
	
	DMA2D_Display();
}

/*
翻转图像
x0,y0,x1,y1,图像在LCD的位置
layer,图像在LCD的层
*/

void turn_over(u32 layer,u32 x0,u32 y0,u32 x1,u32 y1)
{
	u32 i;
	u32 x=x1-x0+1,y=y1-y0+1;
	copy_from_LCD(layer,x0,y0,x1,y1);
	for(i=0;i<=100;i+=20)
	{
		perspective(layer,x0,y0,x1,y1,x-i*x/500,x,y-i*y/100);
	}
	for(i=0;i<=100;i+=20)
	{
		perspective(layer,x0,y0,x1,y1,x,4*x/5+i*x/500,i*y/100);
	}
}

/*
旋转线条
p_ref,旋转参考点
p_origin,需要旋转的线
p_rotated,旋转后的线
count,线中点数
ang,旋转角度
*/

void rotate_line(GUI_POINT * p_ref,GUI_POINT * p_origin,GUI_POINT * p_rotated,I16 count,I16 ang)
{
	float x,y,dis,a1,a2;
	u32 i;
	
	for(i=0;i<count;i++)
	{
		x=((p_origin+i)->x)-(p_ref->x);
		y=((p_origin+i)->y)-(p_ref->y);
		dis=sqrt(x*x+y*y);
		a1=atan2(y,x);
		a2=a1+ang*3.14/180;
		(p_rotated+i)->x=dis*cos(a2)+(p_ref->x);
		(p_rotated+i)->y=dis*sin(a2)+(p_ref->y);
	}
}

/*
绘制导航线
p_nav_line,导航线
count,线中点数
width,线宽度
color,颜色
*/

void draw_nav_line(GUI_POINT * p_nav_line,I16 count,u32 color,u8 width)
{
	int i=0,d=0;
	int x,y,dx,dy,num;
	int skip=10;
	
	GUI_SetPenSize(width);
	_GUI_SetColor(color);
	GUI_AA_SetFactor(6);
	
	for(i=0;i<count-1;i++)
	{
		x=((p_nav_line+i+1)->x)-((p_nav_line+i)->x);
		y=((p_nav_line+i+1)->y)-((p_nav_line+i)->y);
		
		if(abs(x)>abs(y))
		{
			if(x>0)
			{
				for(dx=0;dx<x;dx+=skip)
				{
					dy=dx*y/x;
					GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i)->x+dx),((p_nav_line+i)->y+dy));
					DMA2D_Display();
				}
				GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i+1)->x),((p_nav_line+i+1)->y));
				DMA2D_Display();
			}
			else
			{
				for(dx=0;dx>x;dx-=skip)
				{
					dy=dx*y/x;
					GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i)->x+dx),((p_nav_line+i)->y+dy));
					DMA2D_Display();
				}
				GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i+1)->x),((p_nav_line+i+1)->y));
				DMA2D_Display();
			}
		}
		else
		{
			if(y>0)
			{
				for(dy=0;dy<y;dy+=skip)
				{
					dx=dy*x/y;
					GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i)->x+dx),((p_nav_line+i)->y+dy));
					DMA2D_Display();
				}
				GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i+1)->x),((p_nav_line+i+1)->y));
				DMA2D_Display();
			}
			else
			{
				for(dy=0;dy>y;dy-=skip)
				{
					dx=dy*x/y;
					GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i)->x+dx),((p_nav_line+i)->y+dy));
					DMA2D_Display();
				}
				GUI_AA_DrawLine(((p_nav_line+i)->x),((p_nav_line+i)->y),((p_nav_line+i+1)->x),((p_nav_line+i+1)->y));
				DMA2D_Display();
			}
		}
	}
}

/*
绘制导航箭头
x,坐标
y,坐标
*/

void draw_nav_arrow(int x,int y)
{
	GUI_POINT triangle[3]={{0,-22},{-10,0},{10,0}};
	
	GUI_AA_SetFactor(6);
	GUI_SetPenSize(1);
	
	_GUI_SetColor((0xff<<24) | (185<<16) | (115<<8) | 62);
	GUI_AA_FillPolygon(triangle,3,x,y);
	
	_GUI_SetColor(0x00);
	GUI_AA_FillCircle(x,y,10);
	
	_GUI_SetColor((0xff<<24) | (185<<16) | (115<<8) | 62);
	GUI_AA_FillCircle(x,y,8);
	
	DMA2D_Display();
}

/*
绘制导航目的地
x,坐标
y,坐标
*/

void draw_nav_destination(int x,int y)
{
	GUI_POINT triangle[3]={{0,0},{-11,-21},{11,-21}};
	
	GUI_AA_SetFactor(6);
	GUI_SetPenSize(1);
	
	_GUI_SetColor((0xff<<24) | (35<<16) | (30<<8) | 173);
	GUI_AA_FillPolygon(triangle,3,x,y);
	
	_GUI_SetColor((0xff<<24) | (35<<16) | (30<<8) | 173);
	GUI_AA_FillCircle(x,y-21,11);
	
	_GUI_SetColor(0x00);
	GUI_AA_FillCircle(x,y-21,5);
	
	DMA2D_Display();
}





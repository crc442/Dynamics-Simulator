/*#if !defined(__MOUSE_H)
# define __MOUSE_H*/

#include<dos.h>
#include<conio.h>
#include<graphics.h>
#include<stdio.h>
#include<process.h>

union REGS i,o;
extern int mou_x,mou_y;

int initmouse(void)
{
	 i.x.ax = 0;
	 int86(0x33,&i, &o);
	 return(!o.x.ax);
}
void clearmousevalues(void)
{
	int button,x,y;
		getmousepos(&button,&x,&y);
	while(button&1)
		getmousepos(&button,&x,&y);
	return;
}
void restircmouseptr(int x1, int y1,int x2, int y2)
{
	 i.x.ax = 7;
	 i.x.cx = x1;
	 i.x.dx = x2;
	 int86(0x33,&i, &o);
	 i.x.ax = 8;
	 i.x.cx = y1;
	 i.x.dx = y2;
	 int86(0x33,&i,&o);
	 return;
}

void setmousepos()
{
	i.h.ah=2;
	i.h.bh=0;
	i.h.ch=0;
	i.h.cl=0;
	i.h.dh=0;
	i.h.dl=0;
	int86(0x10,&i, &o);

	printf("X:%03d Y:%03d",mou_x,mou_y);
}
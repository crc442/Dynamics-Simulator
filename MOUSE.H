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

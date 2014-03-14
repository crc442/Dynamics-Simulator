#include<dos.h>
#include<conio.h>
#include<graphics.h>
#include<stdio.h>

struct button add,gravtn,frictn,delt,exit1; //horiz
struct button rad,asp,CVel,EReport,help; //vert
extern int xm,ym;

struct button
{
	int x;
	int y;
	char name[10];
};

clicked(struct button *obj)
{
	int top,left,right,bottom;

	top=(*obj).y-10;
	left=(*obj).x-10;
	right=(*obj).x+65;
	bottom=(*obj).y+10;

	setfillstyle(SOLID_FILL,LIGHTGRAY);
	bar(left+8,top+8,right+8,bottom+8);

	setfillstyle(1,LIGHTGRAY);
	bar(left,top,right,bottom);

	setfillstyle(1,GREEN);
	bar(left+8,top,right+8,bottom);

	setcolor(WHITE);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	outtextxy(left+12,top+8,(*obj).name);
	return 0;
}
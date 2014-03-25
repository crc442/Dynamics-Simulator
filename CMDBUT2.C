#include<dos.h>
#include<conio.h>
#include<graphics.h>
#include<stdio.h>

struct button
{
	int x;
	int y;
	char name[10];
};

struct button add,gravtn,frictn,delt,exit1; //horiz
struct button rad,asp,CVel,EReport,help; //vert
extern int xm,ym;

void setbutton(struct button *obj, int x,int y, char *name)
{
	(*obj).x=x;
	(*obj).y=y;
	strcpy((*obj).name,name);
}

void drawbutton(struct button *obj)
{
	int top,left,right,bottom;

	top=(*obj).y-10;
	left=(*obj).x-10;
	right=(*obj).x+65;
	bottom=(*obj).y+10;

	setfillstyle(1,LIGHTGRAY);
	bar(left,top,right+8,bottom+8);

	setfillstyle(1,BLACK);
	bar(left+8,top+8,right+8,bottom+8);

	setfillstyle(1,GREEN);
	bar(left,top,right,bottom);

	setcolor(BLACK);
	outtextxy(left+6,top+8,(*obj).name);
}

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

hitbutton(struct button *obj, int x,int y)
{
	if(x>(*obj).x-10 && x<(*obj).x+65 && y>(*obj).y-10 && y<(*obj).y+10)
		return 1;
	else
		return 0;
}

horiz_butt()
{
	int cmdtop=45,cmdbot;
	int space=100;
	cmdbot=ym-30;

	setbutton(&add,cmdtop,cmdbot,"Add Body");
	drawbutton(&add);

	setbutton(&gravtn,cmdtop+space,cmdbot,"Gravity");
	drawbutton(&gravtn);

	setbutton(&frictn,cmdtop+2*space,cmdbot,"Friction");
	drawbutton(&frictn);

	setbutton(&delt,cmdtop+3*space,cmdbot,"Delete");
	drawbutton(&delt);

	setbutton(&exit1,cmdtop+4*space,cmdbot,"EXIT");
	drawbutton(&exit1);

	return 0;
}

vert_butt()
{
	int cmdtop=xm-60,cmdbot;
	int space=40;
	cmdbot=80;

	setbutton(&add,cmdtop,cmdbot,"Add Body");
	drawbutton(&add);

	setbutton(&gravtn,cmdtop,cmdbot+space,"Gravity");
	drawbutton(&gravtn);

	setbutton(&frictn,cmdtop,cmdbot+2*space,"Friction");
	drawbutton(&frictn);

	setbutton(&delt,cmdtop,cmdbot+3*space,"Delete");
	drawbutton(&delt);

	setbutton(&exit1,cmdtop,cmdbot+4*space,"EXIT");
	drawbutton(&exit1);

	return 0;
}

drawCommandButtons()
{
	setfillstyle(SOLID_FILL,0);
	bar(0,0,xm,ym);

	setfillstyle(SOLID_FILL,7);
	bar(xm-75,50,xm-1,ym-50);

	vert_butt();
//	horiz_butt();

	setcolor(15);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
	outtextxy(310,10,"DYNAMICS Simulation");

	return 0;
}

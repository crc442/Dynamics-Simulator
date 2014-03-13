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


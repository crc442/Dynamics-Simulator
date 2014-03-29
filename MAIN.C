#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<stdlib.h>

#include "MOUSE.H"
#include "CMDBUT2.C"
#include "TXTBOX.C"

#define MX 40
#define MY 30
#define VX1 20      //Coordinates within which simulation is done
#define VY1 50
#define VX2 xm-80

#define VY2 ym-50

struct act_vir
{
	float x;
	float y;
};
struct act_vir *av;

struct vir_act
{
	int x;
	int y;
};
struct vir_act *va;

void main()
{
	int gd=DETECT,gm;
	int i;
	int flag=0;
	char b;
	char *str;
	static int button1=2;

	initgraph(&gd,&gm,"\\tc\\bgi");
	xm=getmaxx();	ym=getmaxy();
	numb=0;
	initmouse();

	introduction();

	clearmousevalues();
	rectangle(20,50,xm-80,ym-50);
	while(1)
	{
		showmouseptr();
		button1=0;
		getmousepos(&button1,&mou_x,&mou_y);

		if((mou_x>=20 && mou_x<=xm-80) && (mou_y>=50 && mou_y<=ym-50))
			get_thrust(button1,mou_x,mou_y);
		if (kbhit())
		{
			while (kbhit()) b=getch();      // skip over typeahead and extended codes
			if(b=='d')		//delete the bodies one by one from the screen
				delet=1;
			else if(b=='c')// clear the screen
			{
				setcolor(0);
				setfillstyle(1,0);
				bar(21,51,xm-81,ym-51);
			}
		}
//		printf("%d",button1);
		setcolor(15);
		if((button1&1)==1 &&(mou_x>=xm-75 && mou_x<=xm-2) && (mou_y>=50 && mou_y<=ym-2))
		{
//		  printf("in");
			getmousepos(&button1,&mou_x,&mou_y);
			while((button1&1)==1)
			{
				hidemouse();
				getmousepos(&button1,&mou_x,&mou_y);

				if(hitbutton(&add,mou_x,mou_y))
					clicked(&add);
				else
					drawbutton(&add);

				if(hitbutton(&gravtn,mou_x,mou_y))
					clicked(&gravtn);
				else
					drawbutton(&gravtn);

				if(hitbutton(&frictn,mou_x,mou_y))
					clicked(&frictn);
				else
					drawbutton(&frictn);

				if(hitbutton(&delt,mou_x,mou_y))
					clicked(&delt);
				else
					drawbutton(&delt);

				if(hitbutton(&help,mou_x,mou_y))
					clicked(&help);
				else
					drawbutton(&help);

				if(hitbutton(&EReport,mou_x,mou_y))
					clicked(&EReport);
				else
					drawbutton(&EReport);

				if(hitbutton(&exit1,mou_x,mou_y))
					clicked(&exit1);
				else
					drawbutton(&exit1);

				showmouseptr();
				clearmousevalues();
			}
			hidemouse();
			if(hitbutton(&add,mou_x,mou_y))
			{
				drawbutton(&add);
				add_poly();//addnew();
			}
			if(hitbutton(&gravtn,mou_x,mou_y))
			{
				setcolor(1);
				gcvt(grav*10000,5,str);
				outtextxy(VX1+70,VY2+30,str);

				setcolor(15);
				drawbutton(&gravtn);
				draw_txtbox(200,180,300,"Gravitation * 10000(-10.0  - 10.0 ");
				str=gettext1();
				grav=atof(str);
				outtextxy(VX1+70,VY2+30,str);

				if(grav>10.0)grav=10.0;
				else if(grav<-10.0)grav=-10.0;
				grav /= 10000.0;
				hidebox(200,180,300,"Gravitation * 10000(-10.0  - 10.0 ");

			}
			if(hitbutton(&frictn,mou_x,mou_y))
			{
				drawbutton(&frictn);
				draw_txtbox(200,180,300,"Enter air friction constant (0.0-100.0):");
				str=gettext1();
				fric=atof(str);
				outtextxy(VX1+180,VY2+30,str);
				if(fric>100.0)fric=100.0;
				else if(fric<=0.0)fric=0.0;
				hidebox(200,180,300,"Enter air friction constant (0.0-100.0):");


			}
			if(hitbutton(&delt,mou_x,mou_y))
			{
				drawbutton(&delt);
				delet=1;
			}

			if(hitbutton(&exit1,mou_x,mou_y))
			{
				drawbutton(&exit1);
				exit(0);
			}
//			showmouseptr();
			clearmousevalues();

		}
		flag=0;
		if(numb<=0)button1=0;
		set_poly(flag);
//		set_old();
		compute_new();
		adjust_parts();
		compute_pxy();
		flag=1;
		set_poly(flag);
//		set_old();
		get_accel();
		delay(10);
//		showmouseptr();
	}
//	getch();
	closegraph();
	restorecrtmode();

}

struct virt_co *actual_vir(int x, int y);
struct virt_co *vir_actual(float x, float y);
struct virt_co *actual_vir1(int x, int y);
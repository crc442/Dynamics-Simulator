#include<graphics.h>
#include<conio.h>
#include<math.h>
#include<stdlib.h>

#include "MOUSE.H"
#include "CMDBUT2.C"
#include "TXTBOX.C"

#define MX 40
#define MY 30
#define PI 3.1415926
#define VV 0.1

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
struct virt_co *actual_vir(int x, int y);
struct virt_co *vir_actual(float x, float y);
struct virt_co *actual_vir1(int x, int y);

int xm,ym;
int numb;
int wall=5;
int mou_x;int mou_y;

typedef float fa[50];

float scut=0.9;   // collision center adjustment
float rest=1.0;   // coefficient of restitution (coefficient of static friction)
float fric=0.001;   // air friction
float grav=0.0;   // gravity
float slid=0.0;       // sliding friction
int delet=0;

struct body
{
  int col;
  int sides;
  int r;
  float m;           //  mass
  float i;           //  coefficient of moment of inertia ( = I/m )
  fa sx,sy;          //  shape (center of mass is always origin)
  fa px,py;          //- corner points
  float cx,cy,a;
//  float ocx,ocy,oa;
  float vx,vy,w;     //  velocity
  float kx,ky;       //  my offset to pivot <or> abs tot center of mass
  float tm,ti;       //- total mass and moment coeff
};
struct body bod[50];
int p[50];

introduction()
{
	int dec,sign;
	char *str;
	drawCommandButtons();
	settextjustify(LEFT_TEXT,TOP_TEXT);

	setcolor(0);
	setfillstyle(SOLID_FILL,0);
	bar(VX1-1,VY1-1,VX2+1,VY2+1);
	setcolor(15);
//	rectangle(VX1-1,VY1-1,VX2+1,VY2+1);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	outtextxy(VX1-20,VY2,"0");
	outtextxy(VX2-20,VY2+10,"40");
	outtextxy(VX2-300,VY2+5,"X-axis");

	outtextxy(VX1-20,VY1,"30");
	settextstyle(DEFAULT_FONT,VERT_DIR,1);
	outtextxy(VX1-5,VY2-200,"Y-axis");

	setfillstyle(SOLID_FILL,1);
	bar(20,ym-30,xm-2,ym-5);

	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
//	setcolor(0);
	outtextxy(VX1+5,VY2+30,"Gravity:");
	gcvt(grav,5,str);
	outtextxy(VX1+70,VY2+30,str);

	outtextxy(VX1+100,VY2+30,"Friction:");
	gcvt(fric,5,str);
	outtextxy(VX1+180,VY2+30,str);

//	setcolor(15);
	return 0;
}

int get_thrust(int b,int x, int y)
{
//	 static int x=0,y=0;
	 int i,j,k;
	 float d,t;
	 if (b)
	 {
		av = actual_vir(x,y);

		if (b<3)
		{
			d = 99999;                //  find closest body
			for (j=i=0; i<numb; ++i)
			{
				t = hypot(av->x-bod[i].cx,av->y-bod[i].cy);
				if (t < d)
				{
					d=t;
					j=i;
				}
			}
			if (d >0.00001)
			{
				bod[j].vx = (av->x-bod[j].cx)*VV;
				bod[j].vy = (av->y-bod[j].cy)*VV;

			}
			//printf("%f",d);
			if (b>1) bod[j].w = 0;           // stop spin
		}
		else
		{                                // attract everything
			for (j=0; j<numb; ++j)
			{
				bod[j].vx = (av->x-bod[j].cx)*VV*0.1;
				bod[j].vy = (av->y-bod[j].cy)*VV*0.1;
			}
		}
  }
  return 0;
}
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

int adjust_parts() {
  int b,r,j;
  float ca,sa,px,py,rx,ry;

  for (b=0; b<numb; ++b)
  {
	 //	bod[b].jb = 0;         // num sub parts
		bod[b].kx = bod[b].cx;
		bod[b].ky = bod[b].cy;
		bod[b].tm = bod[b].m;
		bod[b].ti = bod[b].i*bod[b].m;
  }
  return 0;
}

int get_accel(void)
{
  int t,t1,t2;
  int i1,b1,b2,r;
  static float x1,x2,y1,y2,v1,c1,w1,a;
  static float z,ri;
//  ri=1.0;

  // test if any body bumped into a wall
  for (b1=0; b1<numb; ++b1)
  {
	 x1=MX; x2=0;
	 for (i1=0; i1<bod[b1].sides; ++i1)
	 {
		if (x1 > bod[b1].px[i1])
		{        // find leftmost
		  x1=bod[b1].px[i1];
		  y1=bod[b1].py[i1];
		}
		if (x2 < bod[b1].px[i1])
		{        // find rightmost
		  x2=bod[b1].px[i1];
		  y2=bod[b1].py[i1];
		}
	 }
	 if (x2 > MX)
	 {
		x1=x2-MX;
		y1=y2;
		wall|=2;
	 }
	 else if(x1 > 0)
		x1=0;

	 else
		wall|=8;

	 if (x1)
	 {
		ri = bod[b1].ti/bod[b1].tm;
		y1 = bod[b1].cy - y1;
		w1 = bod[b1].w;
		v1 = bod[b1].vx;
		a = y1*y1/ri;
		c1 = ((a-rest)*v1 - y1*(1+rest)*w1)/(a+1);
		bod[b1].vx = c1;
		bod[b1].w = y1*(c1 - v1)/ri + w1;    // - jw ??
		bod[b1].cx -= scut*x1;
	 }

	 y1=MY; y2=0;

	 for (i1=0; i1<bod[b1].sides; ++i1)
	 {
		if (y1 > bod[b1].py[i1])
		{        // find bottommost
//		  printf("y1=%f > %f\n",y1,bod[b1].py[i1]);
		  x1=bod[b1].px[i1];
		  y1=bod[b1].py[i1];
		}
		if (y2 < bod[b1].py[i1])
		{        // find topmost
		  x2=bod[b1].px[i1];
		  y2=bod[b1].py[i1];
		}

	 }
	 if (y2 > MY)
	 {
		y1=y2-MY;
		x1=x2; wall|=1;
	 }
	 else if (y1 > 0) y1=0;
	 else wall|=4;
//	 printf("y1=%f y2=%f\n",y1,y2);
//	 delay(100);
	 y2=y1;
//	 getch();
	 if (y2)
	 {

		ri = bod[b1].ti/bod[b1].tm;
		y1 = -bod[b1].cx + x1;
		w1 = bod[b1].w;
		v1 = bod[b1].vy;
		a = y1*y1/ri;
		c1 = ((a-rest)*v1 - y1*(1+rest)*w1)/(a+1);
		bod[b1].vy = c1;
		bod[b1].w = y1*(c1 - v1)/ri + w1;   // - jw ??
		bod[b1].cy -= scut*y2;
		/*----------------------------------------------------------
			Do sliding friction on the floor (or ceiling)
		*/
		z = bod[b1].vx;              // relative sliding velocity
		z *= 1-1/(slid+1);
		bod[b1].vx -= z;
	 }
//	 printf("bod[%d]=(%f , %f , %f)",b1,bod[b1].cx,bod[b1].cy,bod[b1].a);
//	 delay(100);

  }
	for (b1=0; b1<numb; ++b1)
	{
	 z = hypot(bod[b1].vx,bod[b1].vy);
	 z = 1/(1+z*fric);                // air friction
	 bod[b1].vy *= z;
	 bod[b1].vx *= z;
	 bod[b1].vy -= grav*bod[b1].m;             // gravity

	 t = 0;
	 if (bod[b1].cx < 0) { t=1;  bod[b1].cx = 0; }
	 if (bod[b1].cx >MX) { t=1;  bod[b1].cx = MX; }
	 if (bod[b1].cy <0) { t=1;  bod[b1].cy = 0; }
	 if (bod[b1].cy >MY) { t=1;  bod[b1].cy = MY; }
	 if (t)
	 {
		bod[b1].vx *= 0.9;          // slow it down!
		bod[b1].vy *= 0.9;
		bod[b1].w *= 0.9;
	 }
  }
  return 0;
}


redraw_all(int flag,int sides,int col)
{
	int i;
	int sidewall=50;
	int sidewall2=80;
//	hidemouse();
	if(flag==0)
	{
		setcolor(0);
		drawpoly(sides,p);
	}
	else
	{
		setcolor(col);
	//	hidemouse();
		drawpoly(sides,p);
	}
//	showmouseptr();
	setcolor(15);
	if (wall&1) line(20,50,xm-80,50);
	if (wall&2) line(xm-80,50,xm-80,ym-50);
	if (wall&4) line(xm-80,ym-50,20,ym-50);
	if (wall&8) line(20,ym-50,20,50);

	wall=0;

	return 0;
}
/*int set_old(void)
{
  int b;
  for (b=0; b<numb; ++b)
  {
	 bod[b].ocx = bod[b].cx;
	 bod[b].ocy = bod[b].cy;
	 bod[b].oa  = bod[b].a ;
  }
  return 0;
} */
/*----------------------------------------------------------------------
	computes new c,a  +=  new v,w
*/
int compute_new(void)
{
  int b;
  float sa,ca;
  float sx,sy;
  for (b=0; b<numb; ++b)
  {
	bod[b].cx += bod[b].vx + (bod[b].cy - bod[b].cy)*bod[b].w;
	bod[b].cy += bod[b].vy + (bod[b].cx - bod[b].cx)*bod[b].w;
	bod[b].a  += bod[b].w;
  }
  return 0;
}
int compute_pxy(void)
{
  int i,b;
  float cx,cy,ca,sa;

  for (b=0; b<numb; ++b)
  {
	 cx = bod[b].cx;
	 cy = bod[b].cy;
	 ca = cos(bod[b].a);  //rotation
	 sa = sin(bod[b].a);  //rotation

	 for (i=0; i<bod[b].sides; ++i)
	 {
		bod[b].px[i] = cx + bod[b].sx[i]*ca - bod[b].sy[i]*sa;
		bod[b].py[i] = cy + bod[b].sx[i]*sa + bod[b].sy[i]*ca;
	 }
  }
  return 0;
}
int set_poly(int flag)
{
  int i,b;

//  int sides=4;
  for (b=0; b<numb; ++b)
  {
	 for (i=0; i<bod[b].sides; ++i)
	 {
			va=vir_actual(bod[b].px[i],bod[b].py[i]);
			p[i*2]=va->x;	p[2*i+1]=va->y;
	 }
	 p[i*2]=p[0];	p[2*i+1]=p[1];
	 if(flag==0)
		 redraw_all(0,bod[b].sides+1,0);
	 else if(flag==1 && b<numb-delet)
		redraw_all(1,bod[b].sides+1,bod[b].col);
  }
  numb-=delet;
  if(numb<0)numb=0;
  delet=0;

  return 0;
}

add_poly()
{
	float radius,aspect;
	int sides=4;
	int b,i;
	int bod_count=5;
	float m,a;
	char *in_str;
	int bod_color;
	radius=1.0;aspect=1.0;

	randomize();
  draw_txtbox(200,180,300,"No. of Bodies < 20");
  bod_count=atoi(gettext1());
  if(bod_count>20)bod_count=20;
  if(bod_count<0)bod_count=0;

  draw_txtbox(200,240,300,"Sides");
  sides=atoi(gettext1());
  if(sides>10)sides=10;
  if(sides<0)sides=0;

  draw_txtbox(200,300,300,"Mass");
  m=atof(gettext1());
  if(m<=0)m=50.0;
//  printf("%f",m);

  hidebox(200,180,300,"No. of Bodies < 20");
  hidebox(200,240,300,"Sides");
  hidebox(200,300,300,"Mass");

  memset(&bod[numb],0,sizeof *bod);

  bod[numb].cx = bod[numb].cy = 0.5;
  bod[numb].vx=  bod[numb].vx= 0.05;

//  m=3000.0;//PI*radius*radius*aspect*100.0;
  a=radius*radius*aspect;

  bod[numb].r = radius*aspect;
  bod[numb].m = m;
  bod[numb].i = a;
  bod_color=rand()%15+1;
  for (i=numb; i<numb+bod_count; ++i)
  {
	 bod[i]=bod[numb];
	 bod[i].col=bod_color;//
	 bod[i].sides=sides;
	 if (i>numb)
	 {
		bod[i].cy = bod[i-1].cy+ 2*radius;  //placing the subsequent bodies on the screen such that they don't merge
		bod[i].cx = bod[i-1].cx;
	 }
	 if (bod[i].cx+radius > MX) //if the stacked body exceeds the limit of Y-co ordinate
	 {
		bod[i].cy = 0.5;
		bod[i].cx = bod[i-1].cx+ 2*radius*aspect;
	 }
	 for(b=0;b<bod[i].sides;++b)
	 {
		bod[i].sx[b]=radius*cos(PI*2*(b+0.5)/bod[i].sides)*-aspect;
		bod[i].sy[b]=radius*sin(PI*2*(b+0.5)/bod[i].sides);
	 }
	 bod[i].w=0.0;
	}
	numb+=bod_count;
	return 0;
}
struct virt_co *actual_vir(int x, int y)
{
	av->x=((1.0*av->x/xm)*(xm-100))+20;	//formula for windows x-co-ordinate in viewport
	av->x=1.0*x/xm*MX;

	av->y=((1.0*av->y*ym)/(ym-100))+50;//formula for windows y-co-ordinate in viewport
	av->y=(1-1.0*y/ym)*MY;	//1-1.0*mou_y/ym)*MY;

	return(av);
}
struct virt_co *actual_vir1(int x, int y)
{
	av->x=1.0*x/xm*MX;
	av->y=(1-1.0*y/ym)*MY;	//1-1.0*mou_y/ym)*MY;

	return(av);
}

struct virt_co *vir_actual(float x, float y)
{

	va->x=(x*xm/MX);
	va->x=((1.0*va->x/xm)*(xm-100))+20;	//formula for windows x-co-ordinate in viewport
	va->y=(MY-y)*ym/MY;
	va->y=((1.0*va->y/ym)*(ym-100))+50;//formula for windows y-co-ordinate in viewport
	return(va);
}
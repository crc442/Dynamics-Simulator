#include<dos.h>
#include<bios.h>
#include<alloc.h>
#include<dir.h>
#include<ctype.h>
#define ESC 283
#define LEFT 19200
#define RIGHT 19712
#define HOME 18176
#define END 20224
#define ENTER 7181
#define INSERT 20992
#define DEL 21248
char *ch[1]={'\0'};
int leftx,lefty,rightx;

void draw();
void showbox(int,int,int,char *);
void hidebox(int,int,int,char *);

void cursor(char *,int,int,int);    //draw & erase cursor
void specialkey(char *,int,int,int,int *,int *,int *);   // home,end,left,right,del,insert
void keycheck(char *,int *,int,int *);
void backspace(char *,int *,int,int,int);
void insert(char *,int *,int,int,int);
void notback(char *,int *,int *,int,int);
char* gettext1(void);


// struct txtfld
// {
// 	int x;
// 	int y;
// 	int r;
// 	char *feild_name;
// };
// struct txtfld t1;
void draw_txtbox(int leftx,int lefty,int rightx,char *fname)
{
	 int pos=0,insflg=0,tempflg=0;
	 int i=-1,row,j=0,temp,col,maxsize;
	 char c[1];


	 ch[0]=(char*)calloc(sizeof(char),20);

	 row=lefty+7;
	 col=leftx+5;

	 maxsize=(rightx-leftx)/8 - 2;

	 settextstyle(0,0,1);
	 setcolor(15);
//	 outtextxy(leftx,lefty-10,fname);
	 showbox(leftx,lefty,rightx,fname);

	 do
	 {

		i=i+1;

		cursor(ch[j],col,row,pos);

		ch[j][i]=temp=bioskey(0);
//		printf("%c",ch[j][i]);
		ch[j][i+1]='\0';
		if(temp == HOME||temp == END||temp ==LEFT||temp ==RIGHT||temp ==DEL||temp ==INSERT)
		{
			 specialkey(ch[j],col,row,temp,&pos,&i,&insflg);
			 continue;
		}

		if(ch[j][i]=='\x1B' || ch[j][i]=='\x0D')          //escape
		{
			 ch[j][i]='\0';
//			 hidebox(leftx,lefty,rightx);
 //			 hidebox(leftx,lefty,rightx,fname);
			 break;
		}

		if(ch[j][i]!='\t' && ch[j][i]!='\x1B' && ch[j][i]!='\b')
		{
			 if(i==maxsize)
			 {
				ch[j][i]='\0';
				i=i-1;
				continue;
			 }
			 keycheck(ch[j],&i,5,&tempflg);
			 if(tempflg==1)
			 {
				tempflg=0;
				continue;
			 }

		}
		if(ch[j][i]=='.')        //.(full stop)
		{
			 ch[j][i]='.';
		}

		setcolor(15);
		outtextxy((strlen(ch[j])-1)*8+col,row+2,"_");
		setcolor(0);

		if(ch[j][i]=='\b')        //backspace
		{
			 backspace(ch[j],&i,pos,row,col);
			 continue;
		}

		if(insflg%2==1)
		{
			 insert(ch[j],&i,pos,row,col);
			 continue;
		}
//		if(ch[j][i]!='\b')
//		{
			 if(pos>0)
			 {
				setfillstyle(1,15);
				bar(strlen(ch[j])*8+col-pos*8-8,row,strlen(ch[j])*8+col-pos*8-1,row+7);
				ch[j][i-pos]=ch[j][i];
				ch[j][i]='\x0';
				sprintf(c,"%c",ch[j][i-pos]);
				setcolor(0);
				outtextxy(strlen(ch[j])*8+col-pos*8,row,c);
				pos=pos-1;
				i=i-1;
			 }
			 else
			 {
				sprintf(c,"%c",ch[j][i]);
				setcolor(0);
				outtextxy(strlen(ch[j])*8+col-8-pos*8,row,c);
			 }
  //		}
	 }while(ch[j][i]!='\x1B' || ch[j][i]!='\x0D');
}


void showbox(int lx,int ly,int rx,char *name)
{
	 setlinestyle(0,0,1);

	 setcolor(WHITE);
	 setfillstyle(1,LIGHTGRAY);
	 bar(lx-3,ly-3,rx+3,ly+23);

	 setfillstyle(1,WHITE);
	 outtextxy(lx,ly-13,name);
	 bar(lx,ly,rx,ly+20);

	 setcolor(BLACK);
	 rectangle(lx,ly,rx,ly+20);

	 line(rx-1,ly,rx-1,ly+20);
	 line(lx,ly+20-1,rx,ly+20-1);

}

void hidebox(int lx,int ly,int rx,char *name)
{
	 setlinestyle(0,0,1);

	 setfillstyle(1,BLACK);
	 bar(lx-3,ly-3,rx+3,ly+23);
	 setcolor(BLACK);
	 outtextxy(lx,ly-13,name);
	 bar(lx,ly,rx,ly+20);
	 rectangle(lx,ly,rx,ly+20);
	 line(rx-1,ly,rx-1,ly+20);
	 line(lx,ly+20-1,rx,ly+20-1);

}

void cursor(char *fchr,int fcol,int frow,int fpos)
{
	while (!kbhit())
	{
		setcolor(0);
		outtextxy(strlen(fchr)*8+fcol-fpos*8,frow+2,"_");
		delay(100);
		setcolor(15);
		outtextxy(strlen(fchr)*8+fcol-fpos*8,frow+2,"_");
		delay(100);
	}
}



void main()
{
	int driver=EGA;
	int mode=EGAHI;
	int i,j;
	char *str[1];

	j=0;
	initgraph(&driver,&mode,"\\tc\\bgi");
//	TextField t1(120,200,250);

	setfillstyle(1,7);
	bar(0,0,getmaxx(),getmaxy());
	setcolor(0);
	settextstyle(1,0,2);
	draw_txtbox(200,180,450,"bodies");
	strcpy(*str,gettext1());
	for(i=0;i<=strlen(ch[j]);i++)
	 printf("%c",str[j][i]);
//    printf("%s",ch);
//    outtextxy(20,20,ch);
	getch();
}

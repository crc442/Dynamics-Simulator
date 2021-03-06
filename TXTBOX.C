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

/*struct txtfld
{
	int x;
	int y;
	int r;
	char *feild_name;
};
struct txtfld t1;*/
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
void notback(char *fchr,int *fi,int *fpos,int fcol,int frow)
{
	char c[1];
	int a,b,x,y;

	a=*fpos;
	b=*fi;
	if(*fpos>0)
	{
		 setfillstyle(1,15);
		 bar(strlen(fchr)*8+fcol-*(fpos) * 8-8,frow,strlen(fchr)*8+fcol-*fpos*8-1,frow+7);
		 *(fchr + (b - a))=*(fchr + b);
		 *(fchr + b)='\x0';
		 sprintf(c,"%c",*(fchr + (b - a)));
		 setcolor(0);
		 outtextxy(strlen(fchr)*8+fcol-a*8,frow,c);
		 a=a-1;
		 b=b-1;
	}
	else
	{
		sprintf(c,"%c",*(fchr+ b));
		setcolor(0);
		outtextxy(strlen(fchr)*8+fcol-8-a*8,frow,c);
	}
	*fpos=a;
	*fi=b;
}

void insert(char *fchr,int *fi,int fpos,int frow,int fcol)
{
	 char c[1];
	 setfillstyle(SOLID_FILL,WHITE);
	 bar(strlen(fchr)*8+fcol-fpos*8-8,frow,strlen(fchr)*8+fcol,frow+8);
	 *(fchr + (*fi+1))=*(fchr + *fi);
	 *(fchr + (*fi+2))='\x0';
	 while(*fi>strlen(fchr)-fpos-2)
	 {
		*(fchr + *fi)=*(fchr + (*fi-1));
		sprintf(c,"%c",*(fchr + *fi));
		outtextxy(*fi * 8 +fcol,frow,c);
		*fi=*fi-1;
	 }
	 *fi=strlen(fchr);
	 *(fchr + (*fi-fpos-2))=*(fchr + (*fi-1));
	 *(fchr + (*fi-1))='\x0';
	 sprintf(c,"%c",*(fchr + (*fi-fpos-2)));
	 outtextxy(*fi * 8 + fcol-(fpos+2)*8,frow,c);
	 *fi=*fi-2;
}

void backspace(char *fchr,int *fi,int fpos,int frow,int fcol)
{
	 char c[1];
	 if (*fi!=0 && (*fi-fpos!=0))
	 {
		 *(fchr + *fi)='\x0';
		 setfillstyle(SOLID_FILL,15);
		 bar(strlen(fchr)*8+fcol-fpos*8-8,frow,strlen(fchr)*8+fcol,frow+8);
		 *fi=strlen(fchr)-fpos-1;
		 while(*(fchr + *fi)!='\0')
		 {
			*(fchr + *fi) = *(fchr + (*fi + 1));
			sprintf(c,"%c",*(fchr + *fi));
			outtextxy(*fi * 8 + fcol,frow,c);
			*fi = *fi + 1;
		 }
		 *fi=strlen(fchr);
	 }
	 *(fchr + *fi)='\x0';
	 *fi=*fi-1;
}

void keycheck(char *fchr,int *fi,int fj,int *fl)
{
		 switch(fj)
		 {
			 case 0:
			 case 1:
			 if(!isalpha(*(fchr+ *fi)))
			 {
				  *fi=*fi-1;
				  if (isspace(*(fchr+ (*fi+1))))
				  {
					  *fi=*fi+1;
					  break;
				  }
				 *(fchr+ (*fi+1))='\x0';
				 *fl=1;
				 return;
			}
			break;

		 case 2:

		 case 5:
		 if(*(fchr+ *fi)=='.' || *(fchr+ *fi)=='-')break;
		 if(!isdigit(*(fchr+ *fi)))
		 {
			 *fi=*fi-1;
			 *(fchr+ (*fi+1))='\x0';
			 *fl=1;
			 return;
		 }
		 break;
		 case 3:
		 case 4:
		 if(!isalnum(*(fchr + *fi)))
		 {
			*fi=*fi-1;
			if (isspace(*(fchr + (*fi + 1))))
			{
			  *fi=*fi+1;
			  break;
			}
			*(fchr + (*fi + 1))='\x0';
			*fl=1;
			return;
		 }
		 break;
	}
}
void specialkey(char *fchr,int fcol,int frow,int ftemp,int *fpos,int *fi,int *finsflg)
{
	char c[1];
	switch(ftemp)
	{
		 case HOME:
				setcolor(15);
				outtextxy(strlen(fchr)*8+fcol-*fpos*8,frow+2,"_");
				*fpos=strlen(fchr);
				*fi=*fi-1;
				break;
		 case END:
				setcolor(15);
		      outtextxy(strlen(fchr)*8+fcol-*fpos*8,frow+2,"_");
				*fpos=0;
				*fi=*fi-1;
		      break;
		 case LEFT:
				if((*fi-*fpos)>0)
				{
				  setcolor(15);
				  outtextxy(strlen(fchr)*8+fcol-*fpos*8,frow+2,"_");
				  *fpos=*fpos+1;
				}
				*fi=*fi-1;
				break;
		 case RIGHT:
				if(*fpos>0)
				{
					 setcolor(15);
					 outtextxy(strlen(fchr)*8+fcol-*fpos*8,frow+2,"_");
					 *fpos=*fpos-1;
				}
				*fi=*fi-1;
				break;
		 case INSERT:
			*finsflg=*finsflg+1;
			*fi=*fi-1;
			break;
		 case DEL:
			 if(*fpos!=0)
			 {
				setfillstyle(SOLID_FILL,15);
				bar(strlen(fchr)*8+fcol-*fpos*8,frow,strlen(fchr)*8+fcol,frow+8);
				*fi=*fpos;
				setcolor(0);
				while(*fpos>0)
				{
					 fchr[strlen(fchr)-*fpos]=fchr[strlen(fchr)-*fpos+1];
					 *fpos=*fpos-1;
					 sprintf(c,"%c",fchr[strlen(fchr)-*fpos]);
					 outtextxy(strlen(fchr)*8+fcol-*fpos*8-8,frow,c);
				}
				*fpos=*fi-1;
				*fi=strlen(fchr)-1;
			 }
			 else
				*fi=*fi-1;
				 break;
		 default:
				break;
	}
}
char* gettext1()
{
	return(*ch);
}

/*void main()
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
}*/

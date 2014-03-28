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

struct virt_co *actual_vir(int x, int y);
struct virt_co *vir_actual(float x, float y);
struct virt_co *actual_vir1(int x, int y);
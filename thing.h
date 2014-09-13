#ifndef THING_H
#define THING_H
#include <math.h>
#include <stdlib.h>


typedef struct Thing
{
	int a;
	int b;
} Thing;

Thing* CreateThing(int a_, int b_);

#endif
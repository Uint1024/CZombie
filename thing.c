#include "thing.h"

Thing* CreateThing(int a_, int b_)
{
	Thing* th = (Thing*)malloc(sizeof(Thing));
	th->a = a_;
	th->b = b_;

	return th;
}
#include "common.h"
#include "entity.h"
#include "math.h"

Main_Category  button_category_g[NB_OF_LEVEL_EDITOR_BUTTONS];
//Main_Category button_category_g = (Main_Category)malloc(sizeof(Main_Category) * NB_OF_CAT);




float C_AngleBetween2Points(int xa, int ya, int xb, int yb)
{
	float angle     =    atan2f(yb - ya, xb - xa);

	return angle;
}

//angle between the middle of 2 entities
float C_AngleBetween2Entities(Entity* a, Entity* b)
{
    int xa = a->x       +   a->box.width/2;
    int ya = a->y       +   a->box.height/2;
    int xb = b->x       +   b->box.width/2;
    int yb = b->y       +   b->box.height/2;



	return C_AngleBetween2Points(xa, ya, xb, yb);
}

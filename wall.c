#include <math.h>
#include <stdlib.h>
#include "wall.h"
//#include "boundingBox.h"
#include "entity.h"

//create a plain wall at the tile x:y
Entity* Wall_Create(int x_, int y_)
{
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	ent->t = Wall;
	ent->x = x_;
	ent->y = y_;
	ent->dx = 0;
	ent->dy = 0;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}

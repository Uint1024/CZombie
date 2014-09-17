#include <math.h>
#include <stdlib.h>
#include "wall.h"
//#include "boundingBox.h"
#include "entity.h"

//create a plain wall at the tile x:y
Entity* Wall_Create(int x_, int y_)
{
	Entity* ent = Entity_Spawn();
	ent->t = Wall_cat;
	ent->texture = Wall_tex;
	ent->x = x_;
	ent->y = y_;
	ent->dx = 0;
	ent->dy = 0;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}

Entity* Ground_Create(Ground_Type type, float x, float y)
{
    Entity* ent = Entity_Spawn();
    ent->t = Ground_cat;

    switch(type)
    {
    case Grass_ground:
        ent->texture = GrassGround_tex;
        break;
    case Dirt_ground:
        ent->texture = DirtGround_tex;
    }

	ent->x = x;
	ent->y = y;
	ent->dx = 0;
	ent->dy = 0;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}


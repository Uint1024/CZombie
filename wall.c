#include <math.h>
#include <stdlib.h>
#include "wall.h"
//#include "boundingBox.h"
#include "entity.h"

//create a plain wall at the tile x:y

Entity* Wall_CreateEmpty()
{
    Entity* ent = Entity_Spawn();
        ent->visible = false;
        ent->solid = false;

        return ent;
}

Entity* Wall_Create(Wall_Type type, int x_, int y_)
{
	Entity* ent = Entity_Spawn();
	ent->t = Cat_Wall;
	ent->texture = all_textures_g[Cat_Wall][type];
	ent->x = x_;
	ent->y = y_;

	ent->hp = 1050;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}

Entity* Ground_Create(Ground_Type type, float x, float y)
{
    Entity* ent = Entity_Spawn();
    ent->t = Cat_Ground;

    ent->texture = all_textures_g[Cat_Ground][type];

	ent->x = x;
	ent->y = y;

	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}


void Structure_GetAttacked(Entity* d, Entity* attacker)
{
    if((d->hp -= attacker->damage) <= 0)
    {
        Structure_Die(d);
    }
}

void Structure_Die(Entity* d)
{
    //d->texture = door_textures_g[Door_Dead];
    d->solid = false;
    d->visible = false;
}


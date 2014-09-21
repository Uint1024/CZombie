#include <math.h>
#include <stdlib.h>
#include "wall.h"
//#include "boundingBox.h"
#include "entity.h"

//create a plain wall at the tile x:y
Entity* Wall_Create(Wall_Type type, int x_, int y_)
{
	Entity* ent = Entity_Spawn();
	ent->t = Cat_Wall;
	ent->texture = wall_textures_g[type];
	ent->x = x_;
	ent->y = y_;
	ent->dx = 0;
	ent->dy = 0;
	ent->hp = 1050;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}

Entity* Ground_Create(Ground_Type type, float x, float y)
{
    Entity* ent = Entity_Spawn();
    ent->t = Cat_Ground;

    ent->texture = ground_textures_g[type];

	ent->x = x;
	ent->y = y;
	ent->dx = 0;
	ent->dy = 0;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}


void Structure_GetAttacked(Entity* d, Entity* attacker)
{
    printf("%d\n", d->hp);
    if((d->hp -= attacker->damage) <= 0)
    {
        Structure_Die(d);
    }
}

void Structure_Die(Entity* d)
{
    //d->texture = door_textures_g[Door_Dead];
    d->solid = Jfalse;
    d->visible = Jfalse;
}


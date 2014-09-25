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
//	ent->texture = all_textures_g[Cat_Wall][type];
	ent->x = x_;
	ent->y = y_;

	ent->hp = 50;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);
    //ent->solid = false;
    if(type == Wall_Glass)
    {
        ent->transparent = true;
    }
	return ent;
}

bool Wall_IsWall(Entity* w)
{
    if(w!= NULL)
    {
        return (w->solid && w->visible && !w->transparent);
    }
    else
    {
        return false;
    }
}

Entity* Ground_Create(Ground_Type type, float x, float y)
{
    Entity* ent = Entity_Spawn();
    ent->t = Cat_Ground;
    ent->sub_category = type;
//    ent->texture = all_textures_g[Cat_Ground][type];

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
    Sound_PlayWallDestroyed();
    d->alive = false;
    //d->solid = false;
    //d->visible = false;
    printf("%d\n", d->solid);

}


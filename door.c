#include "door.h"
#include "entity.h"


Entity* Door_Create(Door_Type type, int x, int y)
{
	Entity* ent = Entity_Spawn();
	ent->t = Cat_Door;
	ent->texture = door_textures_g[type];
	ent->x = x;
	ent->y = y;
	ent->dx = 0;
	ent->dy = 0;
	ent->hp = 250;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}

/*void Door_Update(entity d)
{
    d->solid = Jtrue;
}*/

void Door_GetAttacked(Entity* d, Entity* attacker)
{
    if((d->hp -= attacker->damage) <= 0)
    {
        Door_Die(d);
    }
}

void Door_Die(Entity* d)
{
    d->texture = door_textures_g[Door_Dead];
    d->solid = Jfalse;
}

void Door_Switch(Entity* d)
{
    if(SDL_GetTicks() - d->door_opening_timer > 150)
       {
            d->door_opening_timer = SDL_GetTicks();
            d->solid = d->solid ? Jfalse : Jtrue;
       }

}

void Door_Open(Entity* d)
{
    d->solid = Jfalse;
}

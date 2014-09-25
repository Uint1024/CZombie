#include "door.h"
#include "entity.h"
#include "world.h"
#include "sound.h"


static int door_switch_timer = 0;

Entity* Door_Create(Door_Type type, int x, int y)
{
	Entity* ent = Entity_Spawn();
	ent->t = Cat_Door;
	ent->sub_category = Door_Normal;
//	ent->texture = all_textures_g[Cat_Door][type];
	ent->x = x;
	ent->y = y;
	ent->hp = 30;
	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}


void Door_GetAttacked(Entity* d, Entity* attacker)
{
    if((d->hp -= attacker->damage) <= 0)
    {
        Door_Die(d);
    }
}

void Door_Die(Entity* d)
{
    //d->texture = all_textures_g[Cat_Door][Door_Dead];
    d->solid = false;
}

void Door_Switch(Entity* d)
{
    if(SDL_GetTicks() - door_switch_timer > 150)
    {
        door_switch_timer = SDL_GetTicks();
        d->solid = d->solid ? false : true;
    }

}

void Door_Open(Entity* d)
{
    if(d->solid)
    {
        //printf("lol");
       Sound_PlayOpenDoor();
        d->solid = false;
    }

}

void Door_Update(Entity* d, World* w)
{
    if(BoundingBox_CheckSimpleCollision(&d->box, &w->player.box))
    {
        Door_Open(d);
    }
    else
    {
        d->solid = true;
    }
}

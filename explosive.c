#include <SDL.h>
#include <math.h>
#include "explosive.h"
#include "entity.h"

Explosive* ExplosiveComponent_Create(float x, float y, float angle, float speed,
                            float destinationX, float destinationY)
{
    Explosive* explosive = (Explosive*)malloc(sizeof(Explosive));

    explosive->activated = Jtrue;
	explosive->damage = 50;
	explosive->explosion_timer = 2500;
	explosive->explosion_radius = 200;
	explosive->destinationX = destinationX;
	explosive->destinationY = destinationY;




    return explosive;
}

Entity* Grenade_Create(float x, float y, float angle, float speed,
                            float destinationX, float destinationY)
{
    Entity* grenade = Entity_Spawn();

    grenade->t = Grenade;
	grenade->texture = Bullet_tex;
	grenade->x = x;
	grenade->y = y;
	grenade->box.height = 10;
	grenade->box.width = 10;
	grenade->angle = angle;
	grenade->speed = speed;
    grenade->explosive_component = ExplosiveComponent_Create(x, y, angle, speed, destinationX, destinationY);

    return grenade;
}

void Grenade_Update(Entity* g, int delta, World* world)
{
    g->explosive_component->explosion_timer -= delta;

    if(g->explosive_component->explosion_timer <= 0)
    {
        g->alive = Jfalse;
    }
    g->dx = cos(g->angle) * g->speed * delta;
	g->dy = sin(g->angle) * g->speed  * delta;

    if(abs(g->x - g->explosive_component->destinationX) < 10)
    {
        g->dx = 0;
        g->dy = 0;
    }

    moveEntity(g, g->dx, g->dy);
}

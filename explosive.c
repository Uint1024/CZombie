#include <SDL.h>
#include <math.h>
#include "explosive.h"
#include "entity.h"
#include "movement_component.h"

Explosive* ExplosiveComponent_Create(float x, float y, float angle, float speed,
                            float destinationX, float destinationY)
{
    Explosive* explosive = (Explosive*)malloc(sizeof(Explosive));

    explosive->activated = true;
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

    grenade->t = Cat_Grenade;
	grenade->texture = Tex_Bullet;
	grenade->x = x;
	grenade->y = y;
	grenade->box.height = 10;
	grenade->box.width = 10;
	grenade->movementC = MovementC_Create();
	grenade->movementC->angle = angle;
	grenade->movementC->speed = speed;
    grenade->explosiveC = ExplosiveComponent_Create(x, y, angle, speed, destinationX, destinationY);

    return grenade;
}

void Grenade_Update(Entity* g, World* world)
{
    g->explosiveC->explosion_timer -= delta_g;

    if(g->explosiveC->explosion_timer <= 0)
    {
        g->alive = false;
    }
    g->movementC->dx = cos(g->movementC->angle) * g->movementC->speed * delta_g;
	g->movementC->dy = sin(g->movementC->angle) * g->movementC->speed  * delta_g;

    if(abs(g->x - g->explosiveC->destinationX) < 10)
    {
        g->movementC->dx = 0;
        g->movementC->dy = 0;
    }

    moveEntity(g, g->movementC->dx, g->movementC->dy);
}

#include <SDL.h>
#include <math.h>
#include "explosive.h"
#include "entity.h"
#include "movement_component.h"
#include "world.h"

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
    grenade->sub_category = Grenade_Normal;
	grenade->x = x;
	grenade->y = y;
	grenade->box.height = 15;
	grenade->box.width = 15;
	grenade->movementC = MovementC_Create();
	/*if(destinationX != 0 && destinationY != 0)
    {

        grenade->movementC->angle = C_AngleBetween2Points(x, y, destinationX, destinationY);
    }
    else
    {*/
       grenade->angle = angle;
    //}

	grenade->movementC->speed = speed;
    grenade->explosiveC = ExplosiveComponent_Create(x, y, angle, speed, destinationX, destinationY);
    grenade->movementC->dx = cos(grenade->angle ) * speed;
    grenade->movementC->dy = sin(grenade->angle ) * speed;
    moveEntity(grenade, grenade->movementC->dx * delta_g, grenade->movementC->dy * delta_g);

    return grenade;
}

void Grenade_Update(Entity* g, World* world)
{
    g->explosiveC->explosion_timer -= delta_g;

    if(g->explosiveC->explosion_timer <= 0)
    {
        Sound_PlayExplosion();
        g->alive = false;
    }

    for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(&world->monsters_vector, i);
        if(Entity_CheckNear(g, mob))
        {
            if(BoundingBox_CheckSimpleCollision(&g->box, &mob->box))
            {
                Sound_PlayExplosion();
                g->alive = false;
            }
        }
    }

    Box* temp = BoundingBox_CreateTemp(g);

    for(int i = 0 ; i < world->map_size ; i++)
    {
        if(world->map[i] != NULL && Entity_CheckVeryClose(g, world->map[i]))
        {
            if(BoundingBox_CheckSimpleCollision(temp, &world->map[i]->box))
            {
                g->movementC->dx = 0;
                g->movementC->dy = 0;
            }
        }
    }

    free(temp);

    float middleX, middleY;
    Entity_GetMiddleCoordinates(g, &middleX, &middleY);
    /*if((g->movementC->dx < 0 && middleX > g->explosiveC->destinationX) ||
       (g->movementC->dx > 0 && middleX < g->explosiveC->destinationX))
    {*/
        moveEntity(g, g->movementC->dx * delta_g, g->movementC->dy * delta_g);
    //}

}

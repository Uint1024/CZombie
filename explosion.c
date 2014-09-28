#include "explosion.h"
#include "world.h"
#include "wall.h"

Entity* Explosion_Create(int x, int y)
{
    Entity* exp = Entity_Spawn();

    exp->t = Cat_Explosion;
    exp->sub_category = Explosion_Normal;
    exp->alive = true;
    exp->damage =30;
    exp->x = x;
    exp->y = y;
    exp->alive_timer = 500;
//    exp->texture = Explosion1_tex;
    BoundingBox_Create(exp, 128, 128);

    return exp;
}


void Explosion_Update(Entity* exp, World* world)
{
    exp->alive_timer -= delta_g;


    for(int i = 0 ; i < Vector_Count(&world->non_null_walls) ; i++)
    {
        Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);

        if(BoundingBox_CheckSimpleCollision(&exp->box, &wall->box))
        {
            Structure_GetAttacked(wall, exp);
        }

    }

    for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(&world->monsters_vector, i);

        if(BoundingBox_CheckSimpleCollision(&exp->box, &mob->box))
        {
            Zombie_GetAttacked(mob, exp->damage, world);
        }

    }

    exp->damage = 0;

    if(exp->alive_timer <= 0)
        exp->alive = false;
}

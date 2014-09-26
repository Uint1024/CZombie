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


    for(int i = 0 ; i < world->map_size ; i++)
    {
        if(world->map[i] != NULL && BoundingBox_CheckSimpleCollision(&exp->box, &world->map[i]->box))
        {
            Structure_GetAttacked(world->map[i], exp);
        }

    }
    exp->damage = 0;

    if(exp->alive_timer <= 0)
        exp->alive = false;
}

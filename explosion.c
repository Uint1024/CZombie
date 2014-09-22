#include "explosion.h"

Entity* Explosion_Create(int x, int y)
{
    Entity* exp = Entity_Spawn();

    exp->alive = true;
    exp->damage = 50;
    exp->x = x;
    exp->y = y;
    exp->alive_timer = 500;
    exp->texture = Explosion1_tex;
    BoundingBox_Create(exp, 128, 128);

    return exp;
}


void Explosion_Update(Entity* exp)
{
    exp->alive_timer -= delta_g;
    if(exp->alive_timer <= 0)
        exp->alive = false;
}

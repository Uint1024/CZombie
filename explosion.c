#include "explosion.h"

Entity* Explosion_Create(int x, int y)
{
    Entity* exp = Entity_Spawn();

    exp->alive = Jtrue;
    exp->damage = 50;
    exp->x = x;
    exp->y = y;
    exp->explosion_timer = 500;
    exp->texture = Explosion1_tex;
    BoundingBox_Create(exp, 128, 128);

    return exp;
}


Entity* Explosion_Update(Entity* exp, int delta)
{
    exp->explosion_timer -= delta;
    if(exp->explosion_timer <= 0)
        exp->alive = Jfalse;
}

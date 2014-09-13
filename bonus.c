#include "bonus.h"

Entity* Bonus_Create(Type bonus_type, float x, float y)
{
    Entity* bonus = (Entity*)malloc(sizeof(Entity));
    if(bonus_type == Ammo)
    {
        bonus->x = x;
        bonus->y = y;
        bonus->t = Ammo;
        bonus->bullets[AutomaticRifle] = 50;
        bonus->alive = Jtrue;
        BoundingBox_Create(bonus, 20, 20);
    }

    return bonus;
}

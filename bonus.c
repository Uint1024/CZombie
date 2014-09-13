#include "bonus.h"

Entity* Bonus_Create(Type bonus_type, float x, float y)
{
    Entity* bonus = Entity_Spawn();
    //Entity* bonus = (Entity*)malloc(sizeof(Entity));
    if(bonus_type == Ammo)
    {
        bonus->x = x;
        bonus->y = y;
        bonus->t = Ammo;
        bonus->texture = Ammo_Bonus_tex;
        bonus->bullets[AutomaticRifle] = 50;
        BoundingBox_Create(bonus, 20, 20);
    }

    return bonus;
}

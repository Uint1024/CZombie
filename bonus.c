#include "bonus.h"
#include <stdio.h>

Entity* Bonus_Create(Type bonus_type, float x, float y)
{
    Entity* bonus = Entity_Spawn();
    if(bonus_type == Ammo)
    {
        bonus->x = x;
        bonus->y = y;
        bonus->t = Ammo;
        bonus->texture = Ammo_Bonus_tex;
        BoundingBox_Create(bonus, 10, 10);
    }

    return bonus;
}


void Bonus_Update(Entity* bonus, Entity* player)
{
    if(abs(bonus->x - player->x) < 150 && abs(bonus->y - player->y) < 150)
    {
        float dx = 0;
        float dy = 0;

        //velocity = 0.0053 * distance + 1
        //the velocity gets faster when the player is closer
        if(bonus->x > player->x)
        {
            dx = -(-0.0053f * (player->x - bonus->x) + 1);
        }
        else if(bonus->x < player->x)
        {
            dx = -0.0053f * (player->x - bonus->x) + 1;
        }

        if(bonus->y > player->y)
        {
            dy = -(-0.0053f * (player->y - bonus->y) + 1);
        }
        else if(bonus->y < player->y)
        {
            dy = -0.0053f * (player->y - bonus->y) + 1;
        }

        moveEntity(bonus, dx, dy);
    }
}

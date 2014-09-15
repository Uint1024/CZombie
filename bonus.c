#include "bonus.h"
#include <stdio.h>

void Bonus_GenerateRandom(Vector* bonus_vector, Entity* source)
{
    Bonus_type type = rand() % 4;

    Vector_Push(bonus_vector, Bonus_Create(type, source->x, source->y, source->angle));

}

Entity* Bonus_Create(Type bonus_type, float x, float y, float angle)
{
    Entity* bonus = Entity_Spawn();

    bonus->x = x;
    bonus->y = y;
    bonus->t = bonus_type;
    bonus->angle = angle;
    //bonus->pos = {x, y };

    if(bonus_type == Ammo_bonus)
    {
        bonus->texture = Ammo_Bonus_tex;

        BoundingBox_Create(bonus, 10, 10);
    }
    else if(bonus_type == Shotgun_bonus)
    {
        bonus->texture = Shotgun_Bonus_tex;
        bonus->corresponding_weapon = Shotgun_w;
        BoundingBox_Create(bonus, 30, 15);
    }
    else if(bonus_type == Rifle_bonus)
    {
        bonus->texture = Rifle_Bonus_tex;
        bonus->corresponding_weapon = AutomaticRifle_w;
        BoundingBox_Create(bonus, 30, 15);
    }
    else if(bonus_type == GrenadeLauncher_bonus)
    {
        bonus->texture = GrenadeLauncher_Bonus_tex;
        bonus->corresponding_weapon = GrenadeLauncher_w;
        BoundingBox_Create(bonus, 30, 15);
    }
    return bonus;
}


void Bonus_Update(Entity* bonus, Entity* player,int delta)
{
    if(abs(bonus->x - player->x) < 150 && abs(bonus->y - player->y) < 150)
    {
        bonus->angle = C_AngleBetween2Points(bonus->x, bonus->y, player->x, player->y);

        //min speed is 0 when distance is 150, max speed is 0.05f when distance is 0
        bonus->speed = -0.003f * abs(bonus->x - player->x) + 0.5;

        Entity_CalculateVelocityFromAngle(bonus, delta);
        moveEntity(bonus, bonus->dx, bonus->dy);
    }
}

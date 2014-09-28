#include "bonus.h"
#include <stdio.h>
#include "movement_component.h"
#include "entity.h"
#include "player.h"

void Bonus_GenerateRandom(Vector* bonus_vector, Entity* source)
{
    //Bonus_type type = rand() % NB_OF_BONUS_TYPES;
    //temporarily we replace random loot with just handgun ammo

    Vector_Push(bonus_vector, Bonus_Create(Bonus_Handgun, source->x, source->y, source->angle));

}

Entity* Bonus_Create(Bonus_type bonus_type, float x, float y, float angle)
{
    Entity* bonus = Entity_Spawn();

    bonus->x = x;
    bonus->y = y;
    bonus->t = Cat_Bonus;
    bonus->sub_category = bonus_type;
    bonus->movementC = MovementC_Create();
    bonus->angle = angle;

    if(bonus_type == Bonus_TheBigGun)
    {
        BoundingBox_Create(bonus, 60, 30);
    }
    else if(bonus_type == Bonus_Time_Stop)
    {
        BoundingBox_Create(bonus, 20, 20);
    }
    else
    {
        BoundingBox_Create(bonus, 30, 15);
    }


    return bonus;
}


void Bonus_Update(Entity* bonus, Entity* player)
{
    if(Entity_CheckDistance(player, bonus, 150) &&
       !(bonus->sub_category == Bonus_Time_Stop && player->playerC->time_stop == player->playerC->max_time_stop))
    {
        bonus->angle = C_AngleBetween2Points(bonus->x, bonus->y, player->x, player->y);

        //min speed is 0 when distance is 150, max speed is 0.05f when distance is 0
        bonus->movementC->speed = -0.003f * abs(bonus->x - player->x) + 0.5;

        Entity_CalculateVelocity(bonus);
        moveEntity(bonus, bonus->movementC->dx , bonus->movementC->dy);
    }
}

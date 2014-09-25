#ifndef DECALS_H
#define DECALS_H
#include "common.h"
#include "entity.h"
#include "movement_component.h"

Entity* Decal_Create(Entity* parent, Decal_Type type)
{
    Entity* decals = Entity_Spawn();
    decals->solid = false;
    decals->t = Cat_Decal;
    decals->sub_category = type;
    //alright so angle shouldn't be in movementC... FUCK
    decals->movementC = MovementC_Create();
    decals->movementC->angle = parent->movementC->angle;

    int width = 40;
    int height = 40;
    switch(type)
    {
    case Decal_Corpse_Normal:
        break;
    case Decal_Corpse_Heavy:
        break;
    case Decal_Corpse_Trooper:
        width = 60;
        height = 45;
        break;
    case Decal_Corpse_Fast:
        break;
    case Decal_Corpse_Huge:
        width = 150;
        height = 150;
        break;
    }


    decals->box = BoundingBox_CreateBetter(parent->x, parent->y, width, height);
    decals->x = parent->x;
    decals->y = parent->y;

    return decals;

}
#endif // DECALS_H

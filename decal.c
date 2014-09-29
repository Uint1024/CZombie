#include "decals.h"
#include "entity.h"

Entity* Decal_Create(Decal_Type type, float x, float y, float angle)
{
    Entity* decals = Entity_Spawn();
    decals->solid = false;
    decals->t = Cat_Decal;
    decals->sub_category = type;
    //alright so angle shouldn't be in movementC... FUCK
    decals->movementC = MovementC_Create();
    decals->angle = angle;

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
    case Decal_Corpse_Raptor:
        width = 120;
        height = 120;
        break;
    case Decal_Corpse_Destroyer:
        width = 260;
        height = 260;
        break;
    case Decal_Rug_Ancient:
        width = 20;
        height = 60;
        break;
    }

    if(type != Decal_Rug_Ancient)
    {
        decals->is_ennemy = true;
    }


    decals->box = BoundingBox_CreateBetter(x, y, width, height);
    decals->x = x;
    decals->y = y;

    return decals;

}

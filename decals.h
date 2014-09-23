#ifndef DECALS_H
#define DECALS_H
#include "common.h"
#include "entity.h"
#include "movement_component.h"

Entity* Decal_Create(Entity* parent, Decal_Type type, int sub_type)
{
    Entity* decals = Entity_Spawn();
    decals->solid = false;

    //alright so angle shouldn't be in movementC... FUCK
    decals->movementC = MovementC_Create();
    decals->movementC->angle = parent->movementC->angle;

    int width = 40;
    int height = 40;
    if(type == Corpse)
    {
        switch(sub_type)
        {
        case Normal_Zombie:
            decals->texture = Tex_Decals_Corpse_Zombie_Normal;
            break;
        case Heavy_Zombie:
            decals->texture = Tex_Decals_Corpse_Zombie_Heavy;
            break;
        case Trooper_Zombie:
            decals->texture = Tex_Decals_Corpse_Zombie_Trooper;
            width = 60;
            height = 45;
            break;
        case Fast_Zombie:
            decals->texture = Tex_Decals_Corpse_Zombie_Fast;
            break;
        case Huge_Zombie:
            decals->texture = Tex_Decals_Corpse_Zombie_Huge;
            width = 150;
            height = 150;
            break;
        }
    }

    decals->box = BoundingBox_CreateFromAllValues(width, height,
                                                parent->x, parent->y);
    decals->x = parent->x;
    decals->y = parent->y;

    return decals;

}
#endif // DECALS_H

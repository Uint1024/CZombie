#include "prop.h"
#include "entity.h"

Entity* Prop_Create(Bonus_type type, float x, float y, float angle)
{
    Entity* prop = Entity_Spawn();

    prop->x = x;
    prop->y = y;
    prop->t = Cat_Prop;
    prop->sub_category = type;
    prop->angle = angle;

    if(type == Prop_Bed)
    {
        BoundingBox_Create(prop, 32, 64);
    }
    else if(type == Prop_Desk)
    {
        BoundingBox_Create(prop, 25, 40);
    }

    return prop;
}

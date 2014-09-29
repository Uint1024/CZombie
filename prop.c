#include "prop.h"
#include "entity.h"
#include "boundingBox.h"

Entity* Prop_Create(Bonus_type type, float x, float y, float angle)
{
    Entity* prop = Entity_Spawn();
    printf("%f %f\n", x, y);
    prop->x = x;
    prop->y = y;
    prop->t = Cat_Prop;
    prop->sub_category = type;
    prop->angle = angle;
    prop->hp = 10;

    //bbox value, change depending on the angle
    int true_x, true_y, true_width, true_height;

    switch(type)
    {
    case Prop_Bed:
        prop->width = 32;
        prop->height = 64;
        break;
    case Prop_Desk:
        prop->width = 25;
        prop->height = 40;
        break;
    case Prop_Chair:
        prop->width = 20;
        prop->height = 20;
        break;
    case Prop_Car_Cool:
        prop->width = 60;
        prop->height = 120;
        prop->hp = 25;
        break;
    case Prop_Bookcase:
        prop->width = 32;
        prop->height = 32;
        break;
    case Prop_FileCabinet:
        prop->width = 32;
        prop->height = 32;
        break;
    case Prop_BlackChest:
        prop->width = 32;
        prop->height = 32;
        break;
    }

    true_width =  prop->width ;
    true_height = prop->height;
    true_x = x;
    true_y = y;
    int xCenter = x + prop->width / 2;
    int yCenter = y + prop->height / 2;

    if(angle > HALF_PI - 0.1 && angle < HALF_PI + 0.1)
    {
        true_x = xCenter - (cos(angle) * (x - xCenter)) + (sin(angle) * (y - yCenter));
        true_y = yCenter + sin(angle) * (x - xCenter) + cos(angle) * (y - yCenter);

        true_width = prop->height;
        true_height = prop->width;

    }
    else if(angle > PI + HALF_PI - 0.1 && angle < PI + HALF_PI + 0.1)
    {
        true_x = xCenter + cos(angle) * (x - xCenter) - sin(angle) * (y - yCenter);
        true_y = yCenter - sin(angle) * (x - xCenter) + cos(angle) * (y - yCenter);
        true_width = prop->height;
        true_height = prop->width;
    }

    prop->box = BoundingBox_CreateBetter(true_x, true_y, true_width, true_height);




    return prop;
}

#include "mapEvent.h"
#include "entity.h"

Entity* MapEvent_Create(MapEvent_Type type, int x, int y)
{
    Entity* me = Entity_Spawn();
    me->t = Cat_Event;
    me->sub_category = type;
    me->x = x;
    me->y = y;


    me->box = BoundingBox_CreateBetter(x, y, TILE_SIZE, TILE_SIZE);
//    me->texture = all_textures_g[Cat_Event][type];
    return me;
}

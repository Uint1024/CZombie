#ifndef MAPEVENT_H
#define MAPEVENT_H

/*typedef struct MapEvent
{
    int x;
    int y;
    MapEvent_Type type;
    Box box;

} apEvent;*/

Entity* MapEvent_Create(MapEvent_Type type, int x, int y, int width, int height)
{
    Entity* me = Entity_Spawn();
    me->t = Cat_Event;
    me->sub_category = type;
    me->x = x;
    me->y = y;
    me->box = BoundingBox_CreateBetter(x, y, width, height);
    me->texture = event_textures_g[type];
    return me;
}
#endif // MAPEVENT_H

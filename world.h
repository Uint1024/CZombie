#ifndef WORLD_H
#define WORLD_H
#include "vector.h"
#include "entity.h"

typedef struct World
{
    Vector      bullets_vector;
    Vector      bonus_vector;
    Vector      monsters_vector;
    Vector      explosions_vector;
    Entity      player;

    //"foreground" layer of the map
    Entity**     map;
    int         map_width;
    int         map_height;
    int         map_size;
    Entity**      ground_map;

} World;

World World_Initialize(int map_width, int map_height, int screen_width, int screen_height);
#endif // WORLD_H

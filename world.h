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
    Vector      events_vector;
    Entity      player;


    //"foreground" layer of the map, solid
    Entity**     map;
    int         map_width;
    int         map_height;
    int         map_size;

    //background
    Entity**      ground_map;

} World;

//load empty map at the beginning of the program
World World_Initialize(int map_width, int map_height);

//delete the actual loaded map and replace it with a new empty map
void World_Reset(World* world, int map_width, int map_height);
#endif // WORLD_H

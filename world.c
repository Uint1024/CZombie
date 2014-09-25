#include "world.h"
#include "entity.h"
#include "player.h"
#include "wall.h"

World World_Initialize(int map_width, int map_height)
{
    World world;

    world.bullets_vector    =   Vector_Create();
    world.bonus_vector      =   Vector_Create();
    world.monsters_vector   =   Vector_Create();
    world.explosions_vector =   Vector_Create();
    world.events_vector     =   Vector_Create();
    world.decals_vector     =   Vector_Create();
    world.non_null_walls = Vector_Create();
    world.map_width         =   map_width;
    world.map_height        =   map_height;
    world.map_size          =   map_width * map_height;
    world.map               =   calloc(world.map_size, sizeof(Entity*));
    world.ground_map        =   calloc(world.map_size, sizeof(Entity*));
    world.player            =   Player_Create(100,
                                              100,
                                              10, 10);


    //calloc does that
    /*for(int i = 0 ; i < world.map_size ; i++)
    {
        world.map[i] = NULL;//Wall_CreateEmpty();

    }*/

    for(int i = 0 ; i < world.map_size ; i++)
    {
        world.ground_map[i] = Ground_Create(Ground_Grass,
                                            (i % map_width) * TILE_SIZE,
                                            TILE_SIZE*(i/map_width));

    }

    return world;
}


void World_Reset(World* world, int map_width, int map_height)
{
    Vector_Clear(&world->bonus_vector);
    Vector_Clear(&world->bullets_vector);
    Vector_Clear(&world->monsters_vector);
    Vector_Clear(&world->explosions_vector);
    Vector_Clear(&world->events_vector);

    for(int i = 0 ; i < world->map_size ; i++)
    {
        free(world->map[i]);

    }

    for(int i = 0 ; i < world->map_size ; i++)
    {
        free(world->ground_map[i]);
    }


    *world = World_Initialize(map_width, map_height);
}

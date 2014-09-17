#include "world.h"
#include "entity.h"
#include "player.h"
#include "wall.h"

World World_Initialize(int map_width, int map_height,
                       int screen_width, int screen_height)
{
    World world;

    world.bullets_vector    =   Vector_Create();
    world.bonus_vector      =   Vector_Create();
    world.monsters_vector   =   Vector_Create();
    world.explosions_vector =   Vector_Create();
    world.map_width         =   map_width;
    world.map_height        =   map_height;
    world.map_size          =   map_width * map_height;
    world.map               =   calloc(world.map_size, sizeof(Entity*));
    world.ground_map        =   calloc(world.map_size, sizeof(Entity*));
    world.player            =   Player_Create(screen_width / 2 - 10,
                                              screen_height / 2 - 10,
                                              20, 20);

    for(int i = 0 ; i < world.map_size ; i++)
    {
        //world.map[i].t = None;

    }

    for(int i = 0 ; i < world.map_size ; i++)
    {
        world.ground_map[i] = Ground_Create(Ground_Grass,
                                            (i % map_width) * TILE_SIZE,
                                            TILE_SIZE*(i/map_width));

    }

    return world;
}

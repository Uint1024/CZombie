#include <math.h>
#include <stdlib.h>
#include "wall.h"
#include "world.h"
#include "entity.h"


Entity* Wall_CreateEmpty()
{
    Entity* ent = Entity_Spawn();
        ent->visible = false;
        ent->solid = false;

        return ent;
}

void Wall_Update_Tile_Type(Entity* wall, World* world)
{
    Entity** map = world->map;

    int map_position = (wall->y/TILE_SIZE) * world->map_width + wall->x/TILE_SIZE;

    Entity* map_left = map[map_position - 1];
    Entity* map_left_left = map[map_position - 2];
    Entity* map_left_up = map[map_position - 1 - world->map_width];
    Entity* map_left_down = map[map_position - 1 + world->map_width];


    Entity* map_right = map[map_position + 1];
    Entity* map_right_up = map[map_position + 1  - world->map_width];
    Entity* map_right_down = map[map_position + 1 + world->map_width];

    Entity* map_up = map[map_position - world->map_width];
    Entity* map_up_up = map[map_position - 2 * world->map_width];
    Entity* map_down = map[map_position + world->map_width];
    Entity* map_down_down = map[map_position + 2 * world->map_width];


    if(map_left == NULL &&
       map_right  == NULL &&
       map_up == NULL &&
       map_down == NULL)
    {
        wall->tile_type = Column;
    }

    if(map_left != NULL)
    {
        if(map_up == NULL)
        {
            if(map_right == NULL)
            {
                if(map_down == NULL)
                {
                    wall->tile_type = Ending;
                    wall->angle = 0;
                }
                else
                {
                    wall->tile_type = Corner;
                    wall->angle = 0;
                }
            }
            else
            {
                if(map_down == NULL)
                {
                    wall->tile_type = Two_Border;
                    wall->angle = PI;
                }
                else
                {
                    if(map_left_down == NULL &&
                       map_right_down == NULL)
                    {
                        wall->tile_type = One_Border_2_Dots;
                        wall->angle = 0;
                    }
                    else if(map_left_down != NULL &&
                       map_right_down == NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Right;
                        wall->angle = 0;
                    }
                    else if(map_left_down == NULL &&
                       map_right_down != NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Left;
                        wall->angle = 0;
                    }
                    else if(map_left_down != NULL &&
                       map_right_down != NULL)
                    {
                        wall->tile_type = One_Border;
                        wall->angle = 0;
                    }


                }
            }
        }
        else if(map_up != NULL)
        {
            if(map_right == NULL)
            {
                if(map_down == NULL)
                {
                    if(map_left_up == NULL &&
                       map_left_down == NULL)
                    {
                        wall->tile_type = Corner_Dot;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up != NULL &&
                       map_left_down == NULL)
                    {
                        wall->tile_type = Corner;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up == NULL &&
                       map_left_down != NULL)
                    {
                        wall->tile_type = Corner_Dot;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up != NULL &&
                       map_left_down != NULL)
                    {
                        wall->tile_type = Corner;
                        wall->angle = HALF_PI;
                    }
                }
                else
                {
                    if(map_left_up == NULL &&
                       map_left_down == NULL)
                    {
                        wall->tile_type = One_Border_2_Dots;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up != NULL &&
                       map_left_down == NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Right;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up == NULL &&
                       map_left_down != NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Left;
                        wall->angle = HALF_PI;
                    }
                    else if(map_left_up != NULL &&
                       map_left_down != NULL)
                    {
                        wall->tile_type = One_Border;
                        wall->angle = HALF_PI;
                    }
                }
            }
            else if(map_right != NULL)
            {
                if(map_down == NULL)
                {
                    if(map_left_up == NULL &&
                       map_right_up == NULL)
                    {
                        wall->tile_type = One_Border_2_Dots;
                        wall->angle = PI;
                    }
                    else if(map_left_up != NULL &&
                       map_right_up == NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Left;
                        wall->angle = PI;
                    }
                    else if(map_left_up == NULL &&
                       map_right_up != NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Right;
                        wall->angle = PI;
                    }
                    else if(map_left_up != NULL &&
                       map_right_up != NULL)
                    {
                        wall->tile_type = One_Border;
                        wall->angle = PI;
                    }

                }
                else if(map_down != NULL)
                {
                    if(map_left_up == NULL)
                    {
                        if(map_right_up == NULL)
                        {
                            if(map_left_down == NULL)
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Four_Dots;
                                    wall->angle = 0;
                                }
                                else
                                {
                                    wall->tile_type = Three_Dots;
                                    wall->angle = PI;
                                }
                            }
                            else
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Three_Dots;
                                    wall->angle = 3 * HALF_PI;
                                }
                                else
                                {
                                    wall->tile_type = Two_Dots;
                                    wall->angle = PI;
                                }
                            }
                        }
                        else
                        {
                            if(map_left_down == NULL)
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Three_Dots;
                                    wall->angle = HALF_PI;
                                }
                                else
                                {
                                    wall->tile_type = Two_Dots;
                                    wall->angle = HALF_PI;
                                }
                            }
                            else
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Two_Opposite_Dots;
                                    wall->angle = PI;
                                }
                                else
                                {
                                    wall->tile_type = One_Dot;
                                    wall->angle = 0;
                                }
                            }
                        }
                    }
                    else if(map_left_up != NULL)
                    {
                        if(map_right_up == NULL)
                        {
                            if(map_left_down == NULL)
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Three_Dots;
                                    wall->angle = 0;
                                }
                                else
                                {
                                    wall->tile_type = Two_Opposite_Dots;
                                    wall->angle = HALF_PI;
                                }
                            }
                            else
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Two_Dots;
                                    wall->angle = 3 * HALF_PI;
                                }
                                else
                                {
                                    wall->tile_type = One_Dot;
                                    wall->angle = HALF_PI;
                                }
                            }
                        }
                        else if (map_right_up != NULL)
                        {
                            if(map_left_down == NULL)
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = Two_Dots;
                                    wall->angle = 0;
                                }
                                else
                                {
                                    wall->tile_type = One_Dot;
                                    wall->angle = 3 * HALF_PI;
                                }
                            }
                            else
                            {
                                if(map_right_down == NULL)
                                {
                                    wall->tile_type = One_Dot;
                                    wall->angle = PI;
                                }
                                else
                                {
                                    wall->tile_type = No_Border;
                                    wall->angle = 0;
                                }
                            }
                        }
                    }

                }
            }
        }
    }
    else if (map_left == NULL)
    {
        if(map_up == NULL)
        {
            if(map_right == NULL)
            {
                if(map_down == NULL)
                {
                    wall->tile_type = Column;
                    wall->angle = 0;
                }
                else
                {
                    wall->tile_type = Ending;
                    wall->angle = 3 * HALF_PI;
                }
            }
            else
            {
                if(map_down == NULL)
                {
                    wall->tile_type = Ending;
                    wall->angle = PI;
                }
                else
                {
                    wall->tile_type = Corner;
                    wall->angle = 3* HALF_PI;
                }
            }
        }
        else
        {
            if(map_right == NULL)
            {
                if(map_down == NULL)
                {
                    wall->tile_type = Ending;
                    wall->angle = HALF_PI;
                }
                else
                {
                    wall->tile_type = Two_Border;
                    wall->angle = HALF_PI;
                }
            }
            else
            {
                if(map_down == NULL)
                {
                    if(map_right_up != NULL)
                    {
                        wall->tile_type = Corner;
                        wall->angle = PI;
                    }
                    else
                    {
                        wall->tile_type = Corner_Dot;
                        wall->angle = PI;
                    }

                }
                else
                {
                    if(map_right_up == NULL &&
                       map_right_down == NULL)
                    {
                        wall->tile_type = One_Border_2_Dots;
                        wall->angle = 3* HALF_PI;
                    }
                    else if(map_right_up != NULL &&
                       map_right_down == NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Left;
                        wall->angle = 3* HALF_PI;
                    }
                    else if(map_right_up == NULL &&
                       map_right_down != NULL)
                    {
                        wall->tile_type = One_Border_1_Dot_Right;
                        wall->angle = 3* HALF_PI;
                    }
                    else if(map_right_up != NULL &&
                       map_right_down != NULL)
                    {
                        wall->tile_type = One_Border;
                        wall->angle = 3* HALF_PI;
                    }

                }
            }
        }
    }

}

Entity* Wall_Create(Wall_Type type, int x_, int y_, World* world)
{
	Entity* wall = Entity_Spawn();
	wall->t = Cat_Wall;
	wall->sub_category = type;
	wall->x = x_;
	wall->y = y_;

	wall->hp = 25;


	BoundingBox_Create(wall, TILE_SIZE, TILE_SIZE);
    switch(type)
    {
    case Wall_Glass:
        wall->transparent = true;
        wall->hp = 10;
        break;
    case Wall_Reinforced:
        wall->hp = 100000;
        break;
    }

    Entity** map = world->map;

    int map_position = (wall->y/TILE_SIZE) * world->map_width + wall->x/TILE_SIZE;


    Entity* map_left = map[map_position - 1];
    Entity* map_left_left = map[map_position - 2];
    Entity* map_left_up = map[map_position - 1 - world->map_width];
    Entity* map_left_down = map[map_position - 1 + world->map_width];


    Entity* map_right = map[map_position + 1];
    Entity* map_right_up = map[map_position + 1  - world->map_width];
    Entity* map_right_down = map[map_position + 1 + world->map_width];

    Entity* map_up = map[map_position - world->map_width];
    Entity* map_up_up = map[map_position - 2 * world->map_width];
    Entity* map_down = map[map_position + world->map_width];
    Entity* map_down_down = map[map_position + 2 * world->map_width];


    Wall_Update_Tile_Type(wall, world);

    if(map_left != NULL)
    Wall_Update_Tile_Type(map_left, world);
    if(map_left_up != NULL)
    Wall_Update_Tile_Type(map_left_up, world);
    if(map_left_down != NULL)
    Wall_Update_Tile_Type(map_left_down, world);
    if(map_right != NULL)
    Wall_Update_Tile_Type(map_right, world);
    if(map_right_up != NULL)
    Wall_Update_Tile_Type(map_right_up, world);
    if(map_right_down != NULL)
    Wall_Update_Tile_Type(map_right_down, world);
    if(map_up != NULL)
    Wall_Update_Tile_Type(map_up, world);
    if(map_down != NULL)
    Wall_Update_Tile_Type(map_down, world);


	return wall;
}

bool Wall_IsWall(Entity* w)
{
    if(w!= NULL)
    {
        return (w->solid && w->visible && !w->transparent);
    }
    else
    {
        return false;
    }
}

Entity* Ground_Create(Ground_Type type, float x, float y)
{
    Entity* ent = Entity_Spawn();
    ent->t = Cat_Ground;
    ent->sub_category = type;
	ent->x = x;
	ent->y = y;

	BoundingBox_Create(ent, TILE_SIZE, TILE_SIZE);

	return ent;
}


void Structure_GetAttacked(Entity* d, Entity* attacker)
{
    if((d->hp -= attacker->damage) <= 0)
    {
        Structure_Die(d);
    }
}

void Structure_Die(Entity* d)
{
    Sound_PlayWallDestroyed();
    d->alive = false;

}


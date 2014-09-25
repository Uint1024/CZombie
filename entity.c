#include "common.h"
#include "entity.h"
#include "math.h"
#include "boundingBox.h"
#include <math.h>
#include <stdlib.h>
#include "camera.h"
#include "boundingBox.h"
#include "player.h"
#include "world.h"
#include <math.h>
#include "zombie.h"
#include "door.h"
#include "wall.h"
#include "movement_component.h"
#include "mapEvent.h"
#include "bonus.h"


Entity* Entity_Spawn()
{
    Entity* ent = (Entity*)malloc(sizeof(Entity));


    ent->t                          = Nothing;
    ent->sub_category               = 0;
	ent->x                          = 0;
	ent->y                          = 0;
	ent-> box.top                   = 0;
	ent->box.left                   = 0;
	ent->box.width                  = 0;
	ent->box.height                 = 0;
	ent->box.right                  = 0;
	ent->box.bottom                 = 0;
    ent->solid                      = true;
    ent->visible                    = true;
    ent->penetration_chance = 0;
    ent->in_dark = true;
	ent->alive                      = true;
ent->nb_penetrations = 0;
ent->transparent = false;
    ent->damage                     = 0;

	//components
    ent->weaponsC                   = NULL;
    ent->explosiveC                 = NULL;
    ent->zombieC                    = NULL;
    ent->movementC                  = NULL;
    ent->playerC                    = NULL;


	return ent;
}

void Entity_Destroy(Entity* ent)
{
    if(ent->weaponsC != NULL)
    {
        for(int i = 0 ; i < NB_WEAPON_TYPES ; i++)
        {
            free(ent->weaponsC->weapons_inventory[i]);
        }
    }

    free(ent->weaponsC);
    free(ent->explosiveC);
    free(ent->zombieC);
    free(ent->movementC);
    free(ent->playerC);
}

Entity* Entity_Create(Main_Category cat, int type, float x, float y, float angle)
{
    Entity* entity;

    switch(cat)
    {
    case Cat_Wall:
        entity = Wall_Create(type, x, y);
        break;
    case Cat_Door:
        entity = Door_Create(type, x, y);
        break;
    case Cat_Ground:
        entity = Ground_Create(type, x, y);
        break;
    case Cat_Zombie:
        entity = CreateZombie(type, x, y);
        break;
    case Cat_Event:
        entity = MapEvent_Create(type, x, y);
        break;
    case Cat_Bonus:
        entity = Bonus_Create(type, x, y, angle);
        break;
    }

    return entity;
}


bool Entity_CheckNear(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 600 && abs(ent1->y - ent2->y) < 600);
}

bool Entity_CheckVeryClose(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 100 && abs(ent1->y - ent2->y) < 100);
}

void Entity_CalculateVelocity(Entity* ent)
{
    ent->movementC->dx = cos(ent->movementC->angle ) * ent->movementC->speed * delta_g;
    ent->movementC->dy = sin(ent->movementC->angle ) * ent->movementC->speed * delta_g;
}


void Entity_GetMiddleCoordinates(Entity* ent, float* middleX, float* middleY)
{
    *middleX = (float)(ent->x + ((ent->box.width + (ent->box.offsetX*2)) / 2));
    *middleY = (float)(ent->y + ((ent->box.height + (ent->box.offsetY*2)) / 2));
}

float Entity_GetMiddleY(Entity* ent)
{
    return (float)(ent->y + ent->box.height / 2);
}

float Entity_GetMiddleX(Entity* ent)
{
    return (float)(ent->x + ent->box.width / 2);
}


void moveEntity(Entity* ent, float x, float y)
{
	ent->x += x;
	ent->y += y;

	BoundingBox_Update(ent);


}

void Entity_MoveToPosition(Entity* ent, float x, float y)
{
	ent->x = x;
	ent->y = y;


	BoundingBox_Update(ent);
}

float Entity_DistanceBetweenTwoEntities(Entity* ent1, Entity* ent2)
{
    float ent1_middleX = 0;
    float ent1_middleY = 0;
    float ent2_middleX = 0;
    float ent2_middleY = 0;

    Entity_GetMiddleCoordinates(ent1, &ent1_middleX, &ent1_middleY);
    Entity_GetMiddleCoordinates(ent2, &ent2_middleX, &ent2_middleY);

    float distanceX =  ent2_middleX - ent1_middleX;
    float distanceY = ent2_middleY - ent1_middleY;
    float pythDistance = sqrt(distanceX * distanceX +
                                distanceY * distanceY);

    return pythDistance;

}

bool Entity_CheckCanSeeEntity(Entity* ent1, Entity* ent2, World* world)
{
    //middle of ent1, does not change
    float ent1MiddleX = 0;
    float ent1MiddleY = 0;
    Entity_GetMiddleCoordinates(ent1, &ent1MiddleX, &ent1MiddleY);

    //the next position of the end of the line
    //will change until we reach a wall or the target
    float pointX = ent1MiddleX;
    float pointY = ent1MiddleY;

    //distance calculated from ent1 to the end of the line at each loop
    float line_distance = 0;

    //will not change
    float distance_to_ent2 = Entity_DistanceBetweenTwoEntities(ent1, ent2);

    //become false if we hit a wall
    bool can_see = true;

    float angle_to_ent2 = C_AngleBetween2Entities(ent1, ent2);

    while(line_distance <= distance_to_ent2 &&
          can_see == true)
    {
        pointX += cos(angle_to_ent2) * 28;//magic number yay!!
        pointY += sin(angle_to_ent2) * 28;//max of cos(x) and sin(x) is 1 so it
        line_distance = C_DistanceBetween2Points(ent1MiddleX, ent1MiddleY,
                                                 pointX, pointY);

        //looping every walls close to the zombie
        for(int i = 0 ; i < world->map_size ; i++)
        {
            if(world->map[i] != NULL && Entity_CheckNear(ent1, world->map[i]) &&
                Wall_IsWall(world->map[i]))
            {
                //if the end of the line hits a wall, the zombie can't see the player
                if(BoundingBox_CheckPointCollision(pointX, pointY, &world->map[i]->box))
                {
                    can_see = false;
                }
            }
        }
    }

    return can_see;
}

bool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance)
{

    float pythDistance = Entity_DistanceBetweenTwoEntities(ent1, ent2);

    if(pythDistance < distance)
        return true;
    else
        return false;
}

bool Entity_CollisionWithStuff(Entity* ent, World* world)
{

    bool collision_with_walls = Entity_CollisionWithWalls(ent, world);
    bool collision_with_mobs = Entity_CollisionWithMonsters(ent, &world->monsters_vector);

    return (collision_with_walls || collision_with_mobs);
}

void Entity_CalculateVisibility(Entity* ent, World* world)
{
    bool in_dark = true;
    for (int i = 0; i < world->map_size && in_dark; i++)
	{
		if (!world->ground_map[i]->in_dark)
		{
		    if(BoundingBox_CheckSimpleCollision(&world->ground_map[i]->box, &ent->box))
            {
                in_dark = false;
            }
		}
	}

	ent->in_dark = in_dark;
}

bool Entity_CollisionWithWalls(Entity* ent, World* world)
{
    Box* temp = BoundingBox_CreateTemp(ent);
    Entity* collision_wall[5];
	int walls_touched[5] = { 0 };

    bool collision = false;
    for (int i = 0; i < Vector_Count(&world->non_null_walls); i++)
	{
	    Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);
		if (wall->solid)
		{
		    if(Entity_CheckVeryClose(ent, wall))
            {
                Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &wall->box);
                if (collision_direction != None)
                {
                    collision = true;
                    collision_wall[collision_direction] = wall;
                    walls_touched[collision_direction]++;
                    ent->collision_direction = collision_direction;

                    if(ent->t == Cat_Zombie && ent->zombieC->aggressive &&
                      wall->solid &&
                       (wall->t == Cat_Door || wall->t == Cat_Wall) &&
                       ent->zombieC->attack_timer >= ent->zombieC->attack_delay)
                    {
                        Structure_GetAttacked(wall, ent);
                        ent->zombieC->attack_timer = 0;
                    }

                    if(ent->t == Cat_Player && wall->t == Cat_Door)
                    {
                        Door_Open(wall);
                    }
                }
            }
		}
	}


    if(ent->movementC->dy == 0)
    {
        if(walls_touched[Right] == 1 &&
           ent->movementC->dx > 0 &&
           ent->box.right <= collision_wall[Right]->x)
        {
            if(ent->box.top < collision_wall[Right]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_wall[Right]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
        else if(walls_touched[Left] == 1 &&
                ent->movementC->dx < 0 &&
                ent->box.left >= collision_wall[Left]->x)
        {
            if(ent->box.top < collision_wall[Left]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_wall[Left]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
    }

    if(ent->movementC->dx == 0)
    {
        if(walls_touched[Top] == 1 && ent->movementC->dy < 0
                && ent->box.top >= collision_wall[Top]->box.bottom)
        {
            if(ent->box.left < collision_wall[Top]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_wall[Top]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
        else if(walls_touched[Bottom] == 1 && ent->movementC->dy > 0
                && ent->box.bottom <= collision_wall[Bottom]->box.top)
        {
            if(ent->box.left < collision_wall[Bottom]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_wall[Bottom]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
    }

        bool flatBottomTop = false;


        if (walls_touched[Bottom] || walls_touched[Top])
            flatBottomTop = true;

        if (walls_touched[Bottom] > 0 && walls_touched[Left] > 1)
        {
            ent->movementC->dx = collision_wall[Left]->box.right - ent->box.left;
            ent->movementC->dy = 0;
        }
        if (walls_touched[Bottom] > 0 && walls_touched[Right] > 1)
        {
            ent->movementC->dx = collision_wall[Right]->box.left - ent->box.right;
            ent->movementC->dy = 0;
        }
        if (walls_touched[Top] > 0 && walls_touched[Left] > 1)
        {
            ent->movementC->dx = collision_wall[Left]->box.right - ent->box.left;
            ent->movementC->dy = 0;
        }
        if (walls_touched[Top] > 0 && walls_touched[Right] > 1)
        {
            ent->movementC->dx = collision_wall[Right]->box.left - ent->box.right;
            ent->movementC->dy = 0;
        }

        if (walls_touched[Bottom] && ent->movementC->dy > 0)
        {
            ent->movementC->dy = collision_wall[Bottom]->box.top - ent->box.bottom;
        }

        if (walls_touched[Top] && ent->movementC->dy < 0)
        {

            ent->movementC->dy = collision_wall[Top]->box.bottom - ent->box.top;
        }




        if (!flatBottomTop)
        {
            if (walls_touched[Right] && ent->movementC->dx > 0)
            {
                ent->movementC->dx = collision_wall[Right]->box.left - ent->box.right;
            }

            if (walls_touched[Left] && ent->movementC->dx < 0)
            {
                ent->movementC->dx = collision_wall[Left]->box.right - ent->box.left;
            }
        }


	free(temp);

	return collision;
}



bool Entity_CollisionWithMonsters(Entity* ent, Vector* monsters_vector)
{
    bool collision = false;
    Box* temp = BoundingBox_CreateTemp(ent);

    if(ent->t != Cat_Player ||
       (ent->t == Cat_Player && ent->playerC->invulnerability_timer <= 0))
    {
        Entity* collision_sides[5] = {false};
        for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
        {
            if(Vector_Get(monsters_vector, i) != ent)
            {
                Entity* mob_to_check = (struct Entity*)Vector_Get(monsters_vector, i);
                if(Entity_CheckNear(ent, mob_to_check))
                {
                    Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &mob_to_check->box);
                    if (collision_direction != None)
                    {
                        collision = true;
                        collision_sides[collision_direction] = mob_to_check;
                        ent->collision_direction = collision_direction;
                    }
                }
            }
        }

        if ((collision_sides[Bottom] && ent->movementC->dy > 0) ||
            (collision_sides[Top] && ent->movementC->dy < 0))
        {
            ent->movementC->dy = 0;
        }

        if ((collision_sides[Right] && ent->movementC->dx > 0) ||
            (collision_sides[Left] && ent->movementC->dx < 0))
        {
            ent->movementC->dx = 0;
        }

        if(collision && ent->t == Cat_Player)
        {

            Player_TakeDamage(ent, collision_sides);

        }
    }

    free(temp);

    return collision;
}

void Entity_CollisionWithExplosions(Entity* ent, Vector* explosions)
{
    bool collision = false;
    for(int i = 0 ; i < Vector_Count(explosions) ; i++)
    {
        Entity* exp = (Entity*)Vector_Get(explosions, i);
        collision = BoundingBox_CheckSimpleCollision(&ent->box, &exp->box);
        if(collision)
        {
            Entity_LoseHealth(ent, exp->damage);
        }
    }
}

void Entity_LoseHealth(Entity* ent, int damage)
{
    ent->hp -= damage;

    if (ent->hp <= 0)
        ent->alive = false;
}

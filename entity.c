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
#include "prop.h"
#include "decals.h"



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
    ent->angle = 0;
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
    case Cat_Decal:
        entity = Decal_Create(type, x, y, angle);
        break;
    case Cat_Prop:
        entity = Prop_Create(type, x, y, angle);
        break;
    }

    if(angle != 0)
    {
        entity->angle = angle;
    }

    return entity;
}


bool Entity_CheckNear(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 800 && abs(ent1->y - ent2->y) < 600);
}

bool Entity_CheckVeryClose(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 300 && abs(ent1->y - ent2->y) < 300);
}

void Entity_CalculateVelocity(Entity* ent)
{
    ent->movementC->dx = cos(ent->angle ) * ent->movementC->speed * delta_g;
    ent->movementC->dy = sin(ent->angle ) * ent->movementC->speed * delta_g;
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
    float ent1MiddleX = 0;
    float ent1MiddleY = 0;
    Entity_GetMiddleCoordinates(ent1, &ent1MiddleX, &ent1MiddleY);

    float ent2MiddleX = 0;
    float ent2MiddleY = 0;
    Entity_GetMiddleCoordinates(ent2, &ent2MiddleX, &ent2MiddleY);

    if(ent2MiddleX < ent1MiddleX)
    {
        float tempX = ent1MiddleX;
        float tempY = ent1MiddleY;

        ent1MiddleX = ent2MiddleX;
        ent1MiddleY = ent2MiddleY;

        ent2MiddleX = tempX;
        ent2MiddleY = tempY;
    }

    float angle = C_AngleBetween2Points(ent1MiddleX, ent1MiddleY, ent2MiddleX, ent2MiddleY);


    float dx = cos(angle) * 20;
    float dy = sin(angle) * 20;

    float pointX = ent1MiddleX;
    float pointY = ent1MiddleY;


    bool collision = false;

    while(!collision && (pointX - ent1MiddleX <= ent2MiddleX - ent1MiddleX))
    {
        pointX += dx;
        pointY += dy;

        for(int i = 0 ; i < Vector_Count(&world->non_null_walls) && !collision ; i++)
        {

            Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);

            if(Entity_CheckDistance(ent1, wall, 1000) &&
               Wall_IsWall(wall) &&
               BoundingBox_CheckPointCollision(pointX, pointY, &wall->box))
            {
                collision = true;
            }
        }
    }


    return !collision;
}

bool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance)
{
    return ((abs(ent1->x - ent2->x) < distance) && (abs(ent1->y - ent2->y) < distance));
}

bool Entity_CollisionWithStuff(Entity* ent, World* world)
{

    bool collision_with_walls = Entity_CollisionWithWalls(ent, world);
    bool collision_with_mobs = Entity_CollisionWithMonsters(ent, &world->monsters_vector);
    //bool collision_with_props = Entity_CollisionWithProps(ent, &world->props_vector);
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
    Entity* collision_sides[5] = {NULL};
	int collisions_nb[5] = { 0 };

    bool collision = false;
    for (int i = 0; i < Vector_Count(&world->non_null_walls); i++)
	{
	    Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);
        if(wall->solid && Entity_CheckVeryClose(ent, wall))
        {
            Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &wall->box);
            if (collision_direction != None)
            {

                collision = true;
                collision_sides[collision_direction] = wall;
                collisions_nb[collision_direction]++;
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

    for(int i = 0 ; i < Vector_Count(&world->props_vector) ; i++)
    {
        Entity* prop = (struct Entity*)Vector_Get(&world->props_vector, i);
        if(Entity_CheckVeryClose(ent, prop))
        {
            Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &prop->box);

            if (collision_direction != None)
            {
                if(ent->t == Cat_Zombie && ent->zombieC->attack_timer >= ent->zombieC->attack_delay)
                {
                    Structure_GetAttacked(prop, ent);
                    ent->zombieC->attack_timer = 0;
                }
                collision = true;
                collision_sides[collision_direction] = prop;
                collisions_nb[collision_direction]++;
                ent->collision_direction = collision_direction;
            }
        }
    }

    if(ent->movementC->dy == 0)
    {

        if(collisions_nb[Right] == 1 &&
           ent->movementC->dx > 0 &&
           ent->box.right <= collision_sides[Right]->box.left)
        {
            printf("loool");
            if(ent->box.top < collision_sides[Right]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_sides[Right]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
        else if(collisions_nb[Left] == 1 &&
                ent->movementC->dx < 0 &&
                ent->box.left >= collision_sides[Left]->box.right)
        {
            if(ent->box.top < collision_sides[Left]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_sides[Left]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
    }

    if(ent->movementC->dx == 0)
    {
        if(collisions_nb[Top] == 1&& ent->movementC->dy < 0
                && ent->box.top >= collision_sides[Top]->box.bottom)
        {
            if(ent->box.left < collision_sides[Top]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_sides[Top]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
        else if(collisions_nb[Bottom]  == 1 && ent->movementC->dy > 0
                && ent->box.bottom <= collision_sides[Bottom]->box.top)
        {
            if(ent->box.left < collision_sides[Bottom]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_sides[Bottom]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
    }



    if (collisions_nb[Bottom] && ent->movementC->dy >= 0)
    {
        ent->movementC->dy = collision_sides[Bottom]->box.top - ent->box.bottom - 1;
    }

    if (collisions_nb[Top] && ent->movementC->dy <= 0)
    {
        ent->movementC->dy = collision_sides[Top]->box.bottom - ent->box.top + 1;
    }



    Box* temp2 = BoundingBox_CreateTemp(ent);


    if (collisions_nb[Right] &&
        BoundingBox_CheckCollision(&ent->box, temp2, &collision_sides[Right]->box) == Right &&
        ent->movementC->dx >= 0)
    {
        ent->movementC->dx = collision_sides[Right]->box.left - ent->box.right - 1;
    }

    if (collisions_nb[Left] &&
        BoundingBox_CheckCollision(&ent->box, temp2, &collision_sides[Left]->box) == Left &&
         ent->movementC->dx <= 0)
    {

        ent->movementC->dx = collision_sides[Left]->box.right - ent->box.left + 1;
    }

    free(temp2);


    if(collisions_nb[Left] > 1 && collisions_nb[Bottom] > 0)
    {
        ent->movementC->dx = collision_sides[Left]->box.right - ent->box.left + 1;
        ent->movementC->dy = collision_sides[Bottom]->box.top - ent->box.bottom - 1;
    }
    if(collisions_nb[Left] > 1 && collisions_nb[Top] > 0)
    {
        ent->movementC->dx = collision_sides[Left]->box.right - ent->box.left + 1;
        ent->movementC->dy = collision_sides[Top]->box.bottom - ent->box.top + 1;
    }
    if(collisions_nb[Right] > 1 && collisions_nb[Bottom] > 0)
    {
        ent->movementC->dx = collision_sides[Right]->box.left - ent->box.right - 1;
        ent->movementC->dy = collision_sides[Bottom]->box.top - ent->box.bottom - 1;
    }
    if(collisions_nb[Right] > 1 && collisions_nb[Top] > 0)
    {
        ent->movementC->dx = collision_sides[Right]->box.left - ent->box.right - 1;
        ent->movementC->dy = collision_sides[Top]->box.bottom - ent->box.top + 1;
    }






	free(temp);

	return collision;
}


bool Entity_CollisionWithProps(Entity* ent, Vector* props_vector)
{
    /*bool collision = false;
    Box* temp = BoundingBox_CreateTemp(ent);

    Entity* collision_sides[5] = {false};
    for(int i = 0 ; i < Vector_Count(props_vector) ; i++)
    {
        Entity* prop = (struct Entity*)Vector_Get(props_vector, i);
        if(Entity_CheckNear(ent, prop))
        {
            Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &prop->box);
            if (collision_direction != None)
            {
                if(ent->t == Cat_Zombie && ent->zombieC->attack_timer >= ent->zombieC->attack_delay)
                {
                    Structure_GetAttacked(prop, ent);
                        ent->zombieC->attack_timer = 0;
                }
                collision = true;
                collision_sides[collision_direction] = prop;
                ent->collision_direction = collision_direction;
            }
        }
    }

    if ((collision_sides[Bottom] && ent->movementC->dy > 0))
    {
        ent->movementC->dy = collision_sides[Bottom]->box.top - ent->box.bottom;
    }
    if ((collision_sides[Top] && ent->movementC->dy < 0))
    {
        ent->movementC->dy = collision_sides[Top]->box.bottom - ent->box.top;
    }
    if ((collision_sides[Right] && ent->movementC->dx > 0))
    {
        ent->movementC->dx = collision_sides[Right]->box.left - ent->box.right;
    }
    if ((collision_sides[Left] && ent->movementC->dx < 0))
    {
        ent->movementC->dx = collision_sides[Left]->box.right - ent->box.left;
    }

    if(ent->movementC->dy == 0)
    {
        if(collision_sides[Right] &&
           ent->movementC->dx > 0 &&
           ent->box.right <= collision_sides[Right]->x)
        {

            if(ent->box.top < collision_sides[Right]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_sides[Right]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
        else if(collision_sides[Left] &&
                ent->movementC->dx < 0 &&
                ent->box.left >= collision_sides[Left]->x)
        {
            if(ent->box.top < collision_sides[Left]->box.top)
            {
                ent->movementC->dy = -1;
            }
            else if(ent->box.bottom > collision_sides[Left]->box.bottom)
            {
                ent->movementC->dy = 1;
            }
        }
    }

    if(ent->movementC->dx == 0)
    {
        if(collision_sides[Top] && ent->movementC->dy < 0
                && ent->box.top >= collision_sides[Top]->box.bottom)
        {
            if(ent->box.left < collision_sides[Top]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_sides[Top]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
        else if(collision_sides[Bottom] && ent->movementC->dy > 0
                && ent->box.bottom <= collision_sides[Bottom]->box.top)
        {
            if(ent->box.left < collision_sides[Bottom]->box.left)
            {
                ent->movementC->dx = -1;
            }
            else if(ent->box.right > collision_sides[Bottom]->box.right)
            {
                ent->movementC->dx = 1;
            }
        }
    }

    free(temp);*/

    return false;
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

void Entity_LoseHealth(Entity* ent, int damage)
{
    ent->hp -= damage;

    if (ent->hp <= 0)
        ent->alive = false;
}

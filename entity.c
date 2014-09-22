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

Entity* Entity_Spawn()
{
    Entity* ent = (Entity*)malloc(sizeof(Entity));


    ent->t                          = Nothing;
    //position, box and texture should be in a physical component?
	ent->x                          = 0;
	ent->y                          = 0;
	ent-> box.top                   = 0;
	ent->box.left                   = 0;
	ent->box.width                  = 0;
	ent->box.height                 = 0;
	ent->box.right                  = 0;
	ent->box.bottom                 = 0;
    ent->solid = Jtrue;
    ent->texture                    = No_texture;
    ent->visible = Jtrue;

	//speed, angle, and velocity should be in a movement component?
	ent->dx                         = 0;
	ent->dy                         = 0;
	ent->speed                      = 0;
	ent->angle                      = 0;


	ent->alive                      = Jtrue;
	ent->hp                         = 0;
	ent->last_creation              = 0;


    ent->damage                     = 0;
    ent->stamina                    = 0;
    ent->door_opening_timer         = 0;

	//components
    ent->weapons_component          = NULL;
    ent->explosive_component        = NULL;
    ent->zombieC                    = NULL;
    ent->camera                     = NULL;



	return ent;
}

void Entity_Destroy(Entity* ent)
{
    free(ent->weapons_component);
    free(ent->explosive_component);
    free(ent->zombieC);
    free(ent->camera);
}

Jbool Entity_CheckNear(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 600 && abs(ent1->y - ent2->y) < 600);
}

void Entity_CalculateVelocity(Entity* ent)
{
    ent->dx = cos(ent->angle ) * ent->speed * delta_g;
    ent->dy = sin(ent->angle ) * ent->speed * delta_g;
}

/*void Entity_CalculateVelocityFromAngle(Entity* ent)
{
    ent->dx = cos(ent->angle) * ent->speed * delta_g;
    ent->dy = sin(ent->angle) * ent->speed * delta_g;
}*/

void Entity_GetMiddleCoordinates(Entity* ent, float* middleX, float* middleY)
{
    *middleX = (float)(ent->x + ent->box.width / 2);
    *middleY = (float)(ent->y + ent->box.height / 2);
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

Jbool Entity_CheckCanSeeEntity(Entity* ent1, Entity* ent2, World* world)
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
    Jbool can_see = Jtrue;

    float angle_to_ent2 = C_AngleBetween2Entities(ent1, ent2);

    while(line_distance <= distance_to_ent2 &&
          can_see == Jtrue)
    {
        pointX += cos(angle_to_ent2) * 10;//magic number yay!!
        pointY += sin(angle_to_ent2) * 10;
        line_distance = C_DistanceBetween2Points(ent1MiddleX, ent1MiddleY,
                                                 pointX, pointY);

        //looping every walls close to the zombie
        for(int i = 0 ; i < world->map_size ; i++)
        {
            if(world->map[i]->solid && Entity_CheckNear(ent1, world->map[i]))
            {
                //if the end of the line hits a wall, the zombie can't see the player
                if(BoundingBox_CheckPointCollision(pointX, pointY, &world->map[i]->box))
                {
                    can_see = Jfalse;
                }
            }
        }
    }

    return can_see;
}

Jbool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance)
{

    float pythDistance = Entity_DistanceBetweenTwoEntities(ent1, ent2);

    if(pythDistance < distance)
        return Jtrue;
    else
        return Jfalse;
}

Jbool Entity_CollisionWithStuff(Entity* ent, World* world)
{

    Jbool collision_with_walls = Entity_CollisionWithWalls(ent, world->map, world->map_size);
    Jbool collision_with_mobs = Entity_CollisionWithMonsters(ent, &world->monsters_vector);

    return (collision_with_walls || collision_with_mobs);
}



Jbool Entity_CollisionWithWalls(Entity* ent, Entity** map, int map_size)
{
    Box* temp = BoundingBox_CreateTemp(ent);
    Entity* collision_wall[5];
	int walls_touched[5] = { 0 };

    Jbool collision = Jfalse;
    for (int i = 0; i < map_size; i++)
	{
		if (map[i] != NULL)
		{
		    if(Entity_CheckNear(ent, map[i]) && map[i]->x != 0 && map[i]->solid)
            {
                Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &map[i]->box);
                if (collision_direction != None)
                {
                    collision = Jtrue;
                    collision_wall[collision_direction] = map[i];
                    walls_touched[collision_direction]++;
                    ent->collision_direction = collision_direction;

                    if(ent->t == Cat_Zombie && ent->zombieC->aggressive &&
                       map[i]->solid &&
                       (map[i]->t == Cat_Door || map[i]->t == Cat_Wall))
                    {
                        printf("%d\n", ent->damage);
                        Structure_GetAttacked(map[i], ent);
                    }
                }
            }
		}
	}


    if(ent->dy == 0)
    {
        if(walls_touched[Right] == 1 &&
           ent->dx > 0 &&
           ent->box.right <= collision_wall[Right]->x)
        {
            if(ent->box.top < collision_wall[Right]->box.top)
            {
                ent->dy = -1;
            }
            else if(ent->box.bottom > collision_wall[Right]->box.bottom)
            {
                ent->dy = 1;
            }
        }
        else if(walls_touched[Left] == 1 &&
                ent->dx < 0 &&
                ent->box.left >= collision_wall[Left]->x)
        {
            if(ent->box.top < collision_wall[Left]->box.top)
            {
                ent->dy = -1;
            }
            else if(ent->box.bottom > collision_wall[Left]->box.bottom)
            {
                ent->dy = 1;
            }
        }
    }

    if(ent->dx == 0)
    {
        if(walls_touched[Top] == 1 && ent->dy < 0
                && ent->box.top >= collision_wall[Top]->box.bottom)
        {
            if(ent->box.left < collision_wall[Top]->box.left)
            {
                ent->dx = -1;
            }
            else if(ent->box.right > collision_wall[Top]->box.right)
            {
                ent->dx = 1;
            }
        }
        else if(walls_touched[Bottom] == 1 && ent->dy > 0
                && ent->box.bottom <= collision_wall[Bottom]->box.top)
        {
            if(ent->box.left < collision_wall[Bottom]->box.left)
            {
                ent->dx = -1;
            }
            else if(ent->box.right > collision_wall[Bottom]->box.right)
            {
                ent->dx = 1;
            }
        }
    }


    Jbool flatBottomTop = Jfalse;


	if (walls_touched[Bottom] || walls_touched[Top])
		flatBottomTop = Jtrue;

	if (walls_touched[Bottom] > 0 && walls_touched[Left] > 1)
	{
		ent->dx = collision_wall[Left]->box.right - ent->box.left;
		ent->dy = 0;
	}
	if (walls_touched[Bottom] > 0 && walls_touched[Right] > 1)
	{
		ent->dx = collision_wall[Right]->box.left - ent->box.right;
		ent->dy = 0;
	}
	if (walls_touched[Top] > 0 && walls_touched[Left] > 1)
	{
		ent->dx = collision_wall[Left]->box.right - ent->box.left;
		ent->dy = 0;
	}
	if (walls_touched[Top] > 0 && walls_touched[Right] > 1)
	{
		ent->dx = collision_wall[Right]->box.left - ent->box.right;
		ent->dy = 0;
	}

	if (walls_touched[Bottom] && ent->dy > 0)
	{
		ent->dy = collision_wall[Bottom]->box.top - ent->box.bottom;
	}

	if (walls_touched[Top] && ent->dy < 0)
	{

		ent->dy = collision_wall[Top]->box.bottom - ent->box.top;
	}




	if (!flatBottomTop)
	{
		if (walls_touched[Right] && ent->dx > 0)
		{
		    ent->dx = collision_wall[Right]->box.left - ent->box.right;
		}

		if (walls_touched[Left] && ent->dx < 0)
		{
		    ent->dx = collision_wall[Left]->box.right - ent->box.left;
		}
	}

	free(temp);

	return collision;
}



Jbool Entity_CollisionWithMonsters(Entity* ent, Vector* monsters_vector)
{
    Jbool collision = Jfalse;
    Box* temp = BoundingBox_CreateTemp(ent);

    if(ent->t != Cat_Player ||
       (ent->t == Cat_Player && ent->invulnerability_timer <= 0))
    {
        Entity* collision_sides[5] = {Jfalse};
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
                        collision = Jtrue;
                        collision_sides[collision_direction] = mob_to_check;
                        ent->collision_direction = collision_direction;
                    }
                }
            }
        }

        if ((collision_sides[Bottom] && ent->dy > 0) ||
            (collision_sides[Top] && ent->dy < 0))
        {
            ent->dy = 0;
        }

        if ((collision_sides[Right] && ent->dx > 0) ||
            (collision_sides[Left] && ent->dx < 0))
        {
            ent->dx = 0;
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
    Jbool collision = Jfalse;
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
        ent->alive = Jfalse;
}

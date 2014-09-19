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

Entity* Entity_Spawn()
{
    Entity* ent = (Entity*)malloc(sizeof(Entity));


    ent->t                         = Nothing;
	ent->x                         = 0;
	ent->y                         = 0;
	ent->dx                        = 0;
	ent->dy                        = 0;
	ent->speed                     = 0;
	ent->angle                     = 0;
	ent->aggressive = Jfalse;
	//FUCK, this should be a pointer...

	ent-> box.top = 0;
	ent->box.left = 0;
	ent->box.width = 0;
	ent->box.height = 0;
	ent->box.right = 0;
	ent->box.bottom = 0;
	ent->alive                     = Jtrue;
	ent->hp                        = 0;
    ent->weapons_component          = NULL;
    ent->explosive_component = NULL;
	ent->last_creation             = 0;
	ent->camera                    = NULL;
    ent->texture                   = No_texture;
    ent->damage                    = 0;
    ent->zombie_type               = Not_a_zombie;
    ent->stamina                   = 0;
    ent->vision_distance           = 0;
	return ent;
}

Entity Entity_SpawnOnStack()
{
    Entity ent;
    ent.t                         = Nothing;
	ent.x                         = 0;
	ent.y                         = 0;
	ent.dx                        = 0;
	ent.dy                        = 0;
	ent.speed                     = 0;
	ent.angle                     = 0;
	//FUCK, this should be a pointer...
	//ent. box                       = NULL;
	ent.alive                     = Jtrue;
	ent.hp                        = 0;
    ent.weapons_component          = NULL;
    ent.explosive_component = NULL;
	ent.last_creation             = 0;
	ent.camera                    = NULL;
    ent.texture                   = No_texture;
    ent.damage                    = 0;
    ent.zombie_type               = Not_a_zombie;
	return ent;
}

Jbool Entity_CheckNear(Entity* ent1, Entity* ent2)
{
    return (abs(ent1->x - ent2->x) < 600 && abs(ent1->y - ent2->y) < 600);
}

void moveEntity(Entity* ent, float x, float y)
{
	ent->x += x;
	ent->y += y;

	BoundingBox_Update(ent);
}

void moveToPosition(Entity* ent, float x, float y)
{
	ent->x = x;
	ent->y = y;
	BoundingBox_Update(ent);
}

Jbool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance)
{
    float distanceX =  ent2->x - ent1->x;
    float distanceY = ent2->y - ent1->y;
    float pythDistance = sqrt(distanceX * distanceX +
                                distanceY * distanceY);

    if(pythDistance < distance)
        return Jtrue;
    else
        return Jfalse;
}
void Entity_CollisionWithStuff(Entity* ent, World* world)
{
    Entity* collision_wall[5];
	int walls_touched[5] = { 0 };

	Box* temp = BoundingBox_CreateTemp(ent);

    Entity_CollisionWithWalls(ent, world->map, world->map_size, temp, collision_wall, walls_touched);

    free(temp);


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

}

void Entity_CalculateVelocityFromAngle(Entity* ent, int delta)
{
    ent->dx = cos(ent->angle) * ent->speed * delta;
    ent->dy = sin(ent->angle) * ent->speed * delta;
}

void Entity_CollisionWithWalls(Entity* ent, Entity** map, int map_size, Box* temp, Entity** collision_wall, int* walls_touched)
{

    /*if(ent->vision_distance != 0)
    {
        for(int i = 0 ; i < 10 ; i++)
        {
            ent->vision_points[i].x = ent->x + ent->box.width / 2;
            ent->vision_points[i].y = ent->y + ent->box.height / 2;
            float angle = ent->angle;
            if(i < 5)
            {
                angle = ent->angle - 0.05 * i;
            }
            if(i == 5)
            {
                angle = ent->angle;
            }
            if(i > 5)
            {
                angle = ent->angle + 0.05 * i;
            }


            float distance = 0;
            while(distance < ent->vision_distance)
            {
                ent->vision_points[i].x += cos(angle) * ent->speed * (float)(delta_g);
                ent->vision_points[i].y += sin(angle) * ent->speed * (float)(delta_g);

                distance += ent->speed * (float)(delta_g);

                for (int j = 0; j < map_size; j++)
                {
                    if (map[j] != NULL)
                    {
                        if(Entity_CheckNear(ent, map[j]))
                        {
                            if (BoundingBox_CheckPointCollision(ent->vision_points[i].x, ent->vision_points[i].y, &map[j]->box))
                            {

                                distance = ent->vision_distance;
                            }
                        }
                    }
                }
            }

           // printf("%f", ent->vision_distance);
        }
    }*/

    for (int i = 0; i < map_size; i++)
	{
		if (map[i] != NULL)
		{
		    if(Entity_CheckNear(ent, map[i]))
            {
                Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &map[i]->box);
                if (collision_direction != None)
                {
                    collision_wall[collision_direction] = map[i];
                    walls_touched[collision_direction]++;
                }
            }
		}
	}
}

void CollisionWithMonsters(Entity* ent, Vector* monsters_vector)
{
    if(ent->t != Cat_Player ||
       (ent->t == Cat_Player && ent->invulnerability_timer <= 0))
    {
        Box* temp = BoundingBox_CreateTemp(ent);

        Entity* collision_sides[5] = {Jfalse};
        Jbool collision = Jfalse;
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
                    }
                }
            }
        }

        free(temp);

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

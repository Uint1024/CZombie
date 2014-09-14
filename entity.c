#include "common.h"
#include "entity.h"
#include "math.h"
#include "boundingBox.h"
#include <math.h>
#include <stdlib.h>
#include "camera.h"
#include "boundingBox.h"


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
	//FUCK, this should be a pointer...
	//ent-> box                       = NULL;
	ent->alive                     = Jtrue;
	ent->hp                        = 0;
    ent->weapons_component          = NULL;
    ent->explosive_component = NULL;
	ent->last_creation             = 0;
	ent->camera                    = NULL;
    ent->texture                   = No_texture;
    ent->damage                    = 0;
	return ent;
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



void CollisionWithMonsters(Entity* ent, Vector* monsters_vector)
{
    Box* temp = BoundingBox_CreateTemp(ent);
    Entity* collision_sides[5] = {Jfalse};

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != ent)
        {
            Entity* mob_to_check = (struct Entity*)Vector_Get(monsters_vector, i);
            Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &mob_to_check->box);
            if (collision_direction != None)
            {
                collision_sides[collision_direction] = mob_to_check;
            }
        }
    }

    free(temp);

    if (collision_sides[Bottom] && ent->dy > 0)
	{
		ent->dy = 0;
	}

	if (collision_sides[Top] && ent->dy < 0)
	{

		ent->dy = 0;
	}


    if (collision_sides[Right] && ent->dx > 0)
    {
        ent->dx = 0;
    }

    if (collision_sides[Left] && ent->dx < 0)
    {
        ent->dx = 0;
    }
}

void CalculateVelocity(Entity* p, Entity* map, int map_size)
{
	Entity* collision_wall[5];
	int walls_touched[5] = { 0 };

	Box* temp = BoundingBox_CreateTemp(p);
	for (int i = 0; i < map_size; i++)
	{
		if (map[i].t == Wall)
		{
			Direction collision_direction = BoundingBox_CheckCollision(&p->box, temp, &map[i].box);
			if (collision_direction != None)
			{
				collision_wall[collision_direction] = &map[i];
				walls_touched[collision_direction]++;
			}
		}
	}

    free(temp);


	Jbool flatBottomTop = Jfalse;


	if (walls_touched[Bottom] || walls_touched[Top])
		flatBottomTop = Jtrue;

	if (walls_touched[Bottom] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
	}
	if (walls_touched[Bottom] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
	}
	if (walls_touched[Top] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
	}
	if (walls_touched[Top] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
	}

	if (walls_touched[Bottom] && p->dy > 0)
	{
		p->dy = collision_wall[Bottom]->box.top - p->box.bottom;
	}

	if (walls_touched[Top] && p->dy < 0)
	{

		p->dy = collision_wall[Top]->box.bottom - p->box.top;
	}


	if (!flatBottomTop)
	{
		if (walls_touched[Right] && p->dx > 0)
		{
		    p->dx = collision_wall[Right]->box.left - p->box.right;
		}

		if (walls_touched[Left] && p->dx < 0)
		{
		    p->dx = collision_wall[Left]->box.right - p->box.left;
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

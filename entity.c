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

void Entity_CollisionWithWalls(Entity* ent, Entity* map, int map_size, Box* temp, Entity** collision_wall, int* walls_touched)
{
    for (int i = 0; i < map_size; i++)
	{
		if (map[i].t == Wall)
		{
			Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &map[i].box);
			if (collision_direction != None)
			{
				collision_wall[collision_direction] = &map[i];
				walls_touched[collision_direction]++;
			}
		}
	}
}

void CollisionWithMonsters(Entity* ent, Vector* monsters_vector)
{
    if(ent->t != Player ||
       (ent->t == Player && ent->invulnerability_timer <= 0))
    {
        Box* temp = BoundingBox_CreateTemp(ent);

        Entity* collision_sides[5] = {Jfalse};
        Jbool collision = Jfalse;
        for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
        {
            if(Vector_Get(monsters_vector, i) != ent)
            {
                Entity* mob_to_check = (struct Entity*)Vector_Get(monsters_vector, i);
                Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &mob_to_check->box);
                if (collision_direction != None)
                {
                    collision = Jtrue;
                    collision_sides[collision_direction] = mob_to_check;
                }
            }
        }

        free(temp);

        if (collision_sides[Bottom] && ent->dy > 0 ||
            collision_sides[Top] && ent->dy < 0)
        {
            ent->dy = 0;
        }

        if (collision_sides[Right] && ent->dx > 0 ||
            collision_sides[Left] && ent->dx < 0)
        {
            ent->dx = 0;
        }

        if(collision && ent->t == Player)
        {
            Player_TakeDamage(ent, 1, collision_sides);

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

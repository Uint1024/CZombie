#include "common.h"
#include "entity.h"
#include "math.h"
#include "boundingBox.h"
#include <math.h>
#include <stdlib.h>
#include "camera.h"
#include "boundingBox.h"

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

void CollisionWithMonsters(Entity* ent, List* mob_list)
{
    Box* temp = BoundingBox_CreateTemp(ent);
    Entity* collision_sides[5] = {Jfalse};

    for(ListNode* node = mob_list->first ; node != NULL ; node = node->next)
    {

        if(node->value != ent)
        {

            Entity* mob_to_check = (struct Entity*)node->value;
            Direction collision_direction = BoundingBox_CheckCollision(&ent->box, temp, &mob_to_check->box);
            if (collision_direction != None)
            {
                collision_sides[collision_direction] = mob_to_check;
            }
        }
    }

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
    Jbool trajectoryCutShot = Jfalse;
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


	Jbool corner = Jfalse;
	Jbool flatBottomTop = Jfalse;


	if (walls_touched[Bottom] || walls_touched[Top])
		flatBottomTop = Jtrue;

	if (walls_touched[Bottom] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Bottom] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Top] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Top] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
		corner = Jtrue;
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

void CalculateVelocityPlayerCamera(Entity* p, Entity* camera, Entity* map, int map_size)
{
    Jbool trajectoryCutShot = Jfalse;
	Jbool collision[5] = { Jfalse };
	Entity* collision_wall[5];
	int walls_touched[5] = { 0 };

	Box* temp = BoundingBox_CreateTemp(p);
	for (int i = 0; i < map_size; i++)
	{
		if (map[i].t == Wall)
		{
			Direction collision_direction = BoundingBox_CheckCollision(&p->box, temp, &map[i].box);
			collision[collision_direction] = Jtrue;
			if (collision_direction != None)
			{
				collision_wall[collision_direction] = &map[i];
				walls_touched[collision_direction]++;
			}
		}
	}

	Jbool corner = Jfalse;
	Jbool flatBottomTop = Jfalse;


	if (walls_touched[Bottom] || walls_touched[Top])
		flatBottomTop = Jtrue;

	if (walls_touched[Bottom] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Bottom] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Top] > 0 && walls_touched[Left] > 1)
	{
		p->dx = collision_wall[Left]->box.right - p->box.left;
		p->dy = 0;
		corner = Jtrue;
	}
	if (walls_touched[Top] > 0 && walls_touched[Right] > 1)
	{
		p->dx = collision_wall[Right]->box.left - p->box.right;
		p->dy = 0;
		corner = Jtrue;
	}

	if (walls_touched[Bottom] && p->dy > 0)
	{
		//p->dy = collision_wall[Bottom]->box.top - p->box.bottom;
        p->dy = 0;
        moveEntity(camera, 0, collision_wall[Bottom]->box.top - p->box.bottom);
	}

	if (walls_touched[Top] && p->dy < 0)
	{

		//p->dy = collision_wall[Top]->box.bottom - p->box.top;
        p->dy = 0;
        moveEntity(camera, 0, collision_wall[Top]->box.bottom - p->box.top);
	}


	if (!flatBottomTop)
	{
		if (walls_touched[Right] && p->dx > 0)
		{
		    //p->dx = 0;
		    //moveEntity(camera, (collision_wall[Right]->box.left - p->box.right), 0);
			p->dx = collision_wall[Right]->box.left - p->box.right;
		}

		if (walls_touched[Left] && p->dx < 0)
		{
		    p->dx = 0;
		    moveEntity(camera, (collision_wall[Left]->box.right - p->box.left), 0);
			//p->dx = collision_wall[Left]->box.right - p->box.left;
		}
	}
}

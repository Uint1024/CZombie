#include <math.h>
#include <stdlib.h>
#include "linkedList.h"
#include "bullet.h"
#include "entity.h"

Entity* Bullet_Create(float x, float y, float angle, float speed)
{
	Entity* bullet = Entity_Spawn();

	bullet->t = Bullet;
	bullet->texture = Bullet_tex;
	bullet->x = x;
	bullet->y = y;
	bullet->box.height = 10;
	bullet->box.width = 10;
	bullet->angle = angle;
	bullet->speed = speed;
	return bullet;
}

void Bullet_Move(Entity* bullet, Entity* map, int map_size, Vector* monsters_vector, int delta, Entity* camera)
{
	bullet->dx = cos(bullet->angle) * bullet->speed * delta;
	bullet->dy = sin(bullet->angle) * bullet->speed  * delta;
	moveEntity(bullet, bullet->dx, bullet->dy);

	//set alive to false if out of screen
	Direction out_of_screen = BoundingBox_CheckOutOfScreen(&bullet->box, camera);
	if (out_of_screen != None)
		bullet->alive = Jfalse;

	for (int i = 0; i < map_size && bullet->alive == Jtrue; i++)
	{
		bullet->alive = !BoundingBox_CheckSimpleCollision(&bullet->box, &map[i].box);
	}

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
		if (mob->t == Zombie)
		{
			if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box))
			{
			    Entity_LoseHealth(mob, 1);
                bullet->alive = Jfalse;
			}
		}
    }
}

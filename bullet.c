#include <math.h>
#include <stdlib.h>
#include "linkedList.h"
#include "bullet.h"
#include "entity.h"

Entity* Bullet_Create(float x, float y, float angle, float speed)
{
	Entity* bullet = (Entity*)malloc(sizeof(Entity));

	bullet->t = Bullet;
	bullet->texture = Bullet_tex;
	bullet->x = x;
	bullet->y = y;
	bullet->box.height = 10;
	bullet->box.width = 10;
	bullet->angle = angle;
	bullet->speed = speed;
	bullet->alive = Jtrue;

	return bullet;
}

void Bullet_Move(Entity* bullet, Entity* map, int map_size, List* monsters, int delta, Entity* camera)
{
	bullet->dx = cos(bullet->angle) * bullet->speed * delta;
	bullet->dy = sin(bullet->angle) * bullet->speed  * delta;
	moveEntity(bullet, bullet->dx, bullet->dy);

	//set alive to false if out of screen
	Direction out_of_screen = BoundingBox_CheckOutOfScreen(&bullet->box, camera);
	if (out_of_screen != None)
		bullet->alive = Jfalse;

	Jbool collision = Jfalse;
	for (int i = 0; i < map_size && bullet->alive == Jtrue; i++)
	{
		bullet->alive = !BoundingBox_CheckSimpleCollision(&bullet->box, &map[i].box);
	}

	ListNode *_nodeMonster = monsters->first;
	while (_nodeMonster != NULL && bullet->alive)
	{
		Entity* mob = (struct Entity*)_nodeMonster->value;
		if (mob->t == Zombie)
		{
			if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box))
			{
				mob->hp -= 1;
				if (mob->hp <= 0)
					mob->alive = Jfalse;

				bullet->alive = Jfalse;
			}
		}
		_nodeMonster = _nodeMonster->next;
	}
}
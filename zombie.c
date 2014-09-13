#include "math.h"
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
void UpdateZombie(Entity* z, Entity* player, Entity* map, int map_size, List* mob_list, int delta)
{
	float adjacent = player->x - z->x;
	float opposite = player->y - z->y;

	float angle = atan2f(opposite, adjacent);
	z->angle = angle;
	z->dx = cos(angle) * z->speed * delta;
	z->dy = sin(angle) * z->speed * delta;
    CollisionWithMonsters(z, mob_list);
    CalculateVelocity(z, map, map_size);

	moveEntity(z, z->dx, z->dy);
}

Entity* CreateZombie(float x, float y, float speed)
{
	Entity* z = Entity_Spawn();

	z->t = Zombie;
	z->texture = Zombie_tex;
	z->x = x;
	z->y = y;
	z->box.height = 20;
	z->box.width = 20;
	z->speed = speed;
	z->hp = 5;
	return z;
}

void Zombie_Die(Entity* zombie, List* bonus_list)
{
    int random = rand() % 10;

    if(random <= 5)
    {
        List_push(bonus_list, Bonus_Create(Ammo, zombie->x, zombie->y));
    }
}

#include "math.h"
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
#include "world.h"

void Zombie_Update(Entity* z, int delta, World* world)
{
	float adjacent = world->player.x - z->x;
	float opposite =world->player.y - z->y;

	float angle = atan2f(opposite, adjacent);
	z->angle = angle;
	z->dx = cos(angle) * z->speed * delta;
	z->dy = sin(angle) * z->speed * delta;
    CollisionWithMonsters(z, &world->monsters_vector);
    CalculateVelocity(z, world->map, world->map_size);
    Entity_CollisionWithExplosions(z, &world->explosions_vector);

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

void Zombie_Die(Entity* zombie, Vector* bonus_vector)
{
    int random = rand() % 10;

    if(random <= 5)
    {
        Vector_Push(bonus_vector, Bonus_Create(Ammo, zombie->x, zombie->y));
        //List_push(bonus_list, Bonus_Create(Ammo, zombie->x, zombie->y));
    }
}

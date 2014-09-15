#include "math.h"
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
#include "world.h"

void Zombie_Update(Entity* z, int delta, World* world)
{
	z->angle = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);

	z->dx = cos(z->angle ) * z->speed * delta;
	z->dy = sin(z->angle ) * z->speed * delta;
    CollisionWithMonsters(z, &world->monsters_vector);
    Entity_CollisionWithStuff(z, world);
    Entity_CollisionWithExplosions(z, &world->explosions_vector);

	moveEntity(z, z->dx, z->dy);
}

Entity* CreateZombie(Zombie_Type type, float x, float y)
{
	Entity* z = Entity_Spawn();

	z->t = type;
    z->x = x;
	z->y = y;

    switch(type)
    {
    case Normal_Zombie:
        z->texture = Zombie_tex;
        z->box.height = 20;
        z->box.width = 20;
        z->speed = 0.2;
        z->hp = 2;
        break;
    case Fast_Zombie:
        z->texture = FastZombie_tex;
        z->box.height = 20;
        z->box.width = 20;
        z->speed = 0.6;
        z->hp = 2;
        break;
    case Heavy_Zombie:
        z->texture = HeavyZombie_tex;
        z->box.height = 40;
        z->box.width = 40;
        z->speed = 0.1;
        z->hp = 20;
        break;
    case Huge_Zombie:
        z->texture = HugeZombie_tex;
        z->box.height = 100;
        z->box.width = 100;
        z->speed = 0.2;
        z->hp = 150;
        break;
    }



	return z;
}

void Zombie_Die(Entity* zombie, Vector* bonus_vector)
{
    int random = rand() % 10;

    if(random <= 2)
    {
        Bonus_GenerateRandom(bonus_vector, zombie);
    }
}

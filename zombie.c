#include "math.h"
#include <stdio.h>
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
#include "world.h"
#include "weapon.h"

void Zombie_Update(Entity* z, int delta, World* world)
{
	z->angle = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);

	z->dx = cos(z->angle ) * z->speed * delta;
	z->dy = sin(z->angle ) * z->speed * delta;
    CollisionWithMonsters(z, &world->monsters_vector);
    Entity_CollisionWithStuff(z, world);
    Entity_CollisionWithExplosions(z, &world->explosions_vector);

    if(z->zombie_type == Heavy_Zombie || z->zombie_type == Trooper_Zombie)
    {
        Zombie_Shoot(z, world);
    }
	moveEntity(z, z->dx, z->dy);
}

void  Zombie_Shoot(Entity* z, World* world)
{

    float angle = C_AngleBetween2Entities(z, &world->player);
    float originX = z->x + z->box.width/2;
    float originY = z->y + z->box.height/2;

    WeaponsComponent_TryToShoot(z->weapons_component, originX, originY,
                                angle, &world->bullets_vector, 0, 0);
}

Entity* CreateZombie(Zombie_Type type, float x, float y)
{
	Entity* z = Entity_Spawn();

	z->t = type;
    z->x = x;
	z->y = y;
    z->zombie_type = type;

    BoundingBox_Create(z, 0,0);

    z->texture = zombie_templates_g[type]->texture;
    z->box.height = zombie_templates_g[type]->box.height;
    z->box.width = zombie_templates_g[type]->box.width;
    z->speed = zombie_templates_g[type]->speed;
    z->hp = zombie_templates_g[type]->hp;
    z->damage = zombie_templates_g[type]->damage;
    if(zombie_templates_g[type]->weapons_component != NULL)
    {
        z->weapons_component = WeaponsComponent_Create(Jtrue);
        WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                              Weapon_Create(Fireball_w));
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

    free(zombie->weapons_component);
}

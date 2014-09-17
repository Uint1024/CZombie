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
    switch(type)
    {
    case Normal_Zombie:
        z->texture = Zombie_tex;
        z->box.height = 20;
        z->box.width = 20;
        z->speed = 0.2;
        z->hp = 2;
        z->damage = 2;
        break;
    case Fast_Zombie:
        z->texture = FastZombie_tex;
        z->box.height = 20;
        z->box.width = 20;
        z->speed = 0.25;
        z->hp = 2;
        z->damage = 4;
        break;
    case Heavy_Zombie:
        z->texture = HeavyZombie_tex;
        z->box.height = 40;
        z->box.width = 40;
        z->speed = 0.1;
        z->hp = 20;
        z->damage = 10;
        z->weapons_component = WeaponsComponent_Create(Jtrue);
        WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                              Weapon_Create(Fireball_w));
        break;
    case Trooper_Zombie:
        z->texture = TrooperZombie_tex;
        z->box.height = 60;
        z->box.width = 45;
        z->speed = 0.1;
        z->hp = 20;
        z->damage = 10;
        z->weapons_component = WeaponsComponent_Create(Jtrue);
        WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                              Weapon_Create(TripleFireball_w));
        break;
    case Huge_Zombie:
        z->texture = HugeZombie_tex;
        z->box.height = 100;
        z->box.width = 100;
        z->speed = 0.2;
        z->hp = 150;
        z->damage = 20;
        break;

    case NB_ZOMBIE_TYPES:
        printf("Error in Zombie_Create, tried to create zombie type \"NB_ZOMBIE_TYPES\"");
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

    free(zombie->weapons_component);
}

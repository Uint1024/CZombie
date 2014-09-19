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
    Zombie_Ai(z, world);
    CollisionWithMonsters(z, &world->monsters_vector);
    Entity_CollisionWithStuff(z, world);
    Entity_CollisionWithExplosions(z, &world->explosions_vector);

    if(z->zombie_type == Heavy_Zombie || z->zombie_type == Trooper_Zombie)
    {
        Zombie_Shoot(z, world);
    }

    if(z->aggressive)
    {
        z->angle = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);
        z->dx = cos(z->angle ) * z->speed * delta;
        z->dy = sin(z->angle ) * z->speed * delta;
        moveEntity(z, z->dx, z->dy);
    }
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

	z->vision_distance = 400;

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

void Zombie_Ai(Entity* z, World* world)
{
    Entity* player = &world->player;
    float angle_to_player = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);
    float diff = angle_to_player - z->angle;

	if((diff > -0.4 && diff < 0.4 && Entity_CheckDistance(z, player, z->vision_distance)) ||
        Entity_CheckDistance(z, player, 100))
    {
        Zombie_BecomeAggressive(z, world);
    }

    Zombie_CalculateRandomPath(z);
}


void Zombie_CalculateRandomPath(Entity z*)
{

}

void Zombie_BecomeAggressive(Entity* z, World* world)
{
    z->aggressive = Jtrue;
    Vector* monsters = &world->monsters_vector;

    for(int i = 0 ; i < Vector_Count(monsters) ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(monsters, i);
        if(Entity_CheckDistance(z, mob, 200) && !mob->aggressive)
        {
            Zombie_BecomeAggressive(mob, world);
        }
    }
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

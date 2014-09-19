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

    Entity_CalculateVelocity(z);
    Zombie_Ai(z, world);


    if(!z->idling)
    {
        Jbool collision = Entity_CollisionWithStuff(z, world);

        if(collision && !z->aggressive)
        {
            Zombie_NewTrajectory(z);
        }
        //Entity_CollisionWithExplosions(z, &world->explosions_vector);

        if(z->aggressive &&
           z->zombie_type == Heavy_Zombie || z->zombie_type == Trooper_Zombie)
        {
            Zombie_Shoot(z, world);
        }

        moveEntity(z, z->dx, z->dy);
    }

}

void Zombie_GetAttacked(Entity* z, int damage, World* world)
{
    Zombie_BecomeAggressive(z, world);
    Entity_LoseHealth(z, damage);
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

	z->vision_distance = 700;

	z->t = type;
    z->x = x;
	z->y = y;
    z->zombie_type = type;
    BoundingBox_Create(z, 0,0);
    z->angle = C_GenerateRandomAngle();
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
    float angle_to_player = C_AngleBetween2Entities(z, player);

    //convert angle to 2pi circle and compare them
    float diff = C_DifferenceBetweenAngles(z->angle, angle_to_player);

    //check if player is in field of view, or if the player is too close
	if(!z->aggressive &&
        (diff > -1.2f && diff < 1.2f &&
         Entity_CheckDistance(z, player, z->vision_distance)) ||

        Entity_CheckDistance(z, player, 70))
    {
        /*check if the zombie can see the player, if it can then it becomes aggressive

        to do that we draw a line from the zombie to the player,
        if this line hits any wall then the zombie can't see him*/

        Jbool can_see = Entity_CheckCanSeeEntity(z, player, world);

        if(can_see)
        {
            Zombie_BecomeAggressive(z, world);
        }

    }

    if(z->aggressive)
    {
        if(!Entity_CheckDistance(z, player, 600))
        {
            Zombie_BecomeCalm(z);

        }

        z->angle = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);

    }

    if(!z->aggressive)
    {
        Zombie_CalculateRandomPath(z);
    }

}

void Zombie_BecomeCalm(Entity* z)
{
    z->aggressive = Jfalse;
    z->speed = calm_speed_g[z->zombie_type];
}

//when there's a collision, this generate a new angle in the opposite direction
void Zombie_NewTrajectory(Entity* z)
{
    z->angle = C_GenerateAngleInOppositeDirection(z->collision_direction);
}

void Zombie_CalculateRandomPath(Entity* z)
{
    z->rand_move_timer += delta_g;
    if(z->rand_move_timer >= z->rand_move_every)
    {
        z->idling = z->idling ? Jfalse : Jtrue;
        z->rand_move_timer = 0;
        z->rand_move_every = 3000 + (rand() % 10000);

        if(!z->idling)
        {
            z->angle = C_GenerateRandomAngle();
        }
    }
}

void Zombie_BecomeAggressive(Entity* z, World* world)
{
    z->aggressive = Jtrue;
    z->idling = Jfalse;
    z->speed = angry_speed_g[z->zombie_type];
    Vector* monsters = &world->monsters_vector;

    //the zombie tells the closest zombies that he's spotted the player
    for(int i = 0 ; i < Vector_Count(monsters) ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(monsters, i);
        if(Entity_CheckDistance(z, mob, 200) && !mob->aggressive)
        {
            //zombie will become aggressive only if he can be seen by z
            if(Entity_CheckCanSeeEntity(z, mob, world))
            {
                Zombie_BecomeAggressive(mob, world);
            }

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

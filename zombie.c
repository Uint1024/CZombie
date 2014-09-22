#include "math.h"
#include <stdio.h>
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
#include "world.h"
#include "weapon.h"

ZombieC* ZombieC_Create()
{
    ZombieC* zc = (ZombieC*)malloc(sizeof(ZombieC));

    zc->aggressive = Jfalse;
    zc->idling = Jfalse;
    zc->rand_move_every = 1;
    zc->rand_move_timer = 1;
    zc->vision_distance = 450;
    zc->vision_width = 1.2;
    zc->ai_timer = rand() % 500;
    zc->dodging = Jfalse;
    zc->dodging_time = 0;
    return zc;
}


void Zombie_Update(Entity* z, World* world)
{

    Entity_CalculateVelocity(z);

    z->zombieC->ai_timer += delta_g;


    Zombie_Ai(z, world);




    ZombieC* zc = z->zombieC;
    if(!zc->idling)
    {
        Jbool collision = Entity_CollisionWithStuff(z, world);

        if(collision && !zc->aggressive)
        {
            Zombie_NewTrajectory(z);
        }
        Entity_CollisionWithExplosions(z, &world->explosions_vector);

        if(zc->aggressive &&
           (zc->zombie_type == Heavy_Zombie || zc->zombie_type == Trooper_Zombie))
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

	z->t = Cat_Zombie;
    z->x = x;
	z->y = y;
    z->zombieC = ZombieC_Create(type);
    z->zombieC->zombie_type = type;
    BoundingBox_Create(z, zombie_width_g[type], zombie_height_g[type]);
    z->angle = C_GenerateRandomAngle();
    z->texture = zombie_textures_g[type];
    z->speed = calm_speed_g[type];
    z->hp = zombie_hp_g[type];
    z->damage = zombie_damage_g[type];
    if(zombie_weapon_g[type] != No_Weapon)
    {
        z->weapons_component = WeaponsComponent_Create(Jtrue);
        WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                              Weapon_Create(zombie_weapon_g[type]));
    }


	return z;
}



void Zombie_Ai(Entity* z, World* world)
{
    ZombieC* zc = z->zombieC;
    Entity* player = &world->player;

    if(z->zombieC->ai_timer > 500)
    {
        if(Entity_CheckDistance(z, player, zc->vision_distance))
        {
            float angle_to_player = C_AngleBetween2Entities(z, player);

            //convert angle to 2pi circle and compare them
            float diff = C_DifferenceBetweenAngles(z->angle, angle_to_player);

            //check if player is in field of view, or if the player is too close
            if(
                (diff > -1.2f && diff < 1.2f)
                  ||

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
        }

        zc->ai_timer = 0;

        if(zc->dodging)
        {
            printf("%d\n", SDL_GetTicks() - zc->dodging_time);
        }
        if(zc->dodging && SDL_GetTicks() - zc->dodging_time > 700)
        {
            zc->dodging = Jfalse;
        }

        if(zc->aggressive)
        {
            if(!Entity_CheckDistance(z, player, 600))
            {
                Zombie_BecomeCalm(z);
            }

            if(!zc->dodging)
            {
                int dodge_chance = rand() % 100;

                //the zombie tries to dodge
                if(dodge_chance < 40)
                {
                    //convert angle in range -3.14;3.14 to range 0;6.28
                    float temp_angle = C_ConvertAngle2PiCirlce(C_AngleBetween2Points(z->x, z->y,
                                                                                     world->player.x,
                                                                                     world->player.y));
                    //we take one of the angle perpendicular to the one we found
                    int left_or_right = rand() % 2;

                    if(left_or_right == 0) temp_angle -= HALF_PI/3;
                    else temp_angle += HALF_PI/3;

                    z->angle = temp_angle;
                    //zombie will run in this direction for a little while
                    zc->dodging_time = SDL_GetTicks();
                    zc->dodging = Jtrue;
                }
                else
                {
                    z->angle = C_AngleBetween2Points(z->x, z->y, world->player.x, world->player.y);
                }
            }

        }
    }

    if(!zc->aggressive)
    {
        Zombie_CalculateRandomPath(z);
    }

}

void Zombie_BecomeCalm(Entity* z)
{
    z->zombieC->aggressive = Jfalse;
    z->speed = calm_speed_g[z->zombieC->zombie_type];
}

//when there's a collision, this generate a new angle in the opposite direction
void Zombie_NewTrajectory(Entity* z)
{
    z->angle = C_GenerateAngleInOppositeDirection(z->collision_direction);
}

void Zombie_CalculateRandomPath(Entity* z)
{
    z->zombieC->rand_move_timer += delta_g;
    if(z->zombieC->rand_move_timer >= z->zombieC->rand_move_every)
    {
        z->zombieC->idling = z->zombieC->idling ? Jfalse : Jtrue;
        z->zombieC->rand_move_timer = 0;
        z->zombieC->rand_move_every = (rand() % 13000);

        if(!z->zombieC->idling)
        {
            z->angle = C_GenerateRandomAngle();
        }
    }
}

void Zombie_BecomeAggressive(Entity* z, World* world)
{
    ZombieC* zc = z->zombieC;

    zc->aggressive = Jtrue;
    zc->idling = Jfalse;
    z->speed = angry_speed_g[zc->zombie_type];
    Vector* monsters = &world->monsters_vector;

    //the zombie tells the closest zombies that he's spotted the player
    for(int i = 0 ; i < Vector_Count(monsters) ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(monsters, i);
        if(Entity_CheckDistance(z, mob, 200) && !mob->zombieC->aggressive)
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

    if(random <= 4)
    {
        Bonus_GenerateRandom(bonus_vector, zombie);
    }

    Entity_Destroy(zombie);
}

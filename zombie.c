#include "math.h"
#include <stdio.h>
#include "bonus.h"
#include "zombie.h"
#include "entity.h"
#include "linkedList.h"
#include "world.h"
#include "weapon.h"
#include "movement_component.h"
#include "decals.h"

ZombieC* ZombieC_Create()
{
    ZombieC* zc = (ZombieC*)malloc(sizeof(ZombieC));

    zc->aggressive = false;
    zc->idling = false;
    zc->rand_move_every = 1;
    zc->rand_move_timer = 1;
    zc->vision_distance =360;
    zc->vision_width = 1.2;
    zc->ai_timer = rand() % 500;
    zc->dodging = false;
    zc->dodging_time = 0;
    //if the zombies calculates more than 2 paths between each AI update, he stops looking
    zc->paths_calculated = 0;
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
        bool collision = Entity_CollisionWithStuff(z, world);

        if(zc->paths_calculated < 3 &&
           collision && (!zc->aggressive || zc->dodging))
        {
            zc->paths_calculated++;
            Zombie_NewTrajectory(z);
        }
        Entity_CollisionWithExplosions(z, &world->explosions_vector);

        if(zc->aggressive &&
           (zc->zombie_type == Heavy_Zombie || zc->zombie_type == Trooper_Zombie))
        {
            Zombie_Shoot(z, world);
        }

        moveEntity(z, z->movementC->dx, z->movementC->dy);
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

    WeaponsComponent_TryToShoot(z->weaponsC, originX, originY,
                                angle, &world->bullets_vector, 0, 0);
}

Entity* CreateZombie(Zombie_Type type, float x, float y)
{
	Entity* z = Entity_Spawn();

	z->t                        = Cat_Zombie;
    z->x                        = x;
	z->y                        = y;
    z->texture                  = all_textures_g[Cat_Zombie][type];

    z->zombieC                  = ZombieC_Create(type);
    z->zombieC->zombie_type     = type;

    z->movementC                = MovementC_Create();
    z->movementC->speed         = calm_speed_g[type];
    z->movementC->angle         = C_GenerateRandomAngle();

    int width = 20;
    int height = 20;
    Weapon_Type weapon = No_Weapon;
    switch(type)
    {
    case Normal_Zombie:
        z->hp = 7;
        z->damage = 3;
        break;
    case Heavy_Zombie:
        width = 40;
        height = 40;
        z->hp = 20;
        z->damage = 10;
        weapon = Fireball_w;
        break;
    case Fast_Zombie:
        z->hp = 20;
        z->damage = 3;
        break;
    case Trooper_Zombie:
        width = 45;
        height = 45;
        z->hp = 20;
        z->damage = 10;
        weapon = TripleFireball_w;
       break;
    case Huge_Zombie:
        width = 100;
        height = 100;
        z->hp = 20;
        z->damage = 100;
        weapon = TripleFireball_w;
        break;
    }
    if(weapon != No_Weapon)
    {
        z->weaponsC = WeaponsComponent_Create(true);
        WeaponsComponent_AddWeaponToInventory(z->weaponsC,
                                              Weapon_Create(weapon));
    }

    BoundingBox_Create(z, width, height);

	return z;
}



void Zombie_Ai(Entity* z, World* world)
{
    ZombieC* zc = z->zombieC;
    Entity* player = &world->player;

    if(z->zombieC->ai_timer > 600)
    {
        zc->paths_calculated = 0;
        if(!zc->aggressive)
        {
            if(Entity_CheckNear(z, player))//600
            {
                float angle_to_player = C_AngleBetween2Entities(z, player);

                //convert angle to 2pi circle and compare them
                float diff = C_DifferenceBetweenAngles(z->movementC->angle, angle_to_player);

                //check if player is in field of view, or if the player is too close
                if((diff > -1.2f && diff < 1.2f)
                      || Entity_CheckDistance(z, player, 70))
                {
                    bool can_see = Entity_CheckCanSeeEntity(z, player, world);
                    if(can_see)
                    {
                        Zombie_BecomeAggressive(z, world);
                    }

                }
            }
        }



        zc->ai_timer = 0;


        if(zc->dodging && SDL_GetTicks() - zc->dodging_time > 500)
        {
            zc->dodging = false;
        }

        if(zc->aggressive)
        {
            if(!Entity_CheckNear(z, player))
            {
                Zombie_BecomeCalm(z);
            }

            if(!zc->dodging)
            {
                int dodge_chance = rand() % 100;


                //the zombie tries to dodge
                if(dodge_chance < 33)
                {
                    //convert angle in range -3.14;3.14 to range 0;6.28
                    float temp_angle = C_ConvertAngle2PiCirlce(C_AngleBetween2Points(z->x, z->y,
                                                                                     world->player.x,
                                                                                     world->player.y));

                    //generate new angle between pi/3 ;  pi/6
                    //and add it, or its opposite, to temp_angle
                    short sign = rand() % 2;

                    //is between pi/2 ;  half_pi/3 1.047 and + 1.047
                    float rand_angle = (float)(((rand() % 1047) + 523.0f) / 1000.0f);

                    if(sign == 0) temp_angle -= rand_angle;
                    else temp_angle += rand_angle;

                    z->movementC->angle = temp_angle;
                    //zombie will run in this direction for a little while
                    zc->dodging_time = SDL_GetTicks();
                    zc->dodging = true;
                }
                else
                {
                    z->movementC->angle = C_AngleBetween2Points(z->x, z->y,
                                                                world->player.x, world->player.y);
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
    z->zombieC->aggressive = false;
    z->movementC->speed = calm_speed_g[z->zombieC->zombie_type];
}

//when there's a collision, this generate a new angle in the opposite direction
void Zombie_NewTrajectory(Entity* z)
{
    z->movementC->angle = C_GenerateAngleInOppositeDirection(z->collision_direction);
}

void Zombie_CalculateRandomPath(Entity* z)
{
    z->zombieC->rand_move_timer += delta_g;
    if(z->zombieC->rand_move_timer >= z->zombieC->rand_move_every)
    {
        z->zombieC->idling = z->zombieC->idling ? false : true;
        z->zombieC->rand_move_timer = 0;
        z->zombieC->rand_move_every = (rand() % 13000);

        if(!z->zombieC->idling)
        {
            z->movementC->angle = C_GenerateRandomAngle();
        }
    }
}

void Zombie_BecomeAggressive(Entity* z, World* world)
{
    ZombieC* zc = z->zombieC;

    zc->aggressive = true;
    zc->idling = false;
    z->movementC->speed = angry_speed_g[zc->zombie_type];
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


void Zombie_Die(Entity* zombie, Vector* bonus_vector, Vector* decals_vector)
{
    int random = rand() % 10;

    if(random <= 4)
    {
        Bonus_GenerateRandom(bonus_vector, zombie);
    }


    Vector_Push(decals_vector, Decal_Create(zombie, Corpse, zombie->zombieC->zombie_type));

    Entity_Destroy(zombie);
}

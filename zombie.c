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

/*create a default zombie component*/
ZombieC* ZombieC_Create()
{
    ZombieC* zc = (ZombieC*)malloc(sizeof(ZombieC));

    zc->aggressive = false;
    zc->idling = false;
    zc->rand_move_every = 1;
    zc->rand_move_timer = 1;
    zc->vision_distance =300;
    zc->vision_width = 1.2;
    zc->ai_timer = rand() % 500;
    zc->dodging = false;
    zc->dodging_timer = 0;
    zc->spot_timer = 0;
    zc->paths_calculated = 0;
    zc->attack_delay = 500;
    zc->attack_timer = 0;
    return zc;
}



void Zombie_Update(Entity* z, World* world)
{
    if(!bullet_time_g)
    {
        Entity_CalculateVelocity(z);

        z->zombieC->ai_timer += delta_g;
        z->zombieC->rand_move_timer += delta_g;
        z->zombieC->attack_timer += delta_g;
        z->zombieC->dodging_timer += z->zombieC->ai_timer;
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

            moveEntity(z, z->movementC->dx, z->movementC->dy);
        }


        if(zc->aggressive &&
           z->weaponsC != NULL)
        {
            Zombie_Shoot(z, world);
        }
    }

}

void Zombie_GetAttacked(Entity* z, int damage, World* world)
{
    if(!z->zombieC->aggressive)
    {
        Zombie_BecomeAggressive(z, world);
    }

    Entity_LoseHealth(z, damage);
}

void  Zombie_Shoot(Entity* z, World* world)
{

    float originX = z->x + z->box.width/2;
    float originY = z->y + z->box.height/2;

    z->zombieC->shoot_timer += delta_g;
    ZombieC* zc = z->zombieC;

    if(z->sub_category != Zombie_Raptor)
    {
        float random_angle = (float)(rand() % 78 - 39) / 100;
        float angle = C_AngleBetween2Entities(z, &world->player) + random_angle;
        WeaponsComponent_TryToShoot(z->weaponsC, originX, originY,
                                angle, &world->bullets_vector, 0, 0);

    }
    else
    {

        if(zc->shoot_timer > zc->shoot_delay)
        {


            zc->pattern_timer += delta_g;

            if(zc->pattern_timer > zc->pattern_duration)
            {
                zc->pattern_timer = 0;
                zc->shoots_fired_in_pattern = 0;
            }

            if(zc->pattern_timer  == 0)
            {

                zc->pattern_direction_right = rand() % 2;
                float start_angle_difference = zc->pattern_direction_right? -1 : +1;
                zc->shooting_angle = C_AngleBetween2Entities(z, &world->player) - start_angle_difference;
            }
            else
            {
                zc->shooting_angle+= zc->pattern_direction_right? -0.2 : +0.2;
            }

            if(zc->shoots_fired_in_pattern < 16)
            {
                WeaponsComponent_TryToShoot(z->weaponsC, originX, originY,
                zc->shooting_angle, &world->bullets_vector, 0, 0);
                zc->shoots_fired_in_pattern++;
                zc->shoot_timer = 0;
            }

        }

    }
}

Entity* CreateZombie(Zombie_Type type, float x, float y)
{
	Entity* z = Entity_Spawn();

	z->t                                = Cat_Zombie;
    z->x                                = x;
	z->y                                = y;
	z->sub_category                     = type;

    z->zombieC                          = ZombieC_Create(type);
    z->zombieC->zombie_type             = type;
    z->zombieC->reaction_time           = 1000;
    z->zombieC->can_dodge_every         = 1000;

    z->movementC                        = MovementC_Create();
    z->angle                            = C_GenerateRandomAngle();
    z->zombieC->shoot_timer             = 0;
    z->zombieC->shoots_fired_in_pattern = 0;
    z->zombieC->pattern_direction_right = false;

    int width = 20;
    int height = 20;
    Weapon_Type weapon = No_Weapon;

    switch(type)
    {
    case Zombie_Normal:
        z->hp = 4;
        z->damage = 3;
        z->movementC->normal_speed =0.03;
        z->movementC->running_speed = 0.18;
        break;
    case Zombie_Heavy:
        width = 40;
        height = 40;
        z->hp = 30;
        z->damage = 2;
        weapon = Weapon_Fireball;
        z->movementC->normal_speed = 0.05;
        z->movementC->running_speed = 0.05;
        break;
    case Zombie_Slow:
        z->hp = 2;
        z->damage = 1;
        z->movementC->normal_speed = 0.02;
        z->movementC->running_speed = 0.03;
        break;
    case Zombie_Destroyer:
        width = 200;
        height = 200;
        z->hp = 500;
        z->damage = 500;
        z->movementC->normal_speed = 0.02;
        z->movementC->running_speed = 0.12;
        z->zombieC->attack_delay = 50;
        break;
    case Zombie_Raptor:
        width = 80;
        height = 80;
        z->hp = 100;
        z->damage = 15;
        weapon = Weapon_FireballMachineGun;
        z->movementC->normal_speed = 0.02;
        z->movementC->running_speed = 0.02;
        z->zombieC->pattern_duration = 1000;
        z->zombieC->pattern_timer = 0;
        z->zombieC->shooting_angle = 0;
        z->zombieC->shoot_delay = 120;
        break;
    case Zombie_Fast:
        z->hp = 4;
        z->damage = 3;
        z->movementC->normal_speed = 0.05;
        z->movementC->running_speed = 0.3;
        break;
    case Zombie_Trooper:
        width = 45;
        height = 45;
        z->hp = 40;
        z->damage = 5;
        weapon = Weapon_TripleFireball;
        z->movementC->normal_speed = 0.05;
        z->movementC->running_speed = 0.1;
       break;
    case Zombie_Huge:
        width = 120;
        height = 120;
        z->hp = 250;
        z->damage = 15;
        weapon = Weapon_TripleFireball;
        z->movementC->normal_speed = 0.05;
        z->movementC->running_speed = 0.1;
        break;
    }

    z->movementC->speed = z->movementC->normal_speed;

    if(weapon != No_Weapon)
    {
        z->weaponsC = WeaponsComponent_Create(true);
        WeaponsComponent_AddWeaponToInventory(z->weaponsC,
                                              Weapon_Create(weapon));
        WeaponsComponent_AddAmmo(z->weaponsC, weapon, 500);
        WeaponsComponent_ChangeWeapon(z->weaponsC, weapon);
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
            if(Entity_CheckDistance(z, player, z->zombieC->vision_distance))//600
            {
                float angle_to_player = C_AngleBetween2Entities(z, player);

                /*convert angle to [0;2pi] format and compare them*/
                float diff = C_DifferenceBetweenAngles(z->angle, angle_to_player);

                /*check if player is in field of view, or if the player is too close*/
                if((diff > -1.2f && diff < 1.2f) ||
                    Entity_CheckDistance(z, player, 70))
                {
                    bool can_see = Entity_CheckCanSeeEntity(z, player, world);
                    if(can_see)
                    {
                        z->zombieC->spot_timer += z->zombieC->ai_timer;
                        if(z->zombieC->spot_timer >= z->zombieC->reaction_time)
                        {
                            Zombie_BecomeAggressive(z, world);
                        }

                    }

                }
            }
        }



        zc->ai_timer = 0;


        if(zc->dodging && SDL_GetTicks() - zc->dodging_timer > 500)
        {
            zc->dodging = false;
        }

        if(zc->aggressive)
        {

            if(!Entity_CheckDistance(z, player, 3500))
            {

                Zombie_BecomeCalm(z);
            }

            /*calculate the dodge chance*/
            if(!zc->dodging && z->sub_category != Zombie_Destroyer &&  SDL_GetTicks() - zc->dodging_timer > 1500)
            {
                int dodge_chance = rand() % 100;

                if(dodge_chance < 35)
                {
                    /*convert angle in range [-pi;pi] to range [0;2pi]*/
                    float temp_angle = C_ConvertAngle2PiCirlce(C_AngleBetween2Points(z->x, z->y,
                                                                                     world->player.x,
                                                                                     world->player.y));

                    /*generate new angle in range [pi/3;pi/6]
                    and add it, or its opposite, to temp_angle*/
                    float rand_angle = (float)(((rand() % 1047) + 523.0f) / 1000.0f);

                    short sign = rand() % 2;
                    if(sign == 0) temp_angle -= rand_angle;
                    else temp_angle += rand_angle;

                    z->angle = temp_angle;
                    zc->dodging_timer = SDL_GetTicks();
                    zc->dodging = true;
                }

            }

            if(!zc->dodging)
            {
                z->angle = C_AngleBetween2Points(z->x, z->y,
                                                            world->player.x, world->player.y);
            }

        }

        if(!zc->aggressive)
        {
            Zombie_CalculateRandomPath(z);
        }

    }




}

void Zombie_BecomeCalm(Entity* z)
{
    z->zombieC->aggressive = false;
    z->movementC->speed = z->movementC->normal_speed;
    z->zombieC->spot_timer = 0;
}

/*generated a new angle in the opposite direction of the last collision*/
void Zombie_NewTrajectory(Entity* z)
{
    z->angle = C_GenerateAngleInOppositeDirection(z->collision_direction);
}

/*generates a random angle and movement duration*/
void Zombie_CalculateRandomPath(Entity* z)
{
    if(z->zombieC->rand_move_timer >= z->zombieC->rand_move_every)
    {
        z->zombieC->idling = z->zombieC->idling ? false : true;
        z->zombieC->rand_move_timer = 0;
        z->zombieC->rand_move_every = (rand() % 13000);

        if(!z->zombieC->idling)
        {
            z->angle = C_GenerateRandomAngle();
        }
    }
}

/*zombie becomes aggressive and tells the closest zombies that he's spotted the player*/
void Zombie_BecomeAggressive(Entity* z, World* world)
{
    ZombieC* zc = z->zombieC;

    zc->aggressive = true;
    zc->idling = false;
    z->movementC->speed = z->movementC->running_speed;
    Vector* monsters = &world->monsters_vector;

    unsigned char zombies_told = 0;


    for(int i = 0 ; i < Vector_Count(monsters) && zombies_told < 3 ; i++)
    {
        Entity* mob = (Entity*)Vector_Get(monsters, i);
        if(Entity_CheckDistance(z, mob, 300) && !mob->zombieC->aggressive)
        {
            if(Entity_CheckCanSeeEntity(z, mob, world))
            {
                Zombie_BecomeAggressive(mob, world);
                zombies_told++;
            }


        }
    }


}

/*generate corpse and item drops*/
void Zombie_Die(Entity* zombie, Vector* bonus_vector, Vector* decals_vector)
{
    int random = rand() % 100;

    if(random <= 10)
    {
        Bonus_GenerateRandom(bonus_vector, zombie);
    }

    Decal_Type corpse_type = 0;

    switch(zombie->sub_category)
    {
    case Zombie_Normal:
        corpse_type = Decal_Corpse_Normal;
        break;
    case Zombie_Fast:
        corpse_type = Decal_Corpse_Fast;
        break;
    case Zombie_Heavy:
        corpse_type = Decal_Corpse_Heavy;
        break;
    case Zombie_Trooper:
        corpse_type = Decal_Corpse_Trooper;
        break;
    case Zombie_Huge:
        corpse_type = Decal_Corpse_Huge;
        break;
    case Zombie_Slow:
        corpse_type = Decal_Corpse_Slow;
        break;
    case Zombie_Raptor:
        corpse_type = Decal_Corpse_Raptor;
        break;
    case Zombie_Destroyer:
        corpse_type = Decal_Corpse_Destroyer;
        break;
    }
    Vector_Push(decals_vector, Decal_Create(corpse_type, zombie->x, zombie->y, zombie->angle));
}

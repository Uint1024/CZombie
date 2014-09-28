#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "linkedList.h"
#include "vector.h"

typedef struct Entity Entity;
typedef struct World World;

typedef struct ZombieC
{
    Zombie_Type             zombie_type;
    int                     vision_distance;
    float                   vision_width;
    bool                    aggressive;
    int                     rand_move_timer;
    int                     rand_move_every;
    bool                    idling;
    bool                    dodging;
    int                     can_dodge_every; //can dodge every x ms
    int                     dodging_timer;
    //the zombies ai activates every every 700 ms,
    //else it lags too much...
    int                     ai_timer;
    int                     paths_calculated;
    int attack_timer;//close combat
    int attack_delay;
    int shoot_delay;
    int shoot_timer;
    float shooting_angle;
    int pattern_duration;
    int pattern_timer;
    int shoots_fired_in_pattern;
    bool pattern_direction_right;

    //there's a delay between the time the zombie sees the player
    //and when it starts chasing him
    int spot_timer;//nb of ms the zombie has seen the player
    int reaction_time;
} ZombieC;

ZombieC* ZombieC_Create();

void Zombie_Update(Entity* z, World* world);

Entity* CreateZombie(Zombie_Type type, float x, float y);
void Zombie_Die(Entity* zombie, Vector* bonus_vector, Vector* decals_vector);
void  Zombie_Shoot(Entity* z, World* world);
void Zombie_Ai(Entity* z, World* world);
void Zombie_BecomeAggressive(Entity* z, World* world);
void Zombie_BecomeCalm(Entity* z);
void Zombie_CalculateRandomPath(Entity *z);
void Zombie_NewTrajectory(Entity* z);
void Zombie_GetAttacked(Entity* z, int damage, World* world);
#endif

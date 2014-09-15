#ifndef WEAPON_H
#define WEAPON_H
#include "common.h"
#include "weapons_component.h"
#include "linkedList.h"
#include "vector.h"
typedef struct Weapon
{
    char                *name;
    Weapon_Type         type;
    Bonus_type          corresponding_pickup;
    int                 magazine_max_bullets;
    int                 magazine_bullets;
    int                 last_shot;
    int                 delay_between_shots;
    WeaponsComponent*   parent;
    int                 reloading_time;
} Weapon;

Weapon* Weapon_Create(Weapon_Type type);
/*void Weapon_TryToShoot(Weapon* weapon, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY, int delta);*/
#endif // WEAPON_H

#ifndef WEAPON_H
#define WEAPON_H
#include "common.h"
#include "weapons_component.h"
#include "linkedList.h"

typedef struct Weapon
{
    Weapon_Type type;
    int magazine_max_bullets;
    int magazine_bullets;
    int last_shot;
    int delay_between_shots;
    WeaponsComponent* parent;
    int reloading_time;
} Weapon;

Weapon* Weapon_Create(Weapon_Type type);
void Weapon_TryToShoot(Weapon* weapon, float originX, float originY, float angle, List* bullets);
#endif // WEAPON_H

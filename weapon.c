#include <stdlib.h>
#include <SDL.h>
#include "linkedList.h"
#include "weapon.h"
#include "bullet.h"
#include "vector.h"
#include "explosive.h"

Weapon* Weapon_Create(Weapon_Type type)
{
    Weapon* w = (Weapon*)malloc(sizeof(Weapon));
    w->type = type;

    if(type == AutomaticRifle_w)
    {
        w->name = "Automatic Rifle";
        w->magazine_max_bullets = 50;
        w->magazine_bullets = 50;
        w->delay_between_shots = 35;
        w->reloading_time = 3000;
        w->corresponding_pickup = Rifle_bonus;
    }
    else if(type == Handgun_w)
    {
        w->name = "Handgun";
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->delay_between_shots = 200;
        w->reloading_time = 1200;
        //w->corresponding_pickup = ;
    }
    else if(type == Shotgun_w)
    {
        w->name = "Shotgun";
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->delay_between_shots = 450;
        w->reloading_time = 2000;
        w->corresponding_pickup = Shotgun_bonus;
    }
    else if(type == GrenadeLauncher_w)
    {
        w->name = "Grenade Launcher";
        w->magazine_max_bullets = 5;
        w->magazine_bullets = 5;
        w->delay_between_shots = 500;
        w->reloading_time = 6000;
        w->corresponding_pickup = GrenadeLauncher_bonus;
    }
    else if(type == Fireball_w)
    {
        w->name = "Fireball";
        w->magazine_max_bullets = 1;
        w->magazine_bullets = 1;
        w->delay_between_shots = 1500;
        w->reloading_time = 0;
        //w->corresponding_pickup = GrenadeLauncher_bonus;
    }

    w->last_shot = 0;

    return w;
}

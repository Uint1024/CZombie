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

    if(type == Weapon_AutomaticRifle)
    {
        w->name = "Automatic Rifle";
        w->magazine_max_bullets = 50;
        w->magazine_bullets = 50;
        w->delay_between_shots = 35;
        w->reloading_time = 3000;
        w->corresponding_pickup = Bonus_Rifle;
    }
    else if(type == Weapon_Handgun)
    {
        w->name = "Handgun";
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->delay_between_shots = 200;
        w->reloading_time = 1200;
        //w->corresponding_pickup = ;
    }
    else if(type == Weapon_Shotgun)
    {
        w->name = "Shotgun";
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->delay_between_shots = 450;
        w->reloading_time = 2000;
        w->corresponding_pickup = Bonus_Shotgun;
    }
    else if(type == Weapon_GrenadeLauncher)
    {
        w->name = "Grenade Launcher";
        w->magazine_max_bullets = 5;
        w->magazine_bullets = 5;
        w->delay_between_shots = 500;
        w->reloading_time = 6000;
        w->corresponding_pickup = Bonus_GrenadeLauncher;
    }
    else if(type == Weapon_TheBigGun)
    {
        w->name = "The Big Gun";
        w->magazine_max_bullets = 200;
        w->magazine_bullets = 200;
        w->delay_between_shots = 35;
        w->reloading_time = 600;
        w->corresponding_pickup = Bonus_TheBigGun;
    }
    else if(type == Weapon_Fireball)
    {
        w->name = "Fireball";
        w->magazine_max_bullets = 1;
        w->magazine_bullets = 1;
        w->delay_between_shots = 500;
        w->reloading_time = 0;
        //w->corresponding_pickup = Bonus_GrenadeLauncher;
    }
    else if(type == Weapon_TripleFireball)
    {
        w->name = "Triple Fireball";
        w->magazine_max_bullets = 1;
        w->magazine_bullets = 1;
        w->delay_between_shots = 2000;
        w->reloading_time = 0;
        //w->corresponding_pickup = Bonus_GrenadeLauncher;
    }

    w->last_shot = 0;

    return w;
}

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


    w->bullet_width = 14;
    w->bullet_height = 14;

    if(type == Weapon_AutomaticRifle)
    {
        w->name = "Automatic Rifle";
        w->magazine_max_bullets = 50;
        w->magazine_bullets = 50;
        w->delay_between_shots = 65;
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
        w->bullet_width = 20;
        w->bullet_height = 20;
    }
    else if(type == Weapon_Shotgun)
    {
        w->name = "Shotgun";
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->delay_between_shots = 1000;
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
        w->delay_between_shots = 85;
        w->reloading_time = 600;
        w->corresponding_pickup = Bonus_TheBigGun;
        w->bullet_width = 28;
        w->bullet_height = 28;
    }
    else if(type == Weapon_Fireball)
    {
        w->name = "Fireball";
        w->magazine_max_bullets = 1;
        w->magazine_bullets = 1;
        w->delay_between_shots = 500;
        w->reloading_time = 0;
        w->bullet_width = 15;
        w->bullet_height = 15;
        //w->corresponding_pickup = Bonus_GrenadeLauncher;
    }
    else if(type == Weapon_TripleFireball)
    {
        w->name = "Triple Fireball";
        w->magazine_max_bullets = 1;
        w->magazine_bullets = 1;
        w->delay_between_shots = 2000;
        w->reloading_time = 0;
        w->bullet_width = 15;
        w->bullet_height = 15;
        //w->corresponding_pickup = Bonus_GrenadeLauncher;
    }

    w->last_shot = 0;

    return w;
}

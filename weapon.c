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

    if(type == AutomaticRifle_w)
    {
        w->name = "Automatic Rifle";
        w->type = AutomaticRifle_w;
        w->magazine_max_bullets = 50;
        w->magazine_bullets = 50;
        w->last_shot = 0;
        w->delay_between_shots = 35;
        w->reloading_time = 1000;
    }
    else if(type == Handgun_w)
    {
        w->name = "Handgun";
        w->type = Handgun_w;
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->last_shot = 0;
        w->delay_between_shots = 200;
        w->reloading_time = 700;
    }
    else if(type == Shotgun_w)
    {
        w->name = "Shotgun";
        w->type = Shotgun_w;
        w->magazine_max_bullets = 15;
        w->magazine_bullets = 15;
        w->last_shot = 0;
        w->delay_between_shots = 450;
        w->reloading_time = 1500;
    }
    else if(type == GrenadeLauncher_w)
    {
        w->name = "Grenade Launcher";
        w->type = GrenadeLauncher_w;
        w->magazine_max_bullets = 5;
        w->magazine_bullets = 5;
        w->last_shot = 0;
        w->delay_between_shots = 500;
        w->reloading_time = 2500;
    }
    return w;
}
void Weapon_TryToShoot(Weapon* weapon, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY, int delta)
{
    if(weapon->magazine_bullets > 0 &&
        SDL_GetTicks() - weapon->last_shot > weapon->delay_between_shots)
    {
        if(weapon->type == AutomaticRifle_w || weapon->type == Handgun_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle, 1));
            weapon->magazine_bullets -= 1;
            weapon->last_shot = SDL_GetTicks();
        }
        else if(weapon->type == Shotgun_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle, 1));
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle+0.05, 1));
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle+0.1, 1));
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle-0.1, 1));
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle-0.05, 1));
            weapon->magazine_bullets -= 1;
            weapon->last_shot = SDL_GetTicks();
        }
        else if(weapon->type == GrenadeLauncher_w)
        {
            Vector_Push(bullets_vector, Grenade_Create(originX, originY, angle, 1, destinationX, destinationY));
            weapon->last_shot = SDL_GetTicks();
        }
    }
    else if(weapon->magazine_bullets <= 0)
    {
        WeaponsComponent_Reload(weapon->parent, delta);
    }
}

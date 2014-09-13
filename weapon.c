#include <stdlib.h>
#include <SDL.h>
#include "linkedList.h"
#include "weapon.h"
#include "bullet.h"
#include "vector.h"

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
        w->delay_between_shots = 300;
        w->reloading_time = 700;
    }
    return w;
}
void Weapon_TryToShoot(Weapon* weapon, float originX, float originY, float angle, Vector* bullets_vector)
{
    if(weapon->magazine_bullets > 0 &&
        SDL_GetTicks() - weapon->last_shot > weapon->delay_between_shots)
    {
        //List_push(bullets, Bullet_Create(originX,  originY, angle, 1));
        Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle, 1));
        weapon->magazine_bullets -= 1;
        weapon->last_shot = SDL_GetTicks();
    }
    else if(weapon->magazine_bullets <= 0)
    {
        WeaponsComponent_Reload(weapon->parent);
    }
}

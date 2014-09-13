#include <stdlib.h>
#include <SDL.h>
#include "linkedList.h"
#include "weapon.h"
#include "bullet.h"

Weapon* Weapon_Create(Weapon_Type type)
{
    Weapon* w = (Weapon*)malloc(sizeof(Weapon));

    if(type == AutomaticRifle_w)
    {
        w->type = AutomaticRifle_w;
        w->magazine_max_bullets = 50;
        w->magazine_bullets = 50;
        w->last_shot = 0;
        w->delay_between_shots = 35;
        w->reloading_time = 1000;
    }

    return w;
}
void Weapon_TryToShoot(Weapon* weapon, float originX, float originY, float angle, List* bullets)
{
    if(weapon->magazine_bullets > 0 &&
        SDL_GetTicks() - weapon->last_shot > weapon->delay_between_shots)
    {
        List_push(bullets, Bullet_Create(originX,  originY, angle, 1));
        weapon->magazine_bullets -= 1;
        weapon->last_shot = SDL_GetTicks();
    }
    else if(weapon->magazine_bullets <= 0)
    {
        WeaponsComponent_Reload(weapon->parent);
    }
}

#include "stdlib.h"
#include <SDL.h>

#include "bullet.h"

#include "weapon.h"
#include "weapons_component.h"

WeaponsComponent* WeaponsComponent_Create(Jbool is_monster)
{
    WeaponsComponent* wc = (WeaponsComponent*)malloc(sizeof(WeaponsComponent));

    for(int i = 0 ; i < NB_OF_WEAPONS ; i++)
    {
        wc->bullets[i] = 0;
        wc->weapons_inventory[i] = NULL;
    }

    wc->current_weapon = NULL;
    wc->last_reload = 0;
    wc->reloading = Jfalse;
    wc->is_monster = is_monster;

    return wc;
}

void WeaponsComponent_AddWeaponToInventory(WeaponsComponent* wc, Weapon* weapon)
{
    weapon->parent = wc;
    wc->weapons_inventory[weapon->type] = weapon;

    if(wc->current_weapon == NULL)
    {
        wc->current_weapon = weapon;
    }
}

void WeaponsComponent_ChangeWeapon(WeaponsComponent* wc, Weapon_Type type)
{
    if(wc->current_weapon != NULL)
    {
        wc->reloading = Jfalse;
        wc->reload_timer = wc->current_weapon->reloading_time;
    }
    wc->current_weapon = wc->weapons_inventory[type];
}

void WeaponsComponent_AddAmmo(WeaponsComponent* wc, Weapon_Type type, int quantity)
{
    wc->bullets[type] += quantity;
}

void WeaponsComponent_Reload(WeaponsComponent* wc)
{
    if(wc->current_weapon->magazine_bullets < wc->current_weapon->magazine_max_bullets)
    {
        if(!wc->reloading &&  wc->bullets[wc->current_weapon->type] > 0)
        {
            wc->reloading = Jtrue;
            wc->reload_timer = wc->current_weapon->reloading_time;
        }
        else if(wc->reloading && wc->bullets[wc->current_weapon->type] > 0)
        {
            wc->reload_timer -= delta_g;
        }

        if(wc->reloading && wc->reload_timer <= 0)
        {
            int bullets_to_reload = wc->current_weapon->magazine_max_bullets -
                            wc->current_weapon->magazine_bullets;

            if(wc->bullets[wc->current_weapon->type] - bullets_to_reload >= 0)
            {
                wc->bullets[wc->current_weapon->type] -= bullets_to_reload;
            }
            else
            {
                bullets_to_reload = wc->bullets[wc->current_weapon->type];
                wc->bullets[wc->current_weapon->type] = 0;
            }
            wc->current_weapon->magazine_bullets += bullets_to_reload;

            wc->reloading = Jfalse;
        }
    }
}

void WeaponsComponent_ScrollWeapons(WeaponsComponent* wc, int wheel_direction)
{
   Jbool found_weapon = Jfalse;
    Weapon_Type current_type = wc->current_weapon->type;
    Weapon_Type next_type = current_type;


    while(!found_weapon)
    {
        next_type += (wheel_direction == 1) ? 1 : -1;
        if(next_type < NB_OF_WEAPONS)
        {
            if(wc->weapons_inventory[next_type] != NULL)
            {
                WeaponsComponent_ChangeWeapon(wc, next_type);//wc->current_weapon = wc->weapons_inventory[next_type];
                found_weapon = Jtrue;
            }
        }
        else
        {
            next_type = (wheel_direction == 1) ? -1 : NB_OF_WEAPONS;
        }
    }
}

void WeaponsComponent_TryToShoot(WeaponsComponent* wc, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY)
{

    Weapon* weapon = wc->current_weapon;

    if(weapon->magazine_bullets > 0 &&
        SDL_GetTicks() - weapon->last_shot > weapon->delay_between_shots)
    {
        if(weapon->type == AutomaticRifle_w || weapon->type == Handgun_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, 1, wc->is_monster));
        }
        else if(weapon->type == Shotgun_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, 1, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.05, 1, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.1, 1, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.1, 1, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.05, 1, wc->is_monster));
        }
        else if(weapon->type == GrenadeLauncher_w)
        {
            Vector_Push(bullets_vector, Grenade_Create(originX, originY, angle, 1, destinationX, destinationY));
        }
        else if(weapon->type == Fireball_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, 0.4, wc->is_monster));
        }
        else if(weapon->type == TripleFireball_w)
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.1, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.2, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.3, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.1, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.2, 0.4, wc->is_monster));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.3, 0.4, wc->is_monster));
        }

        weapon->magazine_bullets -= wc->is_monster ? 0 : 1;
        weapon->last_shot = SDL_GetTicks();
    }
    else if(weapon->magazine_bullets <= 0)
    {
        WeaponsComponent_Reload(wc);
    }

}

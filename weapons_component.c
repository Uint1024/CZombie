#include "stdlib.h"
#include <SDL.h>

#include "bullet.h"

#include "weapon.h"
#include "weapons_component.h"

WeaponsC* WeaponsComponent_Create(bool is_monster)
{
    WeaponsC* wc = (WeaponsC*)malloc(sizeof(WeaponsC));

    for(int i = 0 ; i < NB_OF_WEAPONS ; i++)
    {
        wc->bullets[i] = 0;
        wc->weapons_inventory[i] = NULL;
    }

    wc->current_weapon = NULL;
    wc->last_reload = 0;
    wc->reloading = false;
    wc->is_monster = is_monster;

    return wc;
}

void WeaponsComponent_AddWeaponToInventory(WeaponsC* wc, Weapon* weapon)
{
    weapon->parent = wc;
    wc->weapons_inventory[weapon->type] = weapon;

    if(wc->current_weapon == NULL)
    {
        wc->current_weapon = weapon;
    }
}

void WeaponsComponent_ChangeWeapon(WeaponsC* wc, Weapon_Type type)
{
    if(wc->current_weapon != NULL)
    {
        wc->reloading = false;
        wc->reload_timer = wc->current_weapon->reloading_time;
    }
    if(wc->weapons_inventory[type] != NULL)
    {
        wc->current_weapon = wc->weapons_inventory[type];
    }

}

void WeaponsComponent_AddAmmo(WeaponsC* wc, Weapon_Type type, int quantity)
{
    wc->bullets[type] += quantity;
}

void WeaponsComponent_Reload(WeaponsC* wc)
{
    if(wc->current_weapon->magazine_bullets < wc->current_weapon->magazine_max_bullets)
    {
        if(!wc->reloading &&  wc->bullets[wc->current_weapon->type] > 0)
        {
            wc->reloading = true;
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

            wc->reloading = false;
        }
    }
}

void WeaponsComponent_ScrollWeapons(WeaponsC* wc, int wheel_direction)
{
   bool found_weapon = false;
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
                found_weapon = true;
            }
        }
        else
        {
            next_type = (wheel_direction == 1) ? -1 : NB_OF_WEAPONS;
        }
    }
}

void WeaponsComponent_TryToShoot(WeaponsC* wc, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY)
{


    Weapon* weapon = wc->current_weapon;

    if(wc->bullets[weapon->type] > 0 && weapon->magazine_bullets > 0 &&
        SDL_GetTicks() - weapon->last_shot > weapon->delay_between_shots)
    {


        if(weapon->type == Weapon_Shotgun)
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.07, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.14, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.21, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.28, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.32, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.36, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.40, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.40, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.36, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.32, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.28, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.21, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.14, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.07, weapon->bullet_speed, wc->is_monster, weapon));

        }
        else if(weapon->type == Weapon_GrenadeLauncher)
        {
            Vector_Push(bullets_vector, Grenade_Create(originX, originY, angle, weapon->bullet_speed, destinationX, destinationY));
        }
        else if(weapon->type == Weapon_TripleFireball)
        {

            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.1, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.2, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle+0.3, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.1, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.2, weapon->bullet_speed, wc->is_monster, weapon));
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle-0.3, weapon->bullet_speed, wc->is_monster, weapon));
        }
        else
        {
            Vector_Push(bullets_vector, Bullet_Create(weapon->type, originX,  originY, angle, weapon->bullet_speed, wc->is_monster, weapon));
        }

        Sound_PlayShot(weapon->type);

        if(!unlimited_ammo_g && reloading_g)
        {
            weapon->magazine_bullets -= wc->is_monster ? 0 : 1;
        }
        else if(!reloading_g)
        {
            wc->bullets[weapon->type]--;
        }

        weapon->last_shot = SDL_GetTicks();
    }
    else if(weapon->magazine_bullets <= 0 && reloading_g)
    {
        WeaponsComponent_Reload(wc);
    }

}

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
    wc->recoil = 0;
    return wc;
}

void WeaponsComponent_AddWeaponToInventory(WeaponsC* wc, Weapon* weapon)
{
    weapon->is_monster = wc->is_monster;
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
            for(float i = -0.3 ; i <= 0.3 ; i += 0.05)
            {
                float random_pattern = (float)(rand() % 200 - 100)/1000.f;
                Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle + i + random_pattern, weapon));
            }
        }
        else if(weapon->type == Weapon_GrenadeLauncher)
        {
            Vector_Push(bullets_vector, Grenade_Create(originX, originY, angle, 1, destinationX, destinationY));
        }
        else if(weapon->type == Weapon_TripleFireball)
        {
            for(float i = -0.3 ; i <= 0.3 ; i += 0.1)
            {
                Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle + i, weapon));
            }
        }
        else
        {
            float recoil = 0;
            if(wc->recoil > 0)
                recoil = (float)(rand() % (2 * (int)wc->recoil) - wc->recoil)/1000.f;

            printf("%f recoil\n", recoil);
            Vector_Push(bullets_vector, Bullet_Create(originX,  originY, angle + recoil, weapon));
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

        wc->recoil += 5.0f;

        if(wc->recoil > 150.0f)
        {
            wc->recoil = 150.0f;
        }
    }
    else if(weapon->magazine_bullets <= 0 && reloading_g)
    {
        WeaponsComponent_Reload(wc);
    }

}

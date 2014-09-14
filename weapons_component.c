#include "stdlib.h"

#include "weapon.h"
#include "weapons_component.h"

WeaponsComponent* WeaponsComponent_Create()
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

    return wc;
}

void WeaponsComponent_AddWeaponToInventory(WeaponsComponent* wc, Weapon* weapon)
{
    weapon->parent = wc;
    wc->weapons_inventory[weapon->type] = weapon;
}

void WeaponsComponent_ChangeWeapon(WeaponsComponent* wc, Weapon_Type type)
{
    wc->current_weapon = wc->weapons_inventory[type];
}

void WeaponsComponent_AddAmmo(WeaponsComponent* wc, Weapon_Type type, int quantity)
{
    wc->bullets[type] += quantity;
}

void WeaponsComponent_Reload(WeaponsComponent* wc, int delta)
{
    if(!wc->reloading &&  wc->bullets[wc->current_weapon->type] > 0)
    {
        wc->reloading = Jtrue;
        wc->reload_timer = wc->current_weapon->reloading_time;
    }
    else if(wc->reloading && wc->bullets[wc->current_weapon->type] > 0)
    {
        wc->reload_timer -= delta;
    }

    if(wc->reloading && wc->reload_timer <= 0)
    {
        wc->current_weapon->magazine_bullets = wc->current_weapon->magazine_max_bullets;
        wc->bullets[wc->current_weapon->type] -= wc->current_weapon->magazine_max_bullets;
        wc->reloading = Jfalse;
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
                wc->current_weapon = wc->weapons_inventory[next_type];
                found_weapon = Jtrue;
            }

        }
        else
        {
            next_type = (wheel_direction == 1) ? -1 : NB_OF_WEAPONS;
        }
    }

}

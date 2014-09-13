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

void WeaponsComponent_Reload(WeaponsComponent* wc)
{
    if(!wc->reloading &&  wc->bullets[wc->current_weapon->type] > 0)
    {
        wc->reloading = Jtrue;
        wc->last_reload = SDL_GetTicks();
    }
    else if(wc->reloading && wc->bullets[wc->current_weapon->type] > 0 &&
            SDL_GetTicks() - wc->last_reload >= wc->current_weapon->reloading_time)
    {
        wc->current_weapon->magazine_bullets = wc->current_weapon->magazine_max_bullets;
        wc->bullets[wc->current_weapon->type] -= wc->current_weapon->magazine_max_bullets;
        wc->reloading = Jfalse;
    }

}

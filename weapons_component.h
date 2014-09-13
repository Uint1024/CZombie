
#ifndef WEAPONS_COMPONENT_H
#define WEAPONS_COMPONENT_H
typedef struct Weapon Weapon;

typedef struct WeaponsComponent
{
    Weapon* current_weapon;
    Weapon* weapons_inventory[NB_OF_WEAPONS];
    int last_reload;
	Jbool reloading;
    int bullets[NB_OF_WEAPONS];
} WeaponsComponent;

WeaponsComponent* WeaponsComponent_Create();
void WeaponsComponent_AddWeaponToInventory(WeaponsComponent* wc, Weapon* weapon);
void WeaponsComponent_AddAmmo(WeaponsComponent* wc, Weapon_Type type, int quantity);
void WeaponsComponent_ChangeWeapon(WeaponsComponent* wc, Weapon_Type type);
void WeaponsComponent_Reload(WeaponsComponent* wc);
#endif // WEAPON_H

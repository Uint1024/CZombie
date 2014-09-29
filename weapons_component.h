
#ifndef WEAPONS_COMPONENT_H
#define WEAPONS_COMPONENT_H
typedef struct Weapon Weapon;
typedef struct Vector Vector;

typedef struct WeaponsC
{
    Weapon* current_weapon;
    Weapon* weapons_inventory[NB_OF_WEAPONS];
    int last_reload;
	bool reloading;
    int bullets[NB_OF_WEAPONS];
    int reload_timer;

    //monsters have infinite bullets
    //their bullets can't harm other mobs
    bool is_monster;

} WeaponsC;

WeaponsC* WeaponsComponent_Create(bool is_monster);
void WeaponsComponent_AddWeaponToInventory(WeaponsC* wc, Weapon* weapon);
void WeaponsComponent_AddAmmo(WeaponsC* wc, Weapon_Type type, int quantity);
void WeaponsComponent_ChangeWeapon(WeaponsC* wc, Weapon_Type type);
void WeaponsComponent_Reload(WeaponsC* wc);
void WeaponsComponent_ScrollWeapons(WeaponsC* wc, int wheel_direction);
void WeaponsComponent_TryToShoot(WeaponsC* wc, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY);
#endif // WEAPON_H

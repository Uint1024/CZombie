
#ifndef WEAPONS_COMPONENT_H
#define WEAPONS_COMPONENT_H
typedef struct Weapon Weapon;
typedef struct Vector Vector;

typedef struct WeaponsComponent
{
    Weapon* current_weapon;
    Weapon* weapons_inventory[NB_OF_WEAPONS];
    int last_reload;
	Jbool reloading;
    int bullets[NB_OF_WEAPONS];
    int reload_timer;

    //monsters have infinite bullets
    //their bullets can't harm other mobs
    Jbool is_monster;
} WeaponsComponent;

WeaponsComponent* WeaponsComponent_Create();
void WeaponsComponent_AddWeaponToInventory(WeaponsComponent* wc, Weapon* weapon);
void WeaponsComponent_AddAmmo(WeaponsComponent* wc, Weapon_Type type, int quantity);
void WeaponsComponent_ChangeWeapon(WeaponsComponent* wc, Weapon_Type type);
void WeaponsComponent_Reload(WeaponsComponent* wc, int delta);
void WeaponsComponent_ScrollWeapons(WeaponsComponent* wc, int wheel_direction);
void WeaponsComponent_TryToShoot(WeaponsComponent* wc, float originX, float originY, float angle, Vector* bullets_vector,
                       float destinationX, float destinationY);
#endif // WEAPON_H

#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"
#include "weapon.h"
#include "weapons_component.h"

void Player_Move(Entity* p, Entity* map, int map_size, int delta, Vector* monsters_vector, Vector* bonus_vector)
{
    if(p->weapons_component->reloading)
    {
        WeaponsComponent_Reload(p->weapons_component);
    }

	p->camera->dx = 0;
	p->camera->dy = 0;
    CalculateVelocity(p, map, map_size);
    Player_CheckBonusCollision(p, bonus_vector);
    p->dx = floor(p->dx);
    p->dy = floor(p->dy);
    moveEntity(p, p->dx, p->dy);
    MoveCamera(p->camera, p->dx , p->dy );

}


Entity* Player_Create(float x, float y, int w, int h)
{
    Entity* p = Entity_Spawn();

	p->t = Player;
	p->texture = Player_tex;
	p->x = x;
	p->y = y;
	BoundingBox_Create(p, w, h);
	p->speed = 0.6;

    p->camera = CreateCamera();

    p->weapons_component = WeaponsComponent_Create();
    WeaponsComponent_AddWeaponToInventory(p->weapons_component, Weapon_Create(AutomaticRifle_w));
    WeaponsComponent_AddWeaponToInventory(p->weapons_component, Weapon_Create(Handgun_w));
    WeaponsComponent_AddWeaponToInventory(p->weapons_component, Weapon_Create(Shotgun_w));
    WeaponsComponent_AddWeaponToInventory(p->weapons_component, Weapon_Create(GrenadeLauncher_w));
    WeaponsComponent_ChangeWeapon(p->weapons_component, Handgun_w);

	return p;
}

void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector)
{
	//Box* temp = BoundingBox_CreateTemp(player);
    Jbool collision = Jfalse;
    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        collision = Jfalse;
        Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
        collision = BoundingBox_CheckSimpleCollision(&player->box, &bonus->box);
        if(collision)
        {
            if(bonus->t == Ammo)
            {
                WeaponsComponent_AddAmmo(player->weapons_component, player->weapons_component->current_weapon->type, 50);
                bonus->alive = Jfalse;
            }
        }
    }
}


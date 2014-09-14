#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"
#include "weapon.h"
#include "weapons_component.h"
#include "world.h"

void Player_Update(int delta, World* world)
{


    Entity* p = &world->player;
    p->invulnerability_timer -= delta;
    if(p->weapons_component->reloading)
    {
        WeaponsComponent_Reload(p->weapons_component, delta);
    }

	p->camera->dx = 0;
	p->camera->dy = 0;
    Entity_CollisionWithStuff(p, world);
    CollisionWithMonsters(p, &world->monsters_vector);
    Player_CheckBonusCollision(p, &world->bonus_vector);
    p->dx = floor(p->dx);
    p->dy = floor(p->dy);
    moveEntity(p, p->dx, p->dy);
    MoveCamera(p->camera, p->dx , p->dy );

}


Entity Player_Create(float x, float y, int w, int h)
{
    Entity p;

	p.t                             =   Player;
	p.texture                       =   Player_tex;
	p.x                             =   x;
	p.y                             =   y;
    p.hp                            =   50;
	p.speed                         =   0.6;
    p.blinking_frame                =   0;
    p.blinking_timer                =   0;
    p.camera                        =   CreateCamera();
    p.invulnerability_timer         =   0;
    p.weapons_component             =   WeaponsComponent_Create();

    BoundingBox_Create(&p, w, h);

    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(AutomaticRifle_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(Handgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(Shotgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(GrenadeLauncher_w));

    WeaponsComponent_ChangeWeapon(p.weapons_component,
                                  Handgun_w);

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

void Player_TakeDamage(Entity* p, int damage)
{
    p->hp -= damage;

    p->invulnerability_timer = 2000;
}


#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"
#include "weapon.h"
#include "weapons_component.h"
#include "world.h"
#include "movement_component.h"
PlayerC* PlayerC_Create()
{
    PlayerC* pc                 = (PlayerC*)malloc(sizeof(PlayerC));
    pc->invulnerability_timer   = 0;
	pc->blinking_timer          = 0;
	pc->blinking_frame          = 0;
    pc->stamina                 = 100;
    pc->max_stamina             = 100;
    pc->running                 = false;
    return pc;
}


void Player_Update(World* world)
{
    Entity* p = &world->player;
    if(p->playerC->running)
    {
        Player_Run(p);
    }
    else
    {
        Player_Walk(p);
    }

    if(game_state_g == GameState_Editing_Map)
    {
        p->movementC->speed = LEVEL_EDITOR_SPEED;
    }
    //p->playerC->dx = 0;
	//p->camera->dy = 0;

    if(game_state_g != GameState_Editing_Map)
    {
        p->playerC->invulnerability_timer -= delta_g;
        if(p->weaponsC->reloading)
        {
            WeaponsComponent_Reload(p->weaponsC);
        }


        Entity_CollisionWithStuff(p, world);
        Player_CheckBonusCollision(p, &world->bonus_vector);
    }



    Player_Move(p, p->movementC->dx, p->movementC->dy);
}


void Player_Move(Entity* p, float dx, float dy)
{
    moveEntity(p, dx, dy);
    p->playerC->cameraX += dx;
    p->playerC->cameraY += dy;
}


Entity Player_Create(float x, float y, int w, int h)
{
    Entity p;

	p.t                             =   Cat_Player;
	p.texture                       =   Tex_Player;
	p.x                             =   x;
	p.y                             =   y;
    p.hp                            =   50;

    p.movementC                     =   MovementC_Create();
    p.movementC->speed              =   BASE_PLAYER_SPEED;

    p.playerC                       =   PlayerC_Create();

    p.weaponsC                      =   WeaponsComponent_Create();

    BoundingBox_Create(&p, w, h);


    WeaponsComponent_AddWeaponToInventory(p.weaponsC,
                                          Weapon_Create(Handgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weaponsC,
                                          Weapon_Create(Shotgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weaponsC,
                                          Weapon_Create(GrenadeLauncher_w));
    WeaponsComponent_AddWeaponToInventory(p.weaponsC,
                                            Weapon_Create(AutomaticRifle_w));

    WeaponsComponent_ChangeWeapon(p.weaponsC,
                                  Handgun_w);

    p.weaponsC->is_monster = false;
	return p;
}

void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector)
{
    bool collision = false;
    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        collision = false;
        Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
        collision = BoundingBox_CheckSimpleCollision(&player->box, &bonus->box);
        if(collision)
        {
            Player_PickUpBonus(player, bonus);

        }
    }
}

void Player_Run(Entity* p)
{
    p->playerC->stamina -= 0.03 * delta_g;
    if(p->playerC->stamina <= 0)
        Player_StopRunning(p);

}

void Player_Walk(Entity* p)
{
    if(p->playerC->stamina < p->playerC->max_stamina)
        p->playerC->stamina += 0.01 * delta_g;

}


void Player_StartRunning(Entity* p)
{
    if(p->playerC->stamina > 0)
    {
        p->movementC->speed = BASE_PLAYER_SPEED * 1.5f;
        p->playerC->running = true;
    }

}

void Player_StopRunning(Entity* p)
{
    p->movementC->speed = BASE_PLAYER_SPEED;
    p->playerC->running = false;
}
void Player_PickUpBonus(Entity* player, Entity* bonus)
{
    WeaponsC* wc = player->weaponsC;

    int type = bonus->sub_category;
    Weapon_Type weapon_type = 999;

    switch(type)
    {
    case Bonus_GrenadeLauncher:
        weapon_type = GrenadeLauncher_w;
        break;
    case Bonus_Rifle:
        weapon_type = AutomaticRifle_w;
        break;
    case Bonus_Shotgun:
        weapon_type = Shotgun_w;
        break;
    case Bonus_Handgun:
        weapon_type = Handgun_w;
        break;
    }

    if(weapon_type != 999)
    {
        if(wc->weapons_inventory[weapon_type] != NULL)
        {
            WeaponsComponent_AddAmmo(wc,
                                 weapon_type,
                                 50);
        }
        else
        {
            WeaponsComponent_AddWeaponToInventory(
                    wc, Weapon_Create(weapon_type));
        }
    }

    bonus->alive = false;
}

//attacker is an array 5 pointers
//a pointer to the attacker of each side
//so attacker[Left] is the attacker on the left
void Player_TakeDamage(Entity* p, Entity** attacker)
{

    //push player in the opposite direction of the attacker
    if(attacker[Left])
    {
        p->hp -= attacker[Left]->damage;
        Player_Move(p, 20,  0);
    }
    if(attacker[Right])
    {
        p->hp -= attacker[Right]->damage;
        Player_Move(p, -20, 0);
    }

    if(attacker[Top])
    {
        p->hp -= attacker[Top]->damage;
        Player_Move(p, 0,   20);
    }

    if(attacker[Bottom])
    {
        p->hp -= attacker[Bottom]->damage;
        Player_Move(p, 0,   -20);
    }
    p->playerC->invulnerability_timer = 2000;
}


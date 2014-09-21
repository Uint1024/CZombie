#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"
#include "weapon.h"
#include "weapons_component.h"
#include "world.h"

void Player_Update(World* world)
{
    Entity* p = &world->player;
    if(p->running)
    {
        Player_Run(p);
    }
    else
    {
        Player_Walk(p);
    }

    if(game_state_g == GameState_Editing_Map)
    {
        p->speed = LEVEL_EDITOR_SPEED;
    }
    p->camera->dx = 0;
	p->camera->dy = 0;

    if(game_state_g != GameState_Editing_Map)
    {
        p->invulnerability_timer -= delta_g;
        if(p->weapons_component->reloading)
        {
            WeaponsComponent_Reload(p->weapons_component);
        }


        Entity_CollisionWithStuff(p, world);
        //CollisionWithMonsters(p, &world->monsters_vector);
        Player_CheckBonusCollision(p, &world->bonus_vector);
    }



    Player_Move(p, p->dx, p->dy);
}


void Player_Move(Entity* p, float dx, float dy)
{
    moveEntity(p, dx, dy);
    MoveCamera(p->camera, dx , dy);
}


Entity Player_Create(float x, float y, int w, int h)
{
    Entity p;

	p.t                             =   Cat_Player;
	p.texture                       =   Tex_Player;
	p.x                             =   x;
	p.y                             =   y;
    p.hp                            =   50;
	p.speed                         =   BASE_PLAYER_SPEED;
    p.blinking_frame                =   0;
    p.blinking_timer                =   0;
    p.camera                        =   CreateCamera();
    p.invulnerability_timer         =   0;
    p.weapons_component             =   WeaponsComponent_Create();
    p.stamina                       =   100;
    p.max_stamina                       =   100;
    p.running = Jfalse;
    BoundingBox_Create(&p, w, h);
    p.dx = 0;
    p.dy = 0;


    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(Handgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(Shotgun_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                          Weapon_Create(GrenadeLauncher_w));
    WeaponsComponent_AddWeaponToInventory(p.weapons_component,
                                            Weapon_Create(AutomaticRifle_w));

    WeaponsComponent_ChangeWeapon(p.weapons_component,
                                  Handgun_w);

    p.weapons_component->is_monster = Jfalse;
	return p;
}

void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector)
{
    Jbool collision = Jfalse;
    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        collision = Jfalse;
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
    p->stamina -= 0.03 * delta_g;
    if(p->stamina <= 0)
        Player_StopRunning(p);

}

void Player_Walk(Entity* p)
{
    if(p->stamina < p->max_stamina)
        p->stamina += 0.01 * delta_g;

}


void Player_StartRunning(Entity* p)
{
    if(p->stamina > 0)
    {
        p->speed = BASE_PLAYER_SPEED * 1.5f;
        p->running = Jtrue;
    }

}

void Player_StopRunning(Entity* p)
{
    p->speed = BASE_PLAYER_SPEED;
    p->running = Jfalse;
}
void Player_PickUpBonus(Entity* player, Entity* bonus)
{
    //WeaponsComponent* wc = player->weapons_component;
    /*    if(wc->weapons_inventory[bonus->corresponding_weapon] != NULL)
        {
            WeaponsComponent_AddAmmo(wc,
                                 bonus->corresponding_weapon,
                                 50);
        }
        else
        {
            WeaponsComponent_AddWeaponToInventory(
                    wc, Weapon_Create(bonus->corresponding_weapon));
        }

*/
    bonus->alive = Jfalse;
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
p->invulnerability_timer = 2000;
}


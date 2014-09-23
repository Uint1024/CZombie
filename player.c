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

static int playerTileX = 0;
static int playerTileY = 0;

PlayerC* PlayerC_Create()
{
    PlayerC* pc                 = (PlayerC*)malloc(sizeof(PlayerC));
    pc->invulnerability_timer   = 0;
	pc->blinking_timer          = 0;
	pc->blinking_frame          = 0;
    pc->stamina                 = 100;
    pc->max_stamina             = 100;
    pc->running                 = false;
    pc->vision_timer            =  0;
    return pc;
}

void Player_FieldOfView(Entity* p, World* world)
{
    float playerMiddleX = 0;
    float playerMiddleY = 0;
    Entity_GetMiddleCoordinates(p, &playerMiddleX, &playerMiddleY);

    for(int i = 0 ; i < world->map_size ; i++)
    {
        if(Entity_CheckNear(p, world->ground_map[i]))
        {
            world->ground_map[i]->in_dark = true;
        }
    }
    bool* wall = calloc(world->map_size, sizeof(bool));
    bool* visible = calloc(world->map_size, sizeof(bool));

    int fov = 10;
    playerTileX = p->x / TILE_SIZE;
    playerTileY = p->y / TILE_SIZE;

    for(int i = 1 ; i < 3 ; i++)
    {
        Player_ScanOctant(1, i, 1, 0, world);
    }



}

void Player_ScanOctant(int depth, int octant, float start_slope,
                       float end_slope,
                       World* world)
{
int y = 0;
int x = 0;
    //can see 10 tiles away
    int vision_distance = 10;
    int vision_distance2 = vision_distance * vision_distance;

    switch(octant)
    {
    case 1:
        //tile being scanned
        y = playerTileY - depth;
        if(y < 0) y = 0;

        x = playerTileX - (int)(start_slope * depth);
        if(x < 0) x = 0;

        while(C_GetSlopeBetween2Points(x, y, playerTileX, playerTileY, false) >= end_slope)
        {
            if(C_DistanceSquaredBetween2Points(x, y,
                                playerTileX, playerTileY) <= vision_distance2)
            {
                //current tile
                int  i = y * world->map_width + x;

                if(world->map[i]->solid)
                {
                    //tile on the left is not a wall
                    if(x - 1 >= 0 && !world->map[i - 1]->solid)
                    {
                        //scan row right over this one with adjusted end slope
                        Player_ScanOctant(depth + 1, octant, start_slope,
                                          C_GetSlopeBetween2Points(x - 0.5f, y + 0.5f,
                                                                   playerTileX, playerTileY,
                                                                   false),
                                          world);
                    }
                }
                else
                {
                    //tile on the left is a wall
                     if(y - 1 >= 0 && world->map[i - 1]->solid)
                     {
                         //adjust start slope
                         start_slope = C_GetSlopeBetween2Points(x - 0.5f, y - 0.5f,
                                                                playerTileX, playerTileY, false);
                     }


                     world->ground_map[i]->in_dark = false;

                }
            }
            x++;

        }
        x--;
        break;
    case 2://nne
        y = playerTileY - depth;
        if(y < 0) y = 0;

        x = playerTileX + (int)(start_slope * depth);
        if(x >= world->map_width) x = world->map_width - 1;

        while(C_GetSlopeBetween2Points(x, y, playerTileX, playerTileY, false) <= end_slope)
        {
            if(C_DistanceSquaredBetween2Points(x, y,
                                playerTileX, playerTileY) <= vision_distance2)
            {
                int  i = y * world->map_width + x;

                if(world->map[i]->solid)
                {
                    if(x + 1 <= world->map_width && !world->map[i + 1]->solid)
                    {
                        Player_ScanOctant(depth + 1, octant, start_slope,
                                          C_GetSlopeBetween2Points(x + 0.5f, y + 0.5f,
                                                                   playerTileX, playerTileY,
                                                                   false),
                                          world);
                    }
                }
                else
                {
                     if(x + 1 <= world->map_width && world->map[i + 1]->solid)
                     {
                         start_slope = -C_GetSlopeBetween2Points(x + 0.5f, y - 0.5f,
                                                                playerTileX, playerTileY, false);
                     }


                     world->ground_map[i]->in_dark = false;

                }
            }
            x--;

        }
        x++;
        break;
    }

    if (x < 0)
    {
        x = 0;
    }
    else if(x >= world->map_width)
    {
        x = world->map_width - 1;
    }

    if (y < 0)
    {
        y = 0;
    }
    else if(y >= world->map_height)
    {
        y = world->map_height - 1;
    }

    if (depth < vision_distance && !world->map[y * world->map_width + x]->solid)
    {
        Player_ScanOctant(depth + 1, octant, start_slope, end_slope, world);
    }
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

    p->playerC->vision_timer += delta_g;

    if(p->playerC->vision_timer > 60)
    {
        Player_FieldOfView(p, world);
        p->playerC->vision_timer = 0;
    }


    if(game_state_g == GameState_Editing_Map)
    {
        p->movementC->speed = LEVEL_EDITOR_SPEED;
    }


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
    p.in_dark = false;
    p.movementC                       =   MovementC_Create();
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


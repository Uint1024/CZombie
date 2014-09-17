#include "gameManager.h"
#include <stdio.h>
#include "world.h"
#include "math.h"
#include "zombie.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "bonus.h"

GameManager GameManager_Create()
{
    GameManager gm;
    gm.ai_on = Jtrue;
    gm.wave_id = 0;
    gm.game_mode = Survival_mode;
    gm.wave_timer = 40000;
    gm.waves[0] = Wave_Create(5, 0, 0, 0, 0);
    gm.waves[1] = Wave_Create(30, 0, 2, 0, 0);
    gm.waves[2] = Wave_Create(40, 5, 4, 0, 1);
    gm.waves[3] = Wave_Create(70, 10, 8, 0, 2);
    gm.waves[4] = Wave_Create(60, 30, 8, 0, 3);
    gm.waves[5] = Wave_Create(50, 50, 5, 0, 3);
    gm.waves[6] = Wave_Create(30, 50, 24, 1, 0);
    gm.waves[7] = Wave_Create(50, 70, 0, 2, 0);
    gm.waves[8] = Wave_Create(5, 0, 0, 9, 0);
    gm.waves[9] = Wave_Create(5, 0, 0, 0, 0);
    gm.waves[10] = Wave_Create(5, 0, 0, 0, 0);
    gm.waves[11] = Wave_Create(5, 0, 0, 0, 0);
    gm.waves[12] = Wave_Create(5, 0, 0, 0, 0);
    gm.waves[13] = Wave_Create(5, 0, 0, 0, 0);


    for(int type = 0 ; type < NB_ZOMBIE_TYPES ; type ++)
    {
        Entity* z = Entity_Spawn();
        BoundingBox_Create(z, 0,0);
        switch(type)
        {
         case Normal_Zombie:
            z->texture = Zombie_tex;
            z->box.height = 20;
            z->box.width = 20;
            z->speed = 0.2;
            z->hp = 2;
            z->damage = 2;
            break;
        case Fast_Zombie:
            z->texture = FastZombie_tex;
            z->box.height = 20;
            z->box.width = 20;
            z->speed = 0.25;
            z->hp = 2;
            z->damage = 4;
            break;
        case Heavy_Zombie:
            z->texture = HeavyZombie_tex;
            z->box.height = 40;
            z->box.width = 40;
            z->speed = 0.1;
            z->hp = 20;
            z->damage = 10;
            z->weapons_component = WeaponsComponent_Create(Jtrue);
            WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                                  Weapon_Create(Fireball_w));
            break;
        case Trooper_Zombie:
            z->texture = TrooperZombie_tex;
            z->box.height = 60;
            z->box.width = 45;
            z->speed = 0.1;
            z->hp = 20;
            z->damage = 10;
            z->weapons_component = WeaponsComponent_Create(Jtrue);
            WeaponsComponent_AddWeaponToInventory(z->weapons_component,
                                                  Weapon_Create(TripleFireball_w));
            break;
        case Huge_Zombie:
            z->texture = HugeZombie_tex;
            z->box.height = 100;
            z->box.width = 100;
            z->speed = 0.2;
            z->hp = 150;
            z->damage = 20;
            break;
        }

        gm.zombie_templates[type] = z;
    }

    gm.button_object_type_correspondance[NormalZombie_button] = Normal_Zombie;
    gm.button_object_type_correspondance[HeavyZombie_button] = Heavy_Zombie;
    gm.button_object_type_correspondance[FastZombie_button] = Fast_Zombie;
    gm.button_object_type_correspondance[HugeZombie_button] = Huge_Zombie;
    gm.button_object_type_correspondance[TrooperZombie_button] = Trooper_Zombie;
    gm.button_object_type_correspondance[GrassGround_button] = Grass_ground;
    gm.button_object_type_correspondance[DirtGround_button] = Dirt_ground;
    return gm;

}

Wave Wave_Create(int normal_zombies, int fast_zombies,
                 int heavy_zombies, int huge_zombies,
                 int trooper_zombies)
{
    Wave wave;

    wave.zombies[Normal_Zombie] = normal_zombies;
    wave.zombies[Fast_Zombie] = fast_zombies;
    wave.zombies[Heavy_Zombie] = heavy_zombies;
    wave.zombies[Huge_Zombie] = huge_zombies;
    wave.zombies[Trooper_Zombie] = trooper_zombies;

    return wave;
}

void GameManager_Update(GameManager* gm, World* world, int delta, Window* level_editor)
{
    GameManage_UpdateWorldEntities(gm, delta, world);
    //GameManager_UpdateEnnemyWaves(gm, world, delta);
    //GameManager_UpdateUI(level_editor);
}

/*void GameManager_UpdateUI(Window level_editor)
{
    //if(BoundingBox_CheckPointCollision())
}*/

void GameManage_UpdateWorldEntities(GameManager* gm, int delta, World* world)
{
    //because I don't want to type world-> 50 times
    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;

    Player_Update(delta, world);

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {
            Entity* projectile = (Entity*)Vector_Get(bullets_vector, i);

            if(projectile->t == Bullet_cat)
            {
                 Bullet_Update(projectile, delta, world);
            }
            else if(projectile->t == Grenade_cat)
            {
                Grenade_Update(projectile, delta, world);

                if(!projectile->alive)
                {
                    Vector_Push(explosions_vector, Explosion_Create(projectile->x - 64, projectile->y - 64) );
                }
            }

            if (projectile->alive == Jfalse)
            {
                Vector_Delete(bullets_vector, i);
            }
        }
        else
        {
            printf("Error during update of bullets vector : bullet = NULL");
        }
    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
            Bonus_Update(bonus, &world->player, delta);
            if (bonus->alive == Jfalse)
            {
                Vector_Delete(bonus_vector, i);
            }
        }
        else
        {
            printf("Error during update of bonus vector : bonus = NULL");
        }
    }

    if(gm->ai_on)
    {
        for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
        {
            if(Vector_Get(monsters_vector, i) != NULL)
            {
                Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);

                    Zombie_Update(mob, delta, world);

                    if (mob->alive == Jfalse)
                    {
                        Zombie_Die(mob, bonus_vector);
                        Vector_Delete(monsters_vector, i);

                    }
            }
            else
            {
                printf("Error during update of monsters vector : monster = NULL");
            }
        }
    }


    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        Entity* exp = (Entity*)Vector_Get(explosions_vector, i);
        Explosion_Update(exp, delta);
        if (!exp->alive)
        {
            Vector_Delete(explosions_vector, i);
        }
    }
}

void GameManager_UpdateEnnemyWaves(GameManager* gm, World* world, int delta)
{
    gm->wave_timer -= delta;


    if(Vector_Count(&world->monsters_vector) == 0 || gm->wave_timer <= 0)
    {
        gm->wave_timer = 30000;
        int randX = 0;
        int randY = 0;

        for(int z_type = 0 ; z_type < NB_ZOMBIE_TYPES ; z_type++)
        {
            for(int i = 0 ; i < gm->waves[gm->wave_id].zombies[z_type] ; i++)
            {

                GameManager_GeneratePositionOutsideOfScreen(&randX, &randY);
                Vector_Push(&world->monsters_vector,
                            CreateZombie(   z_type,
                                            randX + world->player.camera->x,
                                            randY + world->player.camera->y

                                        )
                            );
            }
        }

        gm->wave_id += 1;

    }
}

void GameManager_GeneratePositionOutsideOfScreen(int* randX, int* randY)
{
    Direction direction = rand() % 4;

    int modulusX = 1;
    int modulusY = 1;
    int bumpX = 0;
    int bumpY = 0;
    int signX = 1;
    int signY = 1;
    switch(direction)
    {
    case Left:
        modulusX = -800;
        bumpX = -600;
        signX = -1;
        modulusY = 800;
        break;
    case Top:
        modulusX = 1100;
        modulusY = -800;
        bumpY = -600;
        signY = -1;
        break;
    case Bottom:
        modulusX = 1100;
        modulusY = 800;
        bumpY = 1200;
        break;
    case Right:
        modulusX = 800;
        bumpX = 1600;
        modulusY = 800;
        break;
    case None:
        printf("Error, generated random position None\n");
        break;
    }

    *randX = signX * (rand() % modulusX);
    *randY = signY * (rand() % modulusY);

    *randX += bumpX;
    *randY += bumpY;
}

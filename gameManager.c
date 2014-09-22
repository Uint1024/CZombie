#include "gameManager.h"
#include <stdio.h>
#include "world.h"
#include "math.h"
#include "zombie.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "bonus.h"
#include "weapon.h"

GameManager GameManager_Create()
{
    GameManager gm;

    gm.ai_on = true;
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

void Game_StartMap(World* world)
{

    world->player.visible = true;
    world->player.solid = true;


    for(int i = 0 ; i < Vector_Count(&world->events_vector) ; i++)
    {

        Entity* event = (Entity*)Vector_Get(&world->events_vector, i);
        if(event->sub_category == Event_Player_Start)
        {
            world->player = Player_Create( event->x, event->y, 20, 20);
            world->player.visible = true;
            world->player.solid = true;
            world->player.playerC->cameraX = -screen_width_g/2 + world->player.x;
            world->player.playerC->cameraY = -screen_height_g/2 + world->player.y;
        }
    }


}
void GameManager_Update(GameManager* gm, World* world, Window* level_editor)
{
    Player_Update(world);

    if(game_state_g != GameState_Editing_Map)
    {
        GameManage_UpdateWorldEntities(gm, world);
    }

    //GameManager_UpdateEnnemyWaves(gm, world, delta);
    //GameManager_UpdateUI(level_editor);
}


void GameManage_UpdateWorldEntities(GameManager* gm, World* world)
{
    //because I don't want to type world-> 50 times
    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;



    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {

            Entity* projectile = (Entity*)Vector_Get(bullets_vector, i);

            if(projectile->t == Cat_Bullet)
            {

                 Bullet_Update(projectile, world);
            }
            else if(projectile->t == Cat_Grenade)
            {
                Grenade_Update(projectile, world);

                if(!projectile->alive)
                {
                    Vector_Push(explosions_vector, Explosion_Create(projectile->x - 64, projectile->y - 64) );
                }
            }

            if (projectile->alive == false)
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
            Bonus_Update(bonus, &world->player);
            if (bonus->alive == false)
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

                    Zombie_Update(mob, world);

                    if (mob->alive == false)
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
        Explosion_Update(exp);
        if (!exp->alive)
        {
            Vector_Delete(explosions_vector, i);
        }
    }
}

void GameManager_UpdateEnnemyWaves(GameManager* gm, World* world)
{
    gm->wave_timer -= delta_g;


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
                                            randX + world->player.playerC->cameraX,
                                            randY + world->player.playerC->cameraY

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

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
#include "door.h"

GameManager GameManager_Create()
{
    GameManager gm;

    gm.ai_on = true;
    gm.wave_id = 0;
    gm.game_mode = Survival_mode;
    gm.wave_timer = 40000;
/*    gm.waves[0] = Wave_Create(5, 0, 0, 0, 0);
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

*/


    return gm;

}

/*Wave Wave_Create(
{
    Wave wave;

    wave.zombies[Normal_Zombie] = normal_zombies;
    wave.zombies[Zombie_Fast] = Zombie_Fasts;
    wave.zombies[Zombie_Heavy] = Zombie_Heavys;
    wave.zombies[Zombie_Huge] = Zombie_Huges;
    wave.zombies[Zombie_Trooper] = Zombie_Troopers;

    return wave;
}
*/
void Game_StartMap(World* world)
{

    world->player.visible = true;
    world->player.solid = true;


    for(int i = 0 ; i < Vector_Count(&world->events_vector) ; i++)
    {

        Entity* event = (Entity*)Vector_Get(&world->events_vector, i);
        if(event->sub_category == Event_Player_Start)
        {
            world->player = Player_Create( event->x, event->y, 10, 10);
            world->player.visible = true;
            world->player.solid = true;
            world->player.playerC->cameraX = -screen_width_g/2 + world->player.x;
            world->player.playerC->cameraY = -screen_height_g/2 + world->player.y;
            //Graphics_SetCamera((-screen_width_g/2 + world->player.x), -screen_height_g/2 + world->player.y);
        }

    }


}
void GameManager_Update(GameManager* gm, World* world, Window* level_editor)
{



    Vector_Nullify(&world->non_null_walls);

    //oh god I don't know how to fix this horror
    for(int i = 0 ; i < world->map_size ; i++)
    {
        if(world->map[i] != NULL && world->map[i]->alive)
        {
            Vector_Push(&world->non_null_walls, world->map[i]);
        }
    }

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
    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;
    Vector* decals_vector = &world->decals_vector;
    Vector* props_vector = &world->props_vector;


    for(int i = 0 ; i < world->map_size ; i++)
    {
        if(world->map[i] != NULL)
        {
            if(world->map[i]->t == Cat_Door)
            {
                Door_Update(world->map[i], world);
            }

            if(!world->map[i]->alive)
            {
                free(world->map[i]);//walls don't have any compotent, no need to destroy them
                world->map[i] = NULL;
            }
        }
    }



    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {

            Entity* projectile = (Entity*)Vector_Get(bullets_vector, i);
            Entity_CalculateVisibility(projectile, world);

            if(!bullet_time_g)
            {
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
                    Entity_Destroy(projectile);
                    Vector_Delete(bullets_vector, i);
                }
            }
        }
        else
        {
            printf("Error during update of bullets vector : bullet = NULL");
        }
    }



    if(gm->ai_on)
    {
        for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);

            if(Entity_CheckDistance(mob, &world->player, 800))
            {
                Entity_CalculateVisibility(mob, world);
            }

            if(mob->zombieC->aggressive || Entity_CheckDistance(mob, &world->player, 400))
            {
                Zombie_Update(mob, world);
            }



            if (mob->alive == false)
            {
                Zombie_Die(mob, bonus_vector, decals_vector);
                Entity_Destroy(mob);
                Vector_Delete(monsters_vector, i);

            }
        }
    }



    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {

        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);

            if(Entity_CheckDistance(bonus, &world->player, 800))
            {
                if(!bullet_time_g)
                {
                    Bonus_Update(bonus, &world->player);
                }


                Entity_CalculateVisibility(bonus, world);

            }
            if (bonus->alive == false)
            {
                Entity_Destroy(bonus);
                Vector_Delete(bonus_vector, i);
            }
        }
        else
        {
            printf("Error during update of bonus vector : bonus = NULL");
        }
    }

    for(int i = 0 ; i < Vector_Count(decals_vector) ; i++)
    {

        Entity* decal = (Entity*)Vector_Get(decals_vector, i);

        if(Entity_CheckDistance(decal, &world->player, 800))
        {
            Entity_CalculateVisibility(decal, world);

        }
        if(Vector_Count(decals_vector) > 200)
        {
            if(decal->is_ennemy)
            {
                Vector_Delete(decals_vector, i);
            }
        }

    }

    for(int i = 0 ; i < Vector_Count(props_vector) ; i++)
    {
        Entity* prop = (Entity*)Vector_Get(props_vector, i);

        if(Entity_CheckDistance(prop, &world->player, 800))
        {
            Entity_CalculateVisibility(prop, world);
        }
        if (!prop->alive)
        {
            Entity_Destroy(prop);
            Vector_Delete(props_vector, i);
        }
    }





    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        Entity* exp = (Entity*)Vector_Get(explosions_vector, i);
        if(!bullet_time_g)
        {
            Explosion_Update(exp, world);
        }

        Entity_CalculateVisibility(exp, world);
        if (!exp->alive)
        {
            Entity_Destroy(exp);
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

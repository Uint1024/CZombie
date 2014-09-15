#include "gameManager.h"
#include "world.h"
#include "math.h"
#include "zombie.h"

GameManager GameManager_Create()
{
    GameManager gm;

    gm.wave_id = 0;
    gm.game_mode = Survival_mode;

    gm.waves[0] = Wave_Create(5, 0, 0, 0);
    gm.waves[1] = Wave_Create(15, 6, 0, 0);
    gm.waves[2] = Wave_Create(5, 25, 0, 0);
    gm.waves[3] = Wave_Create(28, 10, 5, 0);
    gm.waves[4] = Wave_Create(0, 36, 15, 0);
    gm.waves[5] = Wave_Create(50, 50, 5, 0);
    gm.waves[6] = Wave_Create(30, 50, 24, 1);
    gm.waves[7] = Wave_Create(50, 70, 0, 2);
    gm.waves[8] = Wave_Create(5, 0, 0, 9);
    gm.waves[9] = Wave_Create(5, 0, 0, 0);
    gm.waves[10] = Wave_Create(5, 0, 0, 0);
    gm.waves[11] = Wave_Create(5, 0, 0, 0);
    gm.waves[12] = Wave_Create(5, 0, 0, 0);
    gm.waves[13] = Wave_Create(5, 0, 0, 0);
    return gm;
}

Wave Wave_Create(int normal_zombies, int fast_zombies,
                 int heavy_zombies, int huge_zombies)
{
    Wave wave;

    wave.zombies[Normal_Zombie] = normal_zombies;
    wave.zombies[Fast_Zombie] = fast_zombies;
    wave.zombies[Heavy_Zombie] = heavy_zombies;
    wave.zombies[Huge_Zombie] = huge_zombies;

    return wave;
}

void GameManager_Update(GameManager* gm, World* world, int delta)
{
    GameManage_UpdateWorldEntities(delta, world);
    GameManager_UpdateEnnemyWaves(gm, world);
}

void GameManage_UpdateWorldEntities(int delta, World* world)
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

            if(projectile->t == Bullet)
            {
                 Bullet_Update(projectile, delta, world);
            }
            else if(projectile->t == Grenade)
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

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
            //if(mob->t == Zombie)
            //{
                Zombie_Update(mob, delta, world);

                if (mob->alive == Jfalse)
                {
                    Zombie_Die(mob, bonus_vector);
                    Vector_Delete(monsters_vector, i);

                }
           // }
        }
        else
        {
            printf("Error during update of monsters vector : monster = NULL");
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

GameManager_UpdateEnnemyWaves(GameManager* gm, World* world)
{
    if(Vector_Count(&world->monsters_vector) == 0)
    {
        gm->wave_id += 1;
        //Jbool all_mobs_generated = Jfalse;

        int randX = 0;
        int randY = 0;
        //while(!all_mobs_generated)
        //{


            for(int z_type = 0 ; z_type < NB_ZOMBIE_TYPES ; z_type++)
            {
                for(int i = 0 ; i < gm->waves[gm->wave_id].zombies[z_type] ; i++)
                {

                    GameManager_GeneratePositionOutsideOfScreen(&randX, &randY);
                    printf("%d\n", randX);
                    Vector_Push(&world->monsters_vector,
                                CreateZombie(   z_type,
                                                randX + world->player.camera->x,
                                                randY + world->player.camera->y
                                            )
                                );
                }
            }
        //}
       /* for(int i = 0 ; i < pow(2, gm->wave_id + 1) ; i++)
        {

            Vector_Push(&world->monsters_vector,
                        CreateZombie(randX + world->player.camera->x,
                                     randY + world->player.camera->y,
                                     0.2));
        }*/

    }
}

GameManager_GeneratePositionOutsideOfScreen(int* randX, int* randY)
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
    }

    *randX = signX * (rand() % modulusX);
    *randY = signY * (rand() % modulusY);

    *randX += bumpX;
    *randY += bumpY;
}

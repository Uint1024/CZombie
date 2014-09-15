#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "common.h"
typedef struct World World;

typedef struct Wave
{
    int         zombies[NB_ZOMBIE_TYPES];
} Wave;

typedef struct GameManager
{
    int         wave_id;
    Wave        waves[20];
    Game_Mode   game_mode;
} GameManager;

GameManager GameManager_Create();
void GameManager_Update(GameManager* gm, World* world, int delta);
void GameManage_UpdateWorldEntities(int delta, World* world);
Wave Wave_Create(int normal_zombies, int fast_zombies,
                 int heavy_zombies, int huge_zombies);
GameManager_GeneratePositionOutsideOfScreen(int* randX, int* randY);
#endif // GAME_MANAGER_H

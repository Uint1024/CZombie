#ifndef PLAYER_H
#define PLAYER_H
#include "linkedList.h"
#include "vector.h"
#include "common.h"

typedef struct Entity Entity;
typedef struct World World;


typedef struct PlayerC
{
	int                     invulnerability_timer;
	int                     blinking_timer;
	int                     blinking_frame;
    float                   stamina;
    float                   max_stamina;
    bool                    running;
    float                   cameraX;
    float                   cameraY;
    float vision_timer;
    Vec2                    vision_points[200];
    float                   time_stop;
    float                   max_time_stop;
}PlayerC;

PlayerC* PlayerC_Create();
void Player_Update(World* world);
Entity Player_Create(float x, float y, int w, int h);
void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector);
void Player_TakeDamage(Entity* p, Entity** attacker);
void Player_Move(Entity* p,float dx, float dy);
void Player_PickUpBonus(Entity* player, Entity* bonus);
void Player_Run(Entity* p);
void Player_StartRunning(Entity* p);
void Player_StopRunning(Entity* p);
void Player_Walk(Entity* p);
void Player_FieldOfView(Entity* p, World* world);
void Player_ScanOctant(int depth, int octant, float start_slope,
                       float end_slope,
                       World* world);
void Player_Reset(Entity* player);

void Player_ScanVisionPoints(Entity* p, World* world);
#endif

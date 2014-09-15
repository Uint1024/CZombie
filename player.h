#ifndef PLAYER_H
#define PLAYER_H
#include "linkedList.h"
#include "vector.h"
typedef struct Entity Entity;
typedef struct World World;

void Player_Update(int delta, World* world);
Entity Player_Create(float x, float y, int w, int h);
void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector);
void Player_TakeDamage(Entity* p, Entity** attacker);
void Player_Move(Entity* p,float dx, float dy);
void Player_PickUpBonus(Entity* player, Entity* bonus);

#endif

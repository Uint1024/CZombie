#ifndef PLAYER_H
#define PLAYER_H
#include "linkedList.h"
#include "vector.h"
typedef struct Entity Entity;

void Player_Update(Entity* p, Entity* map, int map_size, int delta, Vector* monsters_vector, Vector* bonus_vector);
Entity* Player_Create(float x, float y, int w, int h);
void Player_CheckBonusCollision(Entity* player, Vector* bonus_vector);
#endif

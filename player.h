#ifndef PLAYER_H
#define PLAYER_H
#include "linkedList.h"
typedef struct Entity Entity;

void Player_Move(Entity* p, Entity* map, int map_size, Entity* camera, List* bonus_list, int delta, List* mob_list);
Entity* Player_Create(float x, float y, int w, int h);
void Player_CheckBonusCollision(Entity* player, List* bonus_list);
#endif

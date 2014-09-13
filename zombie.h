#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "linkedList.h"
typedef struct Entity Entity;

void UpdateZombie(Entity* z, Entity* player, Entity* map, int map_size, List* mob_list, int delta);

Entity* CreateZombie(float x, float y, float speed);
void Zombie_Die(Entity* zombie, List* bonus_list);
#endif

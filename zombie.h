#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "linkedList.h"
#include "vector.h"

typedef struct Entity Entity;

void UpdateZombie(Entity* z, Entity* player, Entity* map,
                  int map_size, Vector* monsters_vector, int delta,
                  Vector* explosions_vector);

Entity* CreateZombie(float x, float y, float speed);
void Zombie_Die(Entity* zombie, Vector* bonus_vector);
#endif

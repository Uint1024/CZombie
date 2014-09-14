#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "linkedList.h"
#include "vector.h"

typedef struct Entity Entity;
typedef struct World World;

void Zombie_Update(Entity* z, int delta, World* world);

Entity* CreateZombie(float x, float y, float speed);
void Zombie_Die(Entity* zombie, Vector* bonus_vector);
#endif

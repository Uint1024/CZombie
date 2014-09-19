#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "linkedList.h"
#include "vector.h"

typedef struct Entity Entity;
typedef struct World World;

void Zombie_Update(Entity* z, int delta, World* world);

Entity* CreateZombie(Zombie_Type type, float x, float y);
void Zombie_Die(Entity* zombie, Vector* bonus_vector);
void  Zombie_Shoot(Entity* z, World* world);
void Zombie_Ai(Entity* z, World* world);
void Zombie_BecomeAggressive(Entity* z, World* world);
#endif

#ifndef BULLET_H
#define BULLET_H
#include "entity.h"
#include "linkedList.h"
#include "vector.h"
typedef struct World World;

Entity* Bullet_Create(float x, float y, float angle, float speed, Jbool is_ennemy_bullet);
void Bullet_Update(Entity* bullet, int delta, World* world);

#endif

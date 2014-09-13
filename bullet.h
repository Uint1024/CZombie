#ifndef BULLET_H
#define BULLET_H
#include "entity.h"
#include "linkedList.h"
#include "vector.h"

Entity* Bullet_Create(float x, float y, float angle, float speed);
void Bullet_Move(Entity* bullet, Entity* map, int map_size, Vector* monsters_vector, int delta, Entity* camera);

#endif

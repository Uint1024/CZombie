#ifndef BULLET_H
#define BULLET_H
#include "entity.h"
#include "linkedList.h"

Entity* Bullet_Create(float x, float y, float angle, float speed);
void Bullet_Move(Entity* bullet, Entity* map, int map_size, List* monsters, int delta, Entity* camera);

#endif

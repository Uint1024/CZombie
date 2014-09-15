#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "common.h"
#include "entity.h"

Entity* Explosion_Create(int x, int y);
void Explosion_Update(Entity* exp, int delta);

#endif // EXPLOSIVE_H

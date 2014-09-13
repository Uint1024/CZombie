#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "common.h"
#include "entity.h"

Entity* Explosion_Create(int x, int y);
Entity* Explosion_Update(Entity* explosion, int delta);

#endif // EXPLOSIVE_H

#ifndef BONUS_H
#define BONUS_H
#include "entity.h"
#include "common.h"

Entity* Bonus_Create(Type bonus_type, float x, float y);
void Bonus_Update(Entity* bonus, Entity* player);
#endif // BONUS_H

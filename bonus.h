#ifndef BONUS_H
#define BONUS_H
#include "entity.h"
#include "common.h"

Entity* Bonus_Create(Bonus_type bonus_type, float x, float y, float angle);
void Bonus_Update(Entity* bonus, Entity* player,int delta);
void Bonus_GenerateRandom(Vector* bonus_vector, Entity* source);
#endif // BONUS_H

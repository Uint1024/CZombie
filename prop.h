#ifndef PROP_H
#define PROP_H
#include "common.h"

typedef struct Entity Entity;

Entity* Prop_Create(Bonus_type type, float x, float y, float angle);
#endif // PROP_H

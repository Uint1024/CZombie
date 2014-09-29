#ifndef DECALS_H
#define DECALS_H
#include "common.h"

typedef struct Entity Entity;

Entity* Decal_Create(Decal_Type type, float x, float y, float angle);

#endif // DECALS_H

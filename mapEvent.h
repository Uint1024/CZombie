#ifndef MAPEVENT_H
#define MAPEVENT_H
#include "common.h"
typedef struct Entity Entity;

Entity* MapEvent_Create(MapEvent_Type type, int x, int y);
#endif // MAPEVENT_H

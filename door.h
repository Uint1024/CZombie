#ifndef DOOR_H
#define DOOR_H
#include "common.h"

typedef struct Entity Entity;

//doors are Entity
Entity* Door_Create(Door_Type type, int x, int y);
void Door_Open(Entity* d);
void Door_Close(Entity* d);
void Door_Switch(Entity* d);

#endif // DOOR_H

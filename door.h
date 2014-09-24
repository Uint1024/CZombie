#ifndef DOOR_H
#define DOOR_H
#include "common.h"

typedef struct World World;
typedef struct Entity Entity;

//doors are Entity
Entity* Door_Create(Door_Type type, int x, int y);
void Door_Open(Entity* d);
void Door_Close(Entity* d);
void Door_Switch(Entity* d);
void Door_Die(Entity* d);
void Door_GetAttacked(Entity* d, Entity* attacker);
void Door_Update(Entity* d, World* w);
#endif // DOOR_H

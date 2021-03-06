#ifndef WALL_H
#define WALL_H
#include <math.h>
#include <stdlib.h>
#include "entity.h"


Entity* Ground_Create(Ground_Type type, float x, float y);
Entity* Wall_Create(Wall_Type type, int x_, int y_, World* world);
void Structure_GetAttacked(Entity* d, Entity* attacker);
void Structure_Die(Entity* d);
Entity* Wall_CreateEmpty();
void Wall_Update_Tile_Type(Entity* wall, World* world);
bool Wall_IsWall(Entity* w);
#endif

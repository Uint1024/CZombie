#ifndef WALL_H
#define WALL_H
#include <math.h>
#include <stdlib.h>
#include "entity.h"


Entity* Ground_Create(Ground_Type type, float x, float y);
Entity* Wall_Create(Wall_Type type, int x_, int y_);
#endif

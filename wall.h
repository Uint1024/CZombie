#ifndef WALL_H
#define WALL_H
#include <math.h>
#include <stdlib.h>
#include "entity.h"

Entity* Wall_Create(int x_, int y_);
Entity* Ground_Create(Ground_Type type, float x, float y);
#endif

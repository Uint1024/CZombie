#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "common.h"
typedef struct Entity Entity;

//#include "entity.h"
typedef struct Box
{
	int width;
	int height;
	float left;
	float top;
	float right;
	float bottom;
} Box;

Box* BoundingBox_CreateTemp(Entity* ent);
void BoundingBox_Create(Entity* ent, int width, int height);
Direction BoundingBox_CheckOutOfScreen(Box* box);
void BoundingBox_Update(Entity* ent);
Direction BoundingBox_CheckCollision(Box* currentBox1, Box* nextBox1, Box* box2);
Jbool BoundingBox_CheckSimpleCollision(Box* box1, Box* box2);

#endif

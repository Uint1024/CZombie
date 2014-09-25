#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "common.h"
#include <SDL.h>
typedef struct Entity Entity;
typedef struct Window Window;

//#include "entity.h"
typedef struct Box
{
	int width;
	int height;
	float left;
	float top;
	float right;
	float bottom;

	//for when the entity's sprite isn't aligned with its bbox
	int offsetX;
	int offsetY;
} Box;

Box* BoundingBox_CreateTemp(Entity* ent);
void BoundingBox_Create(Entity* ent, int width, int height);
Direction BoundingBox_CheckOutOfScreen(Box* box, Entity* camera);
void BoundingBox_Update(Entity* ent);
Direction BoundingBox_CheckCollision(Box* currentBox1, Box* nextBox1, Box* box2);
bool BoundingBox_CheckSimpleCollision(Box* box1, Box* box2);
bool BoundingBox_CheckPointCollision(int x, int y, Box* box2);
void BoundingBox_CreateWindow(Window* ent, int width, int height);
void BoundingBox_UpdateWindow(Box* box, int newX, int newY);
Box BoundingBox_CreateBetter(int x, int y, int width, int height);
void BoundingBox_UpdateNewSize(Box* box, int newW, int newH);
SDL_Rect BoundingBox_GetSDLRect(Box* box);
Box BoundingBox_CreateWithOffset(int x, int y, int width, int height, int offsetX, int offsetY);
#endif

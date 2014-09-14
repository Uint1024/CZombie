#include "boundingBox.h"
#include "entity.h"

//create temporary bounding box from entity's position and velocity
Box* BoundingBox_CreateTemp(Entity* ent)
{
	Box* tempBox = (Box*)malloc(sizeof(Box));
	tempBox->width = ent->box.width;
	tempBox->height = ent->box.height;
	tempBox->left = ent->box.left + ent->dx;
	tempBox->top = ent->box.top + ent->dy;
	tempBox->right = ent->box.right + ent->dx;
	tempBox->bottom = ent->box.bottom + ent->dy;

	return tempBox;
}



//create bounding box after entity creation
void BoundingBox_Create(Entity* ent, int width, int height)
{
	ent->box.width = width;
	ent->box.height = height;
	ent->box.left = ent->x;
	ent->box.top = ent->y;
	ent->box.right = ent->x + width;
	ent->box.bottom = ent->y + height;
}

//return direction of exit if the box is completely outside of the screen
Direction BoundingBox_CheckOutOfScreen(Box* box, Entity* camera)
{
	if (box->top - camera->y > 768)
		return Bottom;
	else if (box->bottom  - camera->y < 0)
		return Top;
	else if (box->right  - camera->x < 0)
		return Left;
	else if (box->left - camera->x > 1080)
	{
		return Right;
	}
	else
		return None;
}

//add velocity to bounding box
void BoundingBox_Update(Entity* ent)
{
	ent->box.left = ent->x;
	ent->box.top = ent->y;
	ent->box.right = ent->box.left + ent->box.width;
	ent->box.bottom = ent->box.top + ent->box.height;
}

//check collision between 2 objects, returns direction of collision
Direction BoundingBox_CheckCollision(Box* currentBox1, Box* nextBox1, Box* box2)
{

	if (nextBox1->right >= box2->left && nextBox1->left <= box2->left &&
		currentBox1->right - 5 <= box2->left &&
		nextBox1->bottom >= box2->top && nextBox1->top <= box2->bottom)
	{

		return Right;

	}
	else if (nextBox1->left <= box2->right && nextBox1->right >= box2->right &&
		currentBox1->left + 5 >= box2->right &&
		nextBox1->bottom >= box2->top && nextBox1->top <= box2->bottom)
	{
		return Left;
	}
	//object 2 is at the bottom of objet 1
	else if (nextBox1->bottom >= box2->top && nextBox1->top <= box2->top &&
		currentBox1->bottom - 5 <= box2->top &&
		nextBox1->right >= box2->left && nextBox1->left <= box2->right)
	{
		return Bottom;
	}


	else if (nextBox1->top <= box2->bottom && nextBox1->bottom >= box2->bottom &&
		currentBox1->top + 5 >= box2->bottom &&
		nextBox1->right >= box2->left && nextBox1->left <= box2->right)
	{
		return Top;
	}


	return None;
}

//simple collision detection, returns true if collision
Jbool BoundingBox_CheckSimpleCollision(Box* box1, Box* box2)
{

	if (box1->left > box2->right ||
		box1->right < box2->left ||
		box1->top > box2->bottom ||
		box1->bottom < box2->top)
	{
		return Jfalse;
	}
	else
	{
		return Jtrue;
	}
}

//check collision between a point (vec2) and a box
Jbool BoundingBox_CheckPointCollision(int x, int y, Box* box2)
{

	if (x > box2->right ||
		x < box2->left ||
		y > box2->bottom ||
		y < box2->top)
	{
		return Jfalse;
	}
	else
	{
		return Jtrue;
	}
}


Box BoundingBox_CreateFromAllValues(int width, int height, float x, float y)
{
    Box box;
    box.width = width;
    box.height = height;
    box.left = x;
    box.right = x + width;
    box.top = y;
    box.bottom = y + height;

    return box;
}

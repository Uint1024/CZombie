#include "camera.h"
#include "entity.h"
Entity* CreateCamera()
{
	Entity* camera = (Entity*)malloc(sizeof(Entity));
	camera->x = 0;
	camera->y = 0;
	return camera;
}

void MoveCamera(Entity* c, float dx, float dy)
{
	c->x += dx;
	c->y += dy;
}

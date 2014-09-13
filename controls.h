#ifndef CONTROLS_H
#define CONTROLS_H
#include "camera.h"
#include "entity.h"
#include "linkedList.h"
#include "vector.h"

typedef struct Controls
{
	Jbool pressedKeys[200];
	Jbool pressedMouseButtons[20];
	SDL_Event e;
	int mouseX;
	int mouseY;
	int mouseTileX;
	int mouseTileY;
	int mousePositionInWorldX;
	int mousePositionInWorldY;
	int mouseWheelPos;
} Controls;

Controls* CreateControls();
Jbool PoolInputs(Controls* controls, Entity* camera);
void ProcessInputs(Controls* controls, Entity* player,
	Entity* map, int map_width, int map_height, int delta,
	Vector* bullets_vector, Vector* monsters_vector);
#endif

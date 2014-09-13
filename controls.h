#ifndef CONTROLS_H
#define CONTROLS_H
#include "camera.h"
#include "entity.h"
#include "linkedList.h"

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
} Controls;

Controls* CreateControls();
Jbool PoolInputs(Controls* controls, Entity* camera);
void ProcessInputs(Controls* controls, Entity* player, List* bullets,
	Entity* map, int map_width, int map_height, List* monsters, int delta);
#endif

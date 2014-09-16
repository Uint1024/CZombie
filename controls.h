#ifndef CONTROLS_H
#define CONTROLS_H
#include "camera.h"
#include "entity.h"
#include "linkedList.h"
#include "vector.h"

typedef struct World World;

typedef struct Controls
{
	Jbool pressedKeys[200];
	Jbool pressedMouseButtons[20];
	Jbool previousPressedMouseButtons[20];
	SDL_Event e;
	int mouseX;
	int mouseY;
	int mouseTileX;
	int mouseTileY;
	int mousePositionInWorldX;
	int mousePositionInWorldY;
	int mouseWheelPos;
	int timer_menu;
} Controls;

Controls* CreateControls();
Jbool   Inputs_PoolInputs(Controls* controls, Entity* camera);
void    Inputs_ApplyInputs( Controls* controls, int delta,
                            Jbool* game_started,
                            World* world, Window* level_editor);
void Inputs_ProcessInputs(Controls* controls, int delta, Jbool* game_started, World* world, Window* level_editor);
#endif

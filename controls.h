#ifndef CONTROLS_H
#define CONTROLS_H
#include "camera.h"
#include "entity.h"
#include "linkedList.h"
#include "vector.h"

typedef struct World World;
typedef struct Button Button;
typedef struct GameManager GameManager;

extern bool previousPressedKeys_g[200];

typedef struct Controls
{
	bool pressedKeys[200];

	bool pressedMods[50];
	bool pressedMouseButtons[20];
	bool previousPressedMouseButtons[20];
	SDL_Event e;
	int mouseX;
	int mouseY;
    int previousMouseX;
	int previousMouseY;
	int mouseTileX;
	int mouseTileY;
	int mousePositionInWorldX;
	int mousePositionInWorldY;
	int tileInPixelsX;
	int tileInPixelsY;
	int mouseWheelPos;
	int timer_menu;
	Window*     active_window;
	bool       cursor_resize_left_right;
	bool       cursor_resize_up_down;
	bool       resizing_right;
	bool       resizing_left;
	bool       resizing_up;
	bool       resizing_down;
	Button*     active_button;
	bool       hovering_on_window;
	int         last_ai_switch;
} Controls;

Controls* CreateControls();
bool   Inputs_PoolInputs(Controls* controls, Entity* camera);
void    Inputs_ApplyInputs( Controls* controls,
                            World* world, Window* level_editor, GameManager* gm);

void Inputs_ProcessInputs(Controls* controls, World* world, Window* level_editor,
                          GameManager* game_manager);

void Inputs_ApplyInputsLevelEditor(Controls* controls,
                            World* world, Window* level_editor,
                            GameManager* gm);
#endif

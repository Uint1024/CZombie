#ifndef CONTROLS_H
#define CONTROLS_H
#include "camera.h"
#include "entity.h"
#include "linkedList.h"
#include "vector.h"

typedef struct World World;
typedef struct Button Button;
typedef struct GameManager GameManager;

extern Jbool previousPressedKeys_g[200];

typedef struct Controls
{
	Jbool pressedKeys[200];

	Jbool pressedMods[50];
	Jbool pressedMouseButtons[20];
	Jbool previousPressedMouseButtons[20];
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
	Jbool       cursor_resize_left_right;
	Jbool       cursor_resize_up_down;
	Jbool       resizing_right;
	Jbool       resizing_left;
	Jbool       resizing_up;
	Jbool       resizing_down;
	Button*     active_button;
	Jbool       hovering_on_window;
	int         last_ai_switch;
} Controls;

Controls* CreateControls();
Jbool   Inputs_PoolInputs(Controls* controls, Entity* camera);
void    Inputs_ApplyInputs( Controls* controls, int delta,
                            Jbool* game_started,
                            World* world, Window* level_editor, GameManager* gm);
void Inputs_ProcessInputs(Controls* controls, int delta, Jbool* game_started, World* world, Window* level_editor, GameManager* game_manager);
#endif

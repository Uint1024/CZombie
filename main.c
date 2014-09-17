#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "string.h"

#include "entity.h"
#include "controls.h"
#include "linkedList.h"
#include "bullet.h"
#include "common.h"
#include "wall.h"
#include "camera.h"
#include "player.h"
#include "zombie.h"
#include "graphics.h"
#include "weapon.h"
#include "weapons_component.h"
#include "bonus.h"
#include "vector.h"
#include "explosion.h"
#include "menu_manager.h"
#include "world.h"
#include "gameManager.h"
#include "window.h"

Jbool debug_mode = Jfalse;

int main(int argc, char* args[])
{



    srand(time(NULL));
	int screen_width = 1080;
	int screen_height = 768;

    Graphics* graphics = Graphics_Create(screen_width, screen_height);
	Jbool running = Jtrue;

    GameManager game_manager = GameManager_Create();

	Controls* controls = CreateControls();

    World world = World_Initialize(140, 140, screen_width, screen_height);

	int time_now = SDL_GetTicks();
	int time_last_frame = 0;
	int delta = 1;
	float fps = 0;

    int chrono_update = 0;
    int time_last_frame_real = 0;
    int time_this_frame_real = 0;

    Jbool game_started = Jtrue;

    Window level_editor = Window_CreateLevelEditor();
    MenuManager menu_manager = MenuManager_Create(graphics);
    int ms_delay_between_frame = 1000 / 60;

	while (running)
	{
	    time_this_frame_real = SDL_GetTicks();
        chrono_update += time_this_frame_real - time_last_frame_real;

        if(chrono_update > ms_delay_between_frame)
        {
            time_now = SDL_GetTicks();
            delta = time_now - time_last_frame;

            if(delta > 0)
            {
                fps = 1000 / chrono_update;
            }

            chrono_update = 0;

            Inputs_ProcessInputs(controls, delta, &game_started, &world, &level_editor, &game_manager);

            if(game_started)
            {
                GameManager_Update(&game_manager, &world, delta, &level_editor);
                Graphics_RenderGame(graphics,&world, controls, fps, delta, &level_editor, &game_manager);
            }
            else
            {
                MenuManager_Update(&menu_manager, controls, &game_started, &running, delta, &world);
                Graphics_RenderMenu(graphics, menu_manager.active_menu, controls);
            }

            time_last_frame = time_now;
        }

        time_last_frame_real = time_this_frame_real;
	}


	SDL_DestroyRenderer(graphics->renderer);
	SDL_DestroyWindow(graphics->window);

	SDL_Quit();
	return 0;
}

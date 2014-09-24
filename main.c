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
#include "menu.h"

bool           debug_mode = false;
bool           display_menu_g = true;
Game_State      game_state_g = GameState_Main_Menu;
bool             unlimited_ammo_g = false;
bool reloading_g = false;
int screen_width_g = 1080;
int screen_height_g = 768;
int delta_g = 0;
int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Sound_Init();

    Game_InitData();

    srand(time(NULL));
	int screen_width = 1080;
	int screen_height = 768;

    Graphics* graphics = Graphics_Create(screen_width, screen_height);
	bool running = true;

    GameManager game_manager = GameManager_Create();

	Controls* controls = CreateControls();

    World world = World_Initialize(MAP_SIZE, MAP_SIZE);

	int time_now = SDL_GetTicks();
	int time_last_frame = 0;
	float fps = 0;

    //once this reaches 1000 / 60, we update the game (to have 30 updates per second)
    int chrono_update = 0;

    //number of ms since the beginning of the game
    int time_last_frame_real = 0;


    Window level_editor = Window_CreateLevelEditor();

    MenuManager menu_manager = MenuManager_Create(graphics);
    int ms_delay_between_frame = 1000 / 60;
	while (running)
	{
        time_now = SDL_GetTicks();
        chrono_update += time_now - time_last_frame_real;

        if(chrono_update > ms_delay_between_frame)
        {

            delta_g = time_now - time_last_frame;
            if(delta_g > 0)
            {
                fps = 1000 / delta_g;
            }

            chrono_update = 0;

            Inputs_ProcessInputs(controls, &world, &level_editor, &game_manager);

            if(!display_menu_g)
            {

                GameManager_Update(&game_manager, &world, &level_editor);
                Graphics_RenderGame(graphics,&world, controls, fps, &level_editor, &game_manager);
            }
            else
            {
                MenuManager_Update(&menu_manager, controls, &running, &world);
                Graphics_RenderMenu(graphics, menu_manager.active_menu, controls);
            }

            for(int i = 0 ; i < 200 ; i++)
            {
                previousPressedKeys_g[i] = controls->pressedKeys[i];
            }
            time_last_frame = time_now;
        }

        time_last_frame_real = time_now;
	}


	SDL_DestroyRenderer(graphics->renderer);
	SDL_DestroyWindow(graphics->window);

	SDL_Quit();
	return 0;
}

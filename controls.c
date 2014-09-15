#include <SDL.h>
#include "entity.h"
#include "controls.h"
#include "common.h"
#include "linkedList.h"
#include <math.h>
#include "bullet.h"
#include "wall.h"
#include "camera.h"
#include "zombie.h"
#include "weapon.h"
#include "weapons_component.h"
#include <stdio.h>
#include "world.h"

Controls* CreateControls()
{
	Controls* controls = (Controls*)malloc(sizeof(Controls));
	for (int i = 0; i < 200; i++)
	{
		controls->pressedKeys[i] = Jfalse;
	}
    controls->mouseWheelPos = 0;
	return controls;
}

void Inputs_ProcessInputs(Controls* controls, int delta, Jbool* game_started, World* world)
{
    Inputs_PoolInputs(controls, world->player.camera);
    Inputs_ApplyInputs(controls, delta, game_started, world);
}

Jbool Inputs_PoolInputs(Controls* controls, Entity* camera)
{
    controls->mouseWheelPos = 0;
	while (SDL_PollEvent(&controls->e))
	{
		if (controls->e.type == SDL_QUIT || controls->e.key.keysym.scancode == SDL_SCANCODE_F4)
		{
			return Jfalse;
		}

		if(controls->e.key.keysym.scancode == SDL_SCANCODE_P)
		{
			debug_mode = Jtrue;
		}

        if(controls->e.key.keysym.scancode == SDL_SCANCODE_O)
		{
			debug_mode = Jfalse;
		}

		if (controls->e.type == SDL_KEYDOWN)
		{
			controls->pressedKeys[controls->e.key.keysym.scancode] = Jtrue;
		}

		if (controls->e.type == SDL_KEYUP)
		{
			controls->pressedKeys[controls->e.key.keysym.scancode] = Jfalse;
		}

		if (controls->e.type == SDL_MOUSEBUTTONDOWN)
		{
			controls->pressedMouseButtons[controls->e.button.button] = Jtrue;
		}
		if (controls->e.type == SDL_MOUSEBUTTONUP)
		{
			controls->pressedMouseButtons[controls->e.button.button] = Jfalse;
		}

		if(controls->e.type == SDL_MOUSEWHEEL)
        {
            controls->mouseWheelPos += controls->e.wheel.y;
        }
	}

	SDL_GetMouseState(&controls->mouseX, &controls->mouseY);

	controls->mousePositionInWorldX = controls->mouseX + camera->x;

	controls->mousePositionInWorldY = controls->mouseY + camera->y;
	controls->mouseTileX = (controls->mousePositionInWorldX - controls->mousePositionInWorldX % TILE_SIZE) / TILE_SIZE;

	controls->mouseTileY = (controls->mousePositionInWorldY - controls->mousePositionInWorldY % TILE_SIZE) / TILE_SIZE;

	return Jtrue;
}

void Inputs_ApplyInputs( Controls* controls, int delta,
                            Jbool* game_started,
                            World* world)
{
    Entity* player = &world->player;
    Vector* bullets_vector = &world->bullets_vector;
    //Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    //Vector* explosions_vector = &world->explosions_vector;
    Entity* map = world->map;

    controls->timer_menu -= delta;

    //show menu
    if(controls->timer_menu <= 0 && controls->pressedKeys[SDL_SCANCODE_ESCAPE] == Jtrue)
    {   if(*game_started)
        {
           *game_started = Jfalse;
        }
        else
        {
            *game_started = Jtrue;
        }
        controls->timer_menu  = 100;
    }

    if(*game_started && controls->timer_menu < 0)
    {

        player->dx = 0;
        player->dy = 0;
        if (controls->pressedKeys[SDL_SCANCODE_W] == Jtrue)
        {
            player->dy = -1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDL_SCANCODE_S] == Jtrue)
        {
            player->dy = 1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDL_SCANCODE_D] == Jtrue)
        {
            player->dx = 1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDL_SCANCODE_A] == Jtrue)
        {
            player->dx = -1 * player->speed * delta;
        }

        if (player->dx != 0 && player->dy != 0)
        {
            player->dx *= 0.707106781;
            player->dy *= 0.707106781;
        }

        /*if (controls->pressedKeys[SDL_SCANCODE_H] == Jtrue)
        {
            FILE *save_file;
            save_file = fopen("test.txt", "wb");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fwrite(&world->map[i], sizeof(Entity), 1, save_file);
            }

            fclose(save_file);
        }

        if (controls->pressedKeys[SDL_SCANCODE_G] == Jtrue)
        {
            FILE *save_file;
            save_file = fopen("test.txt", "rb");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fread(&world->map[i], sizeof(Entity), 1, save_file);
            }

            fclose(save_file);
        }*/

        /*float adjacent = controls->mousePositionInWorldX  - player->x;
        float opposite = controls->mousePositionInWorldY - player->y;
        float angle_to_mouse = atan2f(opposite, adjacent);*/
        float angle_to_mouse = C_AngleBetween2Points(
                                        player->x,
                                        player->y,
                                        controls->mousePositionInWorldX,
                                        controls->mousePositionInWorldY);

        player->muzzleX = player->x + 10;
        player->muzzleY = player->y + 10;
        player->angle = angle_to_mouse;


        //adjacent = controls->mousePositionInWorldX  - player->muzzleX - 5;
        //opposite = controls->mousePositionInWorldY - player->muzzleY - 5;
        float angle_from_muzzle_to_mouse = C_AngleBetween2Points(
                                        player->muzzleX,
                                        player->muzzleY,
                                        controls->mousePositionInWorldX,
                                        controls->mousePositionInWorldY  );


        if(controls->mouseWheelPos != 0)
        {
            WeaponsComponent_ScrollWeapons(player->weapons_component, controls->mouseWheelPos );
        }

        if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
        {
            WeaponsComponent_TryToShoot(player->weapons_component,
                                          player->muzzleX,
                                          player->muzzleY,
                                          angle_from_muzzle_to_mouse,
                                          bullets_vector,
                                          controls->mousePositionInWorldX,
                                          controls->mousePositionInWorldY);
        }


        if (controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == Jtrue)
        {
            if (controls->mouseTileX < world->map_width && controls->mouseTileX > 0 &&
                controls->mouseTileY < world->map_height && controls->mouseTileY > 0)
            {
                //converting tile position to real position on screen
                int tileInPixelsX = controls->mouseTileX * TILE_SIZE;
                int tileInPixelsY = controls->mouseTileY * TILE_SIZE;

                Entity* wall = (Entity*)malloc(sizeof(Entity));
                wall = Wall_Create(tileInPixelsX, tileInPixelsY);

                int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;

                map[position_in_array] = *wall;
                printf("Created wall at %d:%d\n", tileInPixelsX, tileInPixelsY);

            }
            else
                printf("out of bound!!!");
        }

        if(controls->pressedKeys[SDL_SCANCODE_R])
        {
            WeaponsComponent_Reload(player->weapons_component, delta);
        }

        if(SDL_GetTicks() - player->last_creation > 50)
        {
            if (controls->pressedKeys[SDL_SCANCODE_C] == Jtrue)
            {
                Entity* zombie;
                zombie = CreateZombie(Normal_Zombie, controls->mousePositionInWorldX , controls->mousePositionInWorldY);
                Vector_Push(monsters_vector, zombie);


            }

            player->last_creation = SDL_GetTicks();
        }
    }
}

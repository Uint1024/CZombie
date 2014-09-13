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


Jbool PoolInputs(Controls* controls, Entity* camera)
{
    controls->mouseWheelPos = 0;
	while (SDL_PollEvent(&controls->e))
	{
		if (controls->e.type == SDL_QUIT || controls->e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
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

void ProcessInputs(Controls* controls, Entity* player,
	Entity* map, int map_width, int map_height, int delta, Vector* bullets_vector,
	Vector* monsters_vector)
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

	float adjacent = controls->mousePositionInWorldX  - player->x;
	float opposite = controls->mousePositionInWorldY - player->y;
	float angle_to_mouse = atan2f(opposite, adjacent);

    player->muzzleX = player->x + 10;
    player->muzzleY = player->y + 10;

	player->angle = angle_to_mouse;


	adjacent = controls->mousePositionInWorldX  - player->muzzleX - 5;
	opposite = controls->mousePositionInWorldY - player->muzzleY - 5;
	float angle_from_muzzle_to_mouse = atan2f(opposite, adjacent);


    if(controls->mouseWheelPos != 0)
    {
        WeaponsComponent_ScrollWeapons(player->weapons_component, controls->mouseWheelPos );
    }

    if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
    {
        Weapon_TryToShoot(player->weapons_component->current_weapon,
                          player->muzzleX,
                          player->muzzleY,
                          angle_from_muzzle_to_mouse,
                          bullets_vector,
                          controls->mousePositionInWorldX,
                          controls->mousePositionInWorldY
                          );
    }


    if (controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == Jtrue)
    {
        if (controls->mouseTileX < map_width && controls->mouseTileX > 0 &&
            controls->mouseTileY < map_height && controls->mouseTileY > 0)
        {
            //converting tile position to real position on screen
            int tileInPixelsX = controls->mouseTileX * TILE_SIZE;
            int tileInPixelsY = controls->mouseTileY * TILE_SIZE;

            Entity* wall = (Entity*)malloc(sizeof(Entity));
            wall = Wall_Create(tileInPixelsX, tileInPixelsY);

            int position_in_array = controls->mouseTileY * map_width + controls->mouseTileX;

            map[position_in_array] = *wall;

            printf("Created wall at %d:%d\n", tileInPixelsX, tileInPixelsY);

        }
        else
            printf("out of bound!!!");
    }

    if(SDL_GetTicks() - player->last_creation > 50)
    {
        if (controls->pressedKeys[SDL_SCANCODE_C] == Jtrue)
        {
            Entity* zombie = (Entity*)malloc(sizeof(Entity));
            zombie = CreateZombie(controls->mousePositionInWorldX , controls->mousePositionInWorldY, 0.2);
            Vector_Push(monsters_vector, zombie);

        }

        player->last_creation = SDL_GetTicks();
    }
}

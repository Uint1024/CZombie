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

Controls* CreateControls()
{
	Controls* controls = (Controls*)malloc(sizeof(Controls));
	for (int i = 0; i < 200; i++)
	{
		controls->pressedKeys[i] = Jfalse;
	}

	return controls;
}


Jbool PoolInputs(Controls* controls, Entity* camera)
{

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
	}

	SDL_GetMouseState(&controls->mouseX, &controls->mouseY);

	controls->mousePositionInWorldX = controls->mouseX + camera->x;

	controls->mousePositionInWorldY = controls->mouseY + camera->y;
	controls->mouseTileX = (controls->mousePositionInWorldX - controls->mousePositionInWorldX % TILE_SIZE) / TILE_SIZE;

	controls->mouseTileY = (controls->mousePositionInWorldY - controls->mousePositionInWorldY % TILE_SIZE) / TILE_SIZE;

//printf("%f %d\n", camera->x, controls->mouseTileX);
	return Jtrue;
}

void ProcessInputs(Controls* controls, Entity* player, List* bullets,
	Entity* map, int map_width, int map_height, List* monsters, int delta)
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
	float ratio = opposite / adjacent;
	float angle_to_mouse = atan2f(opposite, adjacent);

	player->angle = angle_to_mouse;

    if(player->current_weapon == AutomaticRifle_w)
    {
        if (player->magazine_bullets > 0 &&
            SDL_GetTicks() - player->last_shoot > 35 &&
            controls->pressedMouseButtons[SDL_BUTTON_LEFT] == Jtrue)
        {
            List_push(bullets, Bullet_Create(player->x + 10, player->y + 10, angle_to_mouse, 3));
            player->magazine_bullets -= 1;


            player->last_shoot = SDL_GetTicks();
        }


        if(player->reloading && SDL_GetTicks() - player->last_reload > 1000)
        {
            player->magazine_bullets = player->magazine_max_bullets;
            player->bullets[AutomaticRifle_w] -= player->magazine_max_bullets;
            player->reloading = Jfalse;
        }
    }

    if(player->bullets[AutomaticRifle_w] > 0)
    {
        if(!player->reloading && player->magazine_bullets <= 0)
        {
            player->reloading = Jtrue;
            player->last_reload = SDL_GetTicks();
        }
    }

//printf("%d %d %d %d\n", controls->mouseTileX, controls->mouseTileY, controls->mousePositionInWorldX, controls->mousePositionInWorldY);
    if (controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == Jtrue)
    {
        if (controls->mouseTileX < map_width && controls->mouseTileX > 0 &&
            controls->mouseTileY < map_height && controls->mouseTileY > 0)
        {
            //converting tile position to real position on screen
            int tileInPixelsX = controls->mouseTileX * TILE_SIZE;
            int tileInPixelsY = controls->mouseTileY * TILE_SIZE;

            //"- camera->dx" is "temporary fix"
            //float positionWithCameraX = tileInPixelsX - camera->dx;
            //float positionWithCameraY = tileInPixelsY  - camera->dy;

            Entity* wall = (Entity*)malloc(sizeof(Entity));
            wall = Wall_Create(tileInPixelsX, tileInPixelsY);

            int position_in_array = controls->mouseTileY * map_width + controls->mouseTileX;

            map[position_in_array] = *wall;

            printf("Created wall at %d:%d\n", tileInPixelsX, tileInPixelsY);

        }
        else
            printf("out of bound!!!");
    }

    if(SDL_GetTicks() - player->last_creation > 190)
    {
        if (controls->pressedKeys[SDL_SCANCODE_C] == Jtrue)
        {
            Entity* zombie = (Entity*)malloc(sizeof(Entity));
            zombie = CreateZombie(controls->mousePositionInWorldX , controls->mousePositionInWorldY, 0.05);
            List_push(monsters, zombie);
        }

        player->last_creation = SDL_GetTicks();
    }
}

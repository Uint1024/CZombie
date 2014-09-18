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
#include "window.h"
#include "gameManager.h"

Controls* CreateControls()
{
	Controls* controls = (Controls*)malloc(sizeof(Controls));
	for (int i = 0; i < 200; i++)
	{
		controls->pressedKeys[i] = Jfalse;
	}
    controls->mouseWheelPos = 0;
    controls->active_window = NULL;
    controls->cursor_resize_left_right = Jfalse;
    controls->cursor_resize_up_down = Jfalse;
    controls->active_button = NULL;

    controls->tileInPixelsX = 0;
    controls->tileInPixelsY = 0;
    controls->hovering_on_window = Jfalse;
    controls->last_ai_switch = 0;
	return controls;
}

void Inputs_ProcessInputs(Controls* controls, int delta, Jbool* game_started, World* world, Window* level_editor,
                          GameManager* game_manager)
{
    Inputs_PoolInputs(controls, world->player.camera);
    Inputs_ApplyInputs(controls, delta, game_started, world, level_editor, game_manager);

    for(int i = 0 ; i < 20 ; i++)
        controls->previousPressedMouseButtons[i] = controls->pressedMouseButtons[i];


    controls->previousMouseX = controls->mouseX;
    controls->previousMouseY = controls->mouseY;
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
			controls->pressedKeys[controls->e.key.keysym.sym] = Jtrue;
			controls->pressedMods[controls->e.key.keysym.mod] = Jtrue;
		}

		if (controls->e.type == SDL_KEYUP)
		{
			controls->pressedKeys[controls->e.key.keysym.sym] = Jfalse;
			controls->pressedMods[controls->e.key.keysym.mod] = Jfalse;
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
                            World* world, Window* level_editor,
                            GameManager* gm)
{
    Entity* player = &world->player;
    Vector* bullets_vector = &world->bullets_vector;
    //Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    //Vector* explosions_vector = &world->explosions_vector;
    Entity** map = world->map;

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


    if(BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &level_editor->box))
    {
       controls->hovering_on_window = Jtrue;

            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
            {
                for(int i = 0 ; i < NB_OF_LEVEL_EDITOR_BUTTONS ; i++)
                {
                    if(BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &level_editor->buttons[i].box))
                    {

                            controls->active_button = &level_editor->buttons[i];

                    }
                }
            }

        if(controls->mouseX > level_editor->box.right - 10 ||
           controls->mouseX < level_editor->box.left + 10 &&
           controls->active_window == NULL)
        {
            controls->cursor_resize_left_right = Jtrue;

            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->mouseX > level_editor->box.right - 10)
            {
                controls->resizing_right = Jtrue;
            }

            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->mouseX < level_editor->box.left + 10)
            {
                controls->resizing_left = Jtrue;
            }
        }
        else
        {
            controls->cursor_resize_left_right = Jfalse;
        }

       if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
            controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
            !controls->cursor_resize_left_right)
        {
            controls->active_window = level_editor;
        }
    }
    else
    {
        controls->cursor_resize_left_right = Jfalse;
        controls->cursor_resize_up_down = Jfalse;
        controls->hovering_on_window = Jfalse;
    }


    if(controls->active_window != NULL &&
       !controls->previousPressedMouseButtons[SDL_BUTTON_LEFT])
    {
        controls->active_window = NULL;
    }

    if(controls->resizing_right || controls->resizing_left)
    {
        controls->cursor_resize_left_right = Jtrue;

        if(!controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
           !controls->previousPressedMouseButtons[SDL_BUTTON_LEFT])
        {
            controls->resizing_right = Jfalse;
            controls->resizing_left = Jfalse;
        }

        if(controls->resizing_right)
        {
            Window_ResizeRight(level_editor, controls->mouseX - controls->previousMouseX);
        }
        else if(controls->resizing_left)
        {
            Window_ResizeLeft(level_editor, controls->mouseX - controls->previousMouseX);
        }

    }

    if(controls->active_window != NULL &&
       controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
       controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
       !controls->resizing_left && !controls->resizing_right)
    {

         Window_Move(level_editor,
                     controls->mouseX - controls->previousMouseX,
                     controls->mouseY - controls->previousMouseY);
    }

    if(*game_started && controls->timer_menu < 0)
    {

        player->dx = 0;
        player->dy = 0;
        if (controls->pressedKeys[SDLK_z] == Jtrue)
        {
            player->dy = -1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDLK_s] == Jtrue)
        {
            player->dy = 1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDLK_d] == Jtrue)
        {
            player->dx = 1 * player->speed * delta;
        }
        if (controls->pressedKeys[SDLK_q] == Jtrue)
        {
            player->dx = -1 * player->speed * delta;
        }

        if (player->dx != 0 && player->dy != 0)
        {
            player->dx *= 0.707106781;
            player->dy *= 0.707106781;
        }



        float angle_to_mouse = C_AngleBetween2Points(
                                        player->x,
                                        player->y,
                                        controls->mousePositionInWorldX,
                                        controls->mousePositionInWorldY);

        player->muzzleX = player->x + 10;
        player->muzzleY = player->y + 10;
        player->angle = angle_to_mouse;

        float angle_from_muzzle_to_mouse = C_AngleBetween2Points(
                                        player->muzzleX,
                                        player->muzzleY,
                                        controls->mousePositionInWorldX,
                                        controls->mousePositionInWorldY  );


        if(!controls->active_window && !controls->hovering_on_window)
        {
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

            controls->tileInPixelsX = controls->mouseTileX * TILE_SIZE;
            controls->tileInPixelsY = controls->mouseTileY * TILE_SIZE;
            int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;
            if (controls->active_button != NULL &&
                controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == Jtrue)
            {
                if (controls->mouseTileX < world->map_width && controls->mouseTileX > 0 &&
                    controls->mouseTileY < world->map_height && controls->mouseTileY > 0)
                {
                    //converting tile position to real position on screen





                    if(controls->active_button->button_type == Button_Wall_Normal)
                    {
                        world->map[position_in_array] = Wall_Create(Wall_Normal, controls->tileInPixelsX, controls->tileInPixelsY);
                    }

                    if(controls->active_button->button_type == DirtGround_button)
                    {
                        world->ground_map[position_in_array] = Ground_Create(Ground_Dirt, controls->tileInPixelsX, controls->tileInPixelsY);
                    }
                    else if(controls->active_button->button_type == GrassGround_button)
                    {
                        world->ground_map[position_in_array] = Ground_Create(Ground_Grass, controls->tileInPixelsX, controls->tileInPixelsY);
                    }

                    if(controls->active_button->main_category == Cat_Zombie &&
                       (SDL_GetTicks() - player->last_creation > 150 || controls->pressedKeys[SDL_SCANCODE_LCTRL]))
                    {
                        Vector_Push(monsters_vector,
                                    CreateZombie(button_object_type_g[controls->active_button->button_type],
                                                 controls->mousePositionInWorldX, controls->mousePositionInWorldY));

                        player->last_creation = SDL_GetTicks();
                    }
                }
                else
                    printf("out of bound!!!");
            }



            if(controls->pressedKeys[SDLK_r])
            {
                WeaponsComponent_Reload(player->weapons_component, delta);
            }

            if(controls->pressedKeys[SDLK_x])
            {
                for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
                {
                    Entity* mob = (Entity*)Vector_Get(&world->monsters_vector, i);
                    if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                       controls->mousePositionInWorldY,
                                                       &mob->box))
                    {
                        //mob->alive = Jfalse;
                        Vector_Delete(&world->monsters_vector, i);
                    }
                }
            }

            if(controls->pressedKeys[SDLK_c])
            {
                world->map[position_in_array] = NULL;
            }

            if(controls->pressedKeys[SDLK_e])
            {
                controls->active_button = NULL;
            }

            if(controls->pressedKeys[SDLK_a] &&
               SDL_GetTicks() - controls->last_ai_switch > 150)
            {
                if(gm->ai_on)
                {
                    gm->ai_on = Jfalse;
                }
                else
                {
                    gm->ai_on = Jtrue;
                }

                controls->last_ai_switch = SDL_GetTicks();

            }

            if(!world->player.running &&
               controls->pressedKeys[SDL_SCANCODE_LSHIFT])
            {
                Player_StartRunning(&world->player);
            }

            if(world->player.running &&
               !controls->pressedKeys[SDL_SCANCODE_LSHIFT])
            {
                Player_StopRunning(&world->player);
            }
        }


    }

}

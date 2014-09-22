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
#include "player.h"
#include "door.h"
#include "mapEvent.h"

    Jbool previousPressedKeys_g[200] = {Jfalse};
static int switch_timer = 0;
static int building_time = 0;
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

void Inputs_ProcessInputs(Controls* controls, World* world, Window* level_editor,
                          GameManager* game_manager)
{
    Inputs_PoolInputs(controls, world->player.camera);
    Inputs_ApplyInputs(controls, world, level_editor, game_manager);

    for(int i = 0 ; i < 20 ; i++)
        controls->previousPressedMouseButtons[i] = controls->pressedMouseButtons[i];



    controls->previousMouseX = controls->mouseX;
    controls->previousMouseY = controls->mouseY;
}

void Inputs_ApplyInputsLevelEditor(Controls* controls,
                            World* world, Window* level_editor,
                            GameManager* gm)
{
    if(controls->pressedKeys[SDLK_F7] && SDL_GetTicks() - switch_timer > 200)
    {

            switch_timer = SDL_GetTicks();
            game_state_g = GameState_Map_Editor_Testing_Level;

            //where the fuck do I put this? in gamemanager? wtf is gamemanager anyway
            Game_StartMap(world);
            world->player.speed = BASE_PLAYER_SPEED;
            Level_Save("saves/tempLevelEditor.sav", world);
    }


    int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;

    Entity* player = &world->player;

    Vector* monsters_vector = &world->monsters_vector;
    if(BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &level_editor->box))
    {
       controls->hovering_on_window = Jtrue;

        //selecting button in level_editor
        if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
        {
            for(int i = 0 ; i < level_editor->nb_of_buttons ; i++)
            {
                if(BoundingBox_CheckPointCollision(controls->mouseX,
                                                   controls->mouseY,
                                                   &level_editor->buttons[i].box))
                {
                    controls->active_button = &level_editor->buttons[i];
                }
            }
        }

        //showing window resizing cursor
        if((controls->mouseX > level_editor->box.right - 10 ||
           controls->mouseX < level_editor->box.left + 10) &&
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

        //clicking on the level editor window
       if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
            controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
            !controls->cursor_resize_left_right)
        {
            controls->active_window = level_editor;
        }
    }
    else //if not hovering on the window
    {
        controls->cursor_resize_left_right = Jfalse;
        controls->cursor_resize_up_down = Jfalse;
        controls->hovering_on_window = Jfalse;

    }

    //if you've clicked on the window and then stopped clicking, it's not "active" anymore
    if(controls->active_window != NULL &&
       !controls->previousPressedMouseButtons[SDL_BUTTON_LEFT])
    {
        controls->active_window = NULL;
    }

    //resizing window
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

    //moving window
    if(controls->active_window != NULL &&
       controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
       controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
       !controls->resizing_left && !controls->resizing_right)
    {

         Window_Move(level_editor,
                     controls->mouseX - controls->previousMouseX,
                     controls->mouseY - controls->previousMouseY);
    }

    //if not hovering and not moving window
    //we can create/delete stuff on the map
    if(!controls->active_window && !controls->hovering_on_window)
    {
        if (controls->active_button != NULL &&
            controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == Jtrue)
        {
            if (controls->mouseTileX < world->map_width && controls->mouseTileX > 0 &&
                controls->mouseTileY < world->map_height && controls->mouseTileY > 0)
            {
                //creating object
                //TODO : merge all the X_Create into one big function that calls all the others
                int obj_type = controls->active_button->button_type;
                int x = controls->tileInPixelsX;
                int y = controls->tileInPixelsY;
                Main_Category category = controls->active_button->main_category;

                if(category == Cat_Wall)
                {
                    world->map[position_in_array] = Wall_Create(obj_type, x, y);
                }

                else if(category == Cat_Door)
                {
                    world->map[position_in_array] = Door_Create(obj_type, x, y);
                }

                else if(category == Cat_Ground)
                {
                    world->ground_map[position_in_array] = Ground_Create(obj_type, x, y);
                }

                else if(category == Cat_Zombie &&
                   (SDL_GetTicks() - building_time > 150 || controls->pressedKeys[SDL_SCANCODE_LCTRL]))
                {
                    Vector_Push(monsters_vector,
                                CreateZombie(obj_type, controls->mousePositionInWorldX, controls->mousePositionInWorldY));

                    building_time = SDL_GetTicks();
                }
                else if(category == Cat_Event &&
                        SDL_GetTicks() - building_time > 150)
                {
                    //there can be only 1 player start and end of level
                    for(int i = 0 ; i < Vector_Count(&world->events_vector) ; i++)
                    {

                        Entity* map_event = (Entity*)Vector_Get(&world->events_vector, i);

                        printf("%d in vector, trying to create %d, object in vector is %d\n", Vector_Count(&world->events_vector), obj_type, map_event->sub_category);
                        if((obj_type == Event_Player_Start || obj_type == Event_End_Level )&&
                           obj_type == map_event->sub_category)
                        {

                            Vector_Delete(&world->events_vector, i);
                            printf("deleted %d, %d left\n", i, Vector_Count(&world->events_vector));
                        }
                    }

                    Vector_Push(&world->events_vector, MapEvent_Create(obj_type, x, y, TILE_SIZE, TILE_SIZE));
                    building_time = SDL_GetTicks();
                }
            }
            else
                printf("out of bound!!!");
        }

        //deleting monster
        if(controls->pressedKeys[SDLK_x])
        {
            for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
            {
                Entity* mob = (Entity*)Vector_Get(&world->monsters_vector, i);
                if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                   controls->mousePositionInWorldY,
                                                   &mob->box))
                {
                    Vector_Delete(&world->monsters_vector, i);

                }
            }
        }

        //deleting every monster
        if(controls->pressedKeys[SDLK_u])
        {
            Vector_Clear(&world->monsters_vector);
        }

        //remove wall (replace it with empty entity)
        if(controls->pressedKeys[SDLK_c])
        {
            world->map[position_in_array] = Entity_Spawn();
        }

        //cancel current selected object
        if(controls->pressedKeys[SDLK_e])
        {
            controls->active_button = NULL;
        }



        //switch mobs AI
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
    }
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

    controls->tileInPixelsX = controls->mouseTileX * TILE_SIZE;
    controls->tileInPixelsY = controls->mouseTileY * TILE_SIZE;


	return Jtrue;
}

//wow this is a mess
void Inputs_ApplyInputs( Controls* controls,
                            World* world, Window* level_editor,
                            GameManager* gm)
{
    Entity* player = &world->player;
    Vector* bullets_vector = &world->bullets_vector;
    //Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    //Vector* explosions_vector = &world->explosions_vector;

    controls->timer_menu -= delta_g;


    //press ESC to show menu
    if(controls->timer_menu <= 0 && controls->pressedKeys[SDLK_ESCAPE] == Jtrue)
    {   if(display_menu_g)
        {
           display_menu_g = Jfalse;
        }
        else
        {
            display_menu_g = Jtrue;
        }
        controls->timer_menu  = 100;
    }

    if(!display_menu_g && controls->timer_menu < 0)
    {

        /*
        PLAYER CONTROL STUFF
        */

        /*MOVEMENT*/
        //cancel current velocity
        player->dx = 0;
        player->dy = 0;

        //determine if running
        if(!world->player.running &&
               controls->pressedKeys[SDL_SCANCODE_LSHIFT])
        {
            Player_StartRunning(&world->player);
        }
        else if(world->player.running &&
               !controls->pressedKeys[SDL_SCANCODE_LSHIFT])
        {
            Player_StopRunning(&world->player);
        }

        //apply controls to player movement
        if (controls->pressedKeys[SDLK_z] == Jtrue)
        {
            player->dy = -1 * player->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_s] == Jtrue)
        {
            player->dy = 1 * player->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_d] == Jtrue)
        {
            player->dx = 1 * player->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_q] == Jtrue)
        {
            player->dx = -1 * player->speed * delta_g;
        }

        if (player->dx != 0 && player->dy != 0)
        {
            player->dx *= 0.707106781;
            player->dy *= 0.707106781;
        }


        if(game_state_g != GameState_Editing_Map)
        {
            //reloading
            if(controls->pressedKeys[SDLK_r])
            {
                WeaponsComponent_Reload(player->weapons_component);
            }

            //calculating angle to mouse
            float muzzleX = 0;
            float muzzleY = 0;
            Entity_GetMiddleCoordinates(player, &muzzleX, &muzzleY);
            float mouse_angle = C_AngleBetween2Points(
                                            muzzleX,
                                            muzzleY,
                                            controls->mousePositionInWorldX,
                                            controls->mousePositionInWorldY  );
            player->angle = mouse_angle;


            //position of the targeted position in the map array
            int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;


            //opening/closing doors
            if(controls->pressedKeys[SDLK_SPACE])
            {
                if(world->map[position_in_array]->t == Cat_Door &&
                   Entity_CheckDistance(player, world->map[position_in_array], 60))
                {
                    Door_Switch(world->map[position_in_array]);
                }
            }

            //weapon scrolling
            if(controls->mouseWheelPos != 0)
            {
                WeaponsComponent_ScrollWeapons(player->weapons_component, controls->mouseWheelPos );
            }

            //shooting
            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
            {
                WeaponsComponent_TryToShoot(player->weapons_component,
                                              muzzleX,
                                              muzzleY,
                                              mouse_angle,
                                              bullets_vector,
                                              controls->mousePositionInWorldX,
                                              controls->mousePositionInWorldY);
            }
        }


        /* LEVEL EDITOR STUFF */
        if(game_state_g == GameState_Editing_Map)
        {
            Inputs_ApplyInputsLevelEditor(controls, world, level_editor, gm);
        }

        if(controls->pressedKeys[SDLK_F7] && SDL_GetTicks() - switch_timer > 200)
        {
            if(game_state_g == GameState_Map_Editor_Testing_Level)
            {
                switch_timer = SDL_GetTicks();
                game_state_g = GameState_Editing_Map;
                world->player.visible = Jfalse;
                world->player.solid = Jfalse;
                Level_Load("saves/tempLevelEditor.sav", world);
            }
        }
    }
}

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
#include "levelEditor.h"
#include "movement_component.h"

    bool previousPressedKeys_g[200] = {false};
static int switch_timer = 0;
static int building_time = 0;
static int deadzone = 9000;
static SDL_Joystick* controller;
bool using_controller_g = true;
static int controller_leftAxisX;
static int controller_leftAxisY;
static int controller_rightAxisX;
static int controller_rightAxisY;
static int controller_leftTrigger;
static int controller_rightTrigger;

Controls* CreateControls()
{


	Controls* controls = (Controls*)malloc(sizeof(Controls));
	for (int i = 0; i < 200; i++)
	{
		controls->pressedKeys[i] = false;
	}
    controls->mouseWheelPos = 0;
    controls->active_window = NULL;
    controls->cursor_resize_left_right = false;
    controls->cursor_resize_up_down = false;
    controls->active_button = NULL;

    controls->tileInPixelsX = 0;
    controls->tileInPixelsY = 0;
    controls->hovering_on_window = false;
    controls->last_ai_switch = 0;
    controls->temp_object_to_create = NULL;
    controller = SDL_JoystickOpen(0);
	return controls;
}

void Inputs_ProcessInputs(Controls* controls, World* world, Window* level_editor,
                          GameManager* game_manager)
{
    Inputs_PoolInputs(controls, world->player.playerC);
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
        LevelEditor_QuickTry(world);
    }


    int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;

    if(controls->temp_object_to_create != NULL)
    {

        controls->temp_object_to_create->x = controls->mouseX;
        controls->temp_object_to_create->y = controls->mouseY;
        BoundingBox_Update(controls->temp_object_to_create);
    }

    Vector* monsters_vector = &world->monsters_vector;
    if(BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &level_editor->box))
    {
       controls->hovering_on_window = true;

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

                    if(controls->temp_object_to_create != NULL)
                    {
                        Entity_Destroy(controls->temp_object_to_create);
                        free(controls->temp_object_to_create);
                    }

                    controls->temp_object_to_create = Entity_Create(controls->active_button->main_category,
                                                                    controls->active_button->button_type,
                                                                    controls->mouseX, controls->mouseY,
                                                                    0);
                }
            }
        }

        //showing window resizing cursor
        if((controls->mouseX > level_editor->box.right - 10 ||
           controls->mouseX < level_editor->box.left + 10) &&
           controls->active_window == NULL)
        {
            controls->cursor_resize_left_right = true;

            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->mouseX > level_editor->box.right - 10)
            {
                controls->resizing_right = true;
            }

            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->previousPressedMouseButtons[SDL_BUTTON_LEFT] &&
               controls->mouseX < level_editor->box.left + 10)
            {
                controls->resizing_left = true;
            }
        }
        else
        {
            controls->cursor_resize_left_right = false;
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
        controls->cursor_resize_left_right = false;
        controls->cursor_resize_up_down = false;
        controls->hovering_on_window = false;

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
        controls->cursor_resize_left_right = true;

        if(!controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
           !controls->previousPressedMouseButtons[SDL_BUTTON_LEFT])
        {
            controls->resizing_right = false;
            controls->resizing_left = false;
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
            controls->pressedMouseButtons[SDL_BUTTON_RIGHT] == true)
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

                if(category == Cat_Wall || category == Cat_Door)
                {
                    Entity_Destroy(world->map[position_in_array]);
                    free(world->map[position_in_array]);
                    world->map[position_in_array] = Entity_Create(category, obj_type, x, y, 0);
                }

                else if(category == Cat_Ground)
                {
                    Entity_Destroy(world->ground_map[position_in_array]);
                    free(world->ground_map[position_in_array]);
                    world->ground_map[position_in_array] = Entity_Create(category, obj_type, x, y, 0);
                }

                else if(category == Cat_Zombie &&
                   (SDL_GetTicks() - building_time > 150 || controls->pressedKeys[SDL_SCANCODE_LCTRL]))
                {
                    Vector_Push(monsters_vector,
                               Entity_Create(category, obj_type,
                                             controls->mousePositionInWorldX,
                                             controls->mousePositionInWorldY,
                                             0));

                    building_time = SDL_GetTicks();

                    printf("%d\n", Vector_Count(monsters_vector));
                }
                else if(category == Cat_Event &&
                        SDL_GetTicks() - building_time > 150)
                {
                    //there can be only 1 player start and end of level
                    for(int i = 0 ; i < Vector_Count(&world->events_vector) ; i++)
                    {
                        Entity* map_event = (Entity*)Vector_Get(&world->events_vector, i);
                        if((obj_type == Event_Player_Start || obj_type == Event_End_Level )&&
                           obj_type == map_event->sub_category)
                        {
                            Vector_Delete(&world->events_vector, i);
                        }
                    }
                    Vector_Push(&world->events_vector, Entity_Create(category, obj_type, x, y, 0));
                    building_time = SDL_GetTicks();
                }
                else if(category == Cat_Bonus && SDL_GetTicks() - building_time > 150)
                {
                    Vector_Push(&world->bonus_vector, Entity_Create(category, obj_type, x, y, 0));
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
           // Vector_Clear(&world->monsters_vector); leads to memory leak
        }

        //remove wall (replace it with empty entity)
        if(controls->pressedKeys[SDLK_c])
        {
            Entity_Destroy(world->map[position_in_array]);
            world->map[position_in_array] = Wall_CreateEmpty();
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
                gm->ai_on = false;
            }
            else
            {
                gm->ai_on = true;
            }

            controls->last_ai_switch = SDL_GetTicks();
        }
    }
}


bool Inputs_PoolInputs(Controls* controls, PlayerC* playerC)
{
    float cameraX = playerC->cameraX;
    float cameraY = playerC->cameraY;

    controls->mouseWheelPos = 0;



	while (SDL_PollEvent(&controls->e))
	{
		if (controls->e.type == SDL_QUIT || controls->e.key.keysym.scancode == SDL_SCANCODE_F4)
		{
			return false;
		}

		if(controls->e.key.keysym.scancode == SDL_SCANCODE_P)
		{
			debug_mode = true;
		}

        if(controls->e.key.keysym.scancode == SDL_SCANCODE_O)
		{
			debug_mode = false;
		}

		if (controls->e.type == SDL_KEYDOWN)
		{
			controls->pressedKeys[controls->e.key.keysym.sym] = true;
			controls->pressedMods[controls->e.key.keysym.mod] = true;
		}

		if (controls->e.type == SDL_KEYUP)
		{
			controls->pressedKeys[controls->e.key.keysym.sym] = false;
			controls->pressedMods[controls->e.key.keysym.mod] = false;
		}

		if (controls->e.type == SDL_MOUSEBUTTONDOWN)
		{
			controls->pressedMouseButtons[controls->e.button.button] = true;
		}
		if (controls->e.type == SDL_MOUSEBUTTONUP)
		{
			controls->pressedMouseButtons[controls->e.button.button] = false;
		}

		if(controls->e.type == SDL_MOUSEWHEEL)
        {
            controls->mouseWheelPos += controls->e.wheel.y;
        }
	}

	SDL_GetMouseState(&controls->mouseX, &controls->mouseY);

	controls->mousePositionInWorldX = controls->mouseX + cameraX;

	controls->mousePositionInWorldY = controls->mouseY + cameraY;
	controls->mouseTileX = (controls->mousePositionInWorldX - controls->mousePositionInWorldX % TILE_SIZE) / TILE_SIZE;

	controls->mouseTileY = (controls->mousePositionInWorldY - controls->mousePositionInWorldY % TILE_SIZE) / TILE_SIZE;

    controls->tileInPixelsX = controls->mouseTileX * TILE_SIZE;
    controls->tileInPixelsY = controls->mouseTileY * TILE_SIZE;


	return true;
}

//wow this is a mess
void Inputs_ApplyInputs( Controls* controls,
                            World* world, Window* level_editor,
                            GameManager* gm)
{
    Entity* player = &world->player;
    Vector* bullets_vector = &world->bullets_vector;
    //Vector* bonus_vector = &world->bonus_vector;
    //Vector* monsters_vector = &world->monsters_vector;
    //Vector* explosions_vector = &world->explosions_vector;

    controls->timer_menu -= delta_g;


    //press ESC to show menu
    if(controls->timer_menu <= 0 && controls->pressedKeys[SDLK_ESCAPE] == true ||
       SDL_JoystickGetButton(controller, BUTTON_START))
    {
        if(display_menu_g)
        {
           display_menu_g = false;
        }
        else
        {
            display_menu_g = true;
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
        player->movementC->dx = 0;
        player->movementC->dy = 0;

        //determine if running
        if(!world->player.playerC->running &&
               controls->pressedKeys[SDL_SCANCODE_LSHIFT])
        {
            Player_StartRunning(&world->player);
        }
        else if(world->player.playerC->running &&
               !controls->pressedKeys[SDL_SCANCODE_LSHIFT])
        {
            Player_StopRunning(&world->player);
        }

        //apply keyboard controls to player movement
        if (controls->pressedKeys[SDLK_z] == true)
        {
            player->movementC->dy = -1 * player->movementC->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_s] == true)
        {
            player->movementC->dy = 1 * player->movementC->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_d] == true)
        {
            player->movementC->dx = 1 * player->movementC->speed * delta_g;
        }
        if (controls->pressedKeys[SDLK_q] == true)
        {
            player->movementC->dx = -1 * player->movementC->speed * delta_g;
        }

        if (player->movementC->dx != 0 && player->movementC->dy != 0)
        {
            player->movementC->dx *= 0.707106781;
            player->movementC->dy *= 0.707106781;
        }

        //apply controller controls
        if(using_controller_g)
        {
            controller_leftAxisX = SDL_JoystickGetAxis(controller, 0);
            controller_leftAxisY = SDL_JoystickGetAxis(controller, 1);
            controller_rightAxisX = SDL_JoystickGetAxis(controller, 2);
            controller_rightAxisY = SDL_JoystickGetAxis(controller, 3);
            controller_leftTrigger = SDL_JoystickGetAxis(controller, 4);
            controller_rightTrigger = SDL_JoystickGetAxis(controller, 5);

            if(controller_leftAxisX < -deadzone || controller_leftAxisX > deadzone)
            {
                player->movementC->dx = ((float)controller_leftAxisX / 100000.0f) * delta_g;
            }
            if(controller_leftAxisY < -deadzone || controller_leftAxisY > deadzone)
            {
                player->movementC->dy = ((float)controller_leftAxisY / 100000.0f) * delta_g;
            }

            if(controller_rightAxisX > deadzone || controller_rightAxisX < -deadzone ||
               controller_rightAxisY > deadzone || controller_rightAxisY < -deadzone)
            {

                player->movementC->angle = atan2(controller_rightAxisY, controller_rightAxisX);
            }

            if(SDL_JoystickGetButton(controller, BUTTON_A))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_Handgun);
            }
            if(SDL_JoystickGetButton(controller, BUTTON_B))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_AutomaticRifle);
            }
            if(SDL_JoystickGetButton(controller, BUTTON_X))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_Shotgun);
            }
            if(SDL_JoystickGetButton(controller, BUTTON_Y))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_GrenadeLauncher);
            }
            if(SDL_JoystickGetButton(controller, BUTTON_LEFT_SHOULDER))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_TheBigGun);
            }

        }

        if(game_state_g != GameState_Editing_Map)
        {
            //reloading
            if(reloading_g)
            {
                if(controls->pressedKeys[SDLK_r])
                {
                    WeaponsComponent_Reload(player->weaponsC);
                }
            }


            float muzzleX = 0;
            float muzzleY = 0;
            Entity_GetMiddleCoordinates(player, &muzzleX, &muzzleY);


            if(!using_controller_g)
            {
                //calculating angle to mouse
                float mouse_angle = C_AngleBetween2Points(
                                                muzzleX,
                                                muzzleY,
                                                controls->mousePositionInWorldX,
                                                controls->mousePositionInWorldY  );
                player->movementC->angle = mouse_angle;
            }


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
                WeaponsComponent_ScrollWeapons(player->weaponsC, controls->mouseWheelPos );
            }

            //shooting
            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] ||
               controller_rightTrigger > 0)
            {
                WeaponsComponent_TryToShoot(player->weaponsC,
                                              muzzleX,
                                              muzzleY,
                                              player->movementC->angle,
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

                LevelEditor_BackToEditing(world);
            }
        }
    }
}

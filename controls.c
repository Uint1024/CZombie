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

Uint32 bullet_time_timer = 0;

    bool previousPressedKeys_g[200] = {false};
static int switch_timer = 0;

static int deadzone = 9000;
static SDL_Joystick* controller;
bool using_controller_g = true;
static int controller_leftAxisX;
static int controller_leftAxisY;
static int controller_rightAxisX;
static int controller_rightAxisY;
static int controller_leftTrigger;
static int controller_rightTrigger;

static bool rectangle_selection_create  = false;
static bool rectangle_selection_delete = false;
static int rectangle_selection_startX;
static int rectangle_selection_startY;
static int rectangle_selection_endX;
static int rectangle_selection_endY;

static bool unlimited_creation = false;
static bool pressedKeys[200] = {false};

//put that in a UI file
static int aimPosX = 0;
static int aimPosY = 0;
static Cursor_Type cursor_type;

static Uint32 rotate_timer = 0;
static float rotation_angle = 0;

Controls* CreateControls()
{


	Controls* controls = (Controls*)malloc(sizeof(Controls));


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


		for (int i = 0; i < 20; i++)
	{
		controls->pressedMouseButtons[i] = false;
	}
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
    rotate_timer += delta_g;

    if(pressedKeys[SDLK_F1] && SDL_GetTicks() - switch_timer > 200)
    {
        draw_grid_g = draw_grid_g? false : true;
    }

    if(pressedKeys[SDLK_F7] && SDL_GetTicks() - switch_timer > 2000)
    {
        switch_timer = SDL_GetTicks();
        LevelEditor_QuickTry(world);
    }


    int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;

    if(controls->temp_object_to_create != NULL)
    {
        if(pressedKeys[SDLK_f] && !previousPressedKeys_g[SDLK_f])
        {
            SDL_WarpMouseInWindow(Graphics_GetWindow(),
                                  (controls->mouseTileX * TILE_SIZE - world->player.playerC->cameraX),
                                  controls->mouseTileY * TILE_SIZE - world->player.playerC->cameraY);
        }
        if(pressedKeys[SDLK_RIGHT] && !previousPressedKeys_g[SDLK_RIGHT])
        {
            SDL_WarpMouseInWindow(Graphics_GetWindow(),
                                  (controls->mouseX + 1 ),
                                  controls->mouseY);
        }
        if(pressedKeys[SDLK_LEFT] && !previousPressedKeys_g[SDLK_LEFT])
        {
            SDL_WarpMouseInWindow(Graphics_GetWindow(),
                                  (controls->mouseX - 1 ),
                                  controls->mouseY);
        }
        if(pressedKeys[SDLK_UP] && !previousPressedKeys_g[SDLK_UP])
        {
            SDL_WarpMouseInWindow(Graphics_GetWindow(),
                                  (controls->mouseX ),
                                  controls->mouseY - 1);
        }
        if(pressedKeys[SDLK_DOWN] && !previousPressedKeys_g[SDLK_DOWN])
        {
            SDL_WarpMouseInWindow(Graphics_GetWindow(),
                                  (controls->mouseX),
                                  controls->mouseY + 1);
        }

        if(controls->mouseWheelPos < 0 && rotate_timer > 50)
        {
            if(controls->temp_object_to_create->angle == 0)
            {
                controls->temp_object_to_create->angle = HALF_PI;
            }
            else if(controls->temp_object_to_create->angle <= HALF_PI + 0.1f)
            {
                controls->temp_object_to_create->angle = HALF_PI * 2;
            }
            else if(controls->temp_object_to_create->angle <= HALF_PI * 2 + 0.1f)
            {
                controls->temp_object_to_create->angle = HALF_PI * 3;
            }
            else if(controls->temp_object_to_create->angle <= HALF_PI * 3 + 0.1f)
            {
                controls->temp_object_to_create->angle = 0;
            }

            rotate_timer = 0;
        }
        if(controls->mouseWheelPos > 0 && rotate_timer > 50)
        {
            if(controls->temp_object_to_create->angle == 0)
            {
                controls->temp_object_to_create->angle = HALF_PI * 3;
            }
            else if(controls->temp_object_to_create->angle >= HALF_PI * 3 - 0.1f)
            {
                controls->temp_object_to_create->angle = HALF_PI * 2;
            }
            else if(controls->temp_object_to_create->angle >= HALF_PI * 2 - 0.1f)
            {
                controls->temp_object_to_create->angle = HALF_PI;
            }
            else if(controls->temp_object_to_create->angle >= HALF_PI - 0.2f)
            {
                controls->temp_object_to_create->angle = 0;
            }
            rotate_timer = 0;
        }

        controls->temp_object_to_create->x = controls->mousePositionInWorldX;
        controls->temp_object_to_create->y = controls->mousePositionInWorldY;
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
                    level_editor->active_button = &level_editor->buttons[i];

                    if(controls->temp_object_to_create != NULL)
                    {
                        Entity_Destroy(controls->temp_object_to_create);
                        free(controls->temp_object_to_create);
                    }


                    controls->temp_object_to_create = Entity_Create(level_editor->active_button->main_category,
                                                                    level_editor->active_button->button_type,
                                                                    controls->mouseX, controls->mouseY,
                                                                    0, world);



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

    if(pressedKeys[SDLK_LCTRL])
    {
        unlimited_creation = true;
    }
    else
    {
        unlimited_creation = false;
    }


    //if not hovering and not moving window
    //we can create/delete stuff on the map
    if(!controls->active_window && !controls->hovering_on_window)
    {
        if (level_editor->active_button != NULL)
        {

            Main_Category category = level_editor->active_button->main_category;
            int obj_type = level_editor->active_button->button_type;

            if(!rectangle_selection_create &&
               (controls->pressedMouseButtons[SDL_BUTTON_RIGHT] ||
                pressedKeys[SDLK_SPACE]))
            {
                if (controls->mouseTileX < world->map_width && controls->mouseTileX > 0 &&
                    controls->mouseTileY < world->map_height && controls->mouseTileY > 0)
                {

                    //pressing shift activate the rectangle selection tool
                    if((category == Cat_Wall || category == Cat_Ground) &&
                       pressedKeys[SDLK_LSHIFT])
                    {

                        rectangle_selection_startX = controls->mousePositionInWorldX;
                        rectangle_selection_startY = controls->mousePositionInWorldY;
                        rectangle_selection_create = true;
                    }

                        LevelEditor_CreateObject(category, obj_type, controls->tileInPixelsX, controls->tileInPixelsY,
                                                 position_in_array, controls->mousePositionInWorldX,
                                                 controls->mousePositionInWorldY, world, unlimited_creation,
                                                 controls->temp_object_to_create->angle);

                }
                else
                    printf("out of bound!!!");
            }

            //remove wall (replace it with empty entity)
            if(pressedKeys[SDLK_c])

            {


            }

            //deleting monster
            if(pressedKeys[SDLK_x])
            {
                if(pressedKeys[SDLK_LSHIFT] && !rectangle_selection_delete)
                {
                    rectangle_selection_startX = controls->mousePositionInWorldX;
                    rectangle_selection_startY = controls->mousePositionInWorldY;
                    rectangle_selection_delete = true;
                }
                if(!rectangle_selection_delete)
                {
                    LevelEditor_UpdateWallsCorners(position_in_array, world);
                    free(world->map[position_in_array]);
                    world->map[position_in_array] = NULL;
                }

                for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
                {
                    Entity* mob = (Entity*)Vector_Get(&world->monsters_vector, i);
                    if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                       controls->mousePositionInWorldY,
                                                       &mob->box))
                    {
                        Entity_Destroy(mob);
                        Vector_Delete(&world->monsters_vector, i);

                    }
                }
                for(int i = 0 ; i < Vector_Count(&world->bonus_vector) ; i++)
                {
                    Entity* bonus = (Entity*)Vector_Get(&world->bonus_vector, i);
                    if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                       controls->mousePositionInWorldY,
                                                       &bonus->box))
                    {
                        Entity_Destroy(bonus);
                        Vector_Delete(&world->bonus_vector, i);

                    }
                }
                for(int i = 0 ; i < Vector_Count(&world->props_vector) ; i++)
                {
                    Entity* obj = (Entity*)Vector_Get(&world->props_vector, i);
                    if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                       controls->mousePositionInWorldY,
                                                       &obj->box))
                    {
                        Entity_Destroy(obj);
                        Vector_Delete(&world->props_vector, i);

                    }
                }
                for(int i = 0 ; i < Vector_Count(&world->decals_vector) ; i++)
                {
                    Entity* obj = (Entity*)Vector_Get(&world->decals_vector, i);
                    if(BoundingBox_CheckPointCollision(controls->mousePositionInWorldX,
                                                       controls->mousePositionInWorldY,
                                                       &obj->box))
                    {
                        Entity_Destroy(obj);
                        Vector_Delete(&world->decals_vector, i);

                    }
                }
            }



            /*stopping pressing left shift stop the selection and creates the selection tile
            in the selection (only if it's a tile and not a monster/object
            */
            if((rectangle_selection_create || rectangle_selection_delete) && pressedKeys[SDLK_LSHIFT])
            {
                rectangle_selection_endX = controls->mousePositionInWorldX;
                rectangle_selection_endY =controls->mousePositionInWorldY;

                int startX = rectangle_selection_startX;
                int endX = rectangle_selection_endX;
                int startY = rectangle_selection_startY;
                int endY = rectangle_selection_endY;

                if(rectangle_selection_endY <= rectangle_selection_startY)
                {
                    startY = rectangle_selection_endY;
                    endY = rectangle_selection_startY;
                }
                if(rectangle_selection_endX <= rectangle_selection_startX)
                {
                    startX = rectangle_selection_endX;
                    endX = rectangle_selection_startX;
                }


                int tileStartX = startX / TILE_SIZE;
                int tileStartY = startY / TILE_SIZE;
                int tileEndX = endX / TILE_SIZE;
                int tileEndY = endY / TILE_SIZE;

                for(int y = tileStartY ; y <= tileEndY ; y++)
                {
                    for(int x = tileStartX ; x <= tileEndX ; x++)
                    {
                        position_in_array = y * world->map_width + x;
                        if(rectangle_selection_create)
                        {
                            int obj_type = level_editor->active_button->button_type;
                            LevelEditor_CreateObject(category, obj_type, x * TILE_SIZE, y * TILE_SIZE,
                                         position_in_array, controls->mousePositionInWorldX,
                                         controls->mousePositionInWorldY, world, unlimited_creation,
                                         controls->temp_object_to_create->angle);
                        }
                        else if(rectangle_selection_delete)
                        {
                            LevelEditor_UpdateWallsCorners(position_in_array, world);
                            free(world->map[position_in_array]);
                            world->map[position_in_array] = NULL;

                        }
                    }
                }

                if(rectangle_selection_delete)
                {
                    for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
                    {
                        Entity* obj = (Entity*)Vector_Get(&world->monsters_vector, i);
                        if(obj->box.right > startX &&
                           obj->box.left < endX &&
                           obj->box.bottom > startY &&
                           obj->box.top < endY)
                        {
                            Entity_Destroy(obj);
                            Vector_Delete(&world->monsters_vector, i);
                        }
                    }
                    for(int i = 0 ; i < Vector_Count(&world->bonus_vector) ; i++)
                    {
                        Entity* obj = (Entity*)Vector_Get(&world->bonus_vector, i);
                        if(obj->box.right > startX &&
                           obj->box.left < endX &&
                           obj->box.bottom > startY &&
                           obj->box.top < endY)
                        {
                            Entity_Destroy(obj);
                            Vector_Delete(&world->bonus_vector, i);

                        }
                    }
                    for(int i = 0 ; i < Vector_Count(&world->props_vector) ; i++)
                    {
                        Entity* obj = (Entity*)Vector_Get(&world->props_vector, i);
                        if(obj->box.right > startX &&
                           obj->box.left < endX &&
                           obj->box.bottom > startY &&
                           obj->box.top < endY)
                        {
                            Entity_Destroy(obj);
                            Vector_Delete(&world->props_vector, i);

                        }
                    }
                    for(int i = 0 ; i < Vector_Count(&world->decals_vector) ; i++)
                    {
                        Entity* obj = (Entity*)Vector_Get(&world->decals_vector, i);
                        if(obj->box.right > startX &&
                           obj->box.left < endX &&
                           obj->box.bottom > startY &&
                           obj->box.top < endY)
                        {
                            Entity_Destroy(obj);
                            Vector_Delete(&world->decals_vector, i);

                        }
                    }
                }
            }

            if((rectangle_selection_create || rectangle_selection_delete) && !pressedKeys[SDLK_LSHIFT])
            {
                rectangle_selection_create = false;
                rectangle_selection_delete = false;
            }


        }





        //deleting every monster
        if(pressedKeys[SDLK_u])
        {
           // Vector_Clear(&world->monsters_vector); leads to memory leak
        }



        //cancel current selected object
        /*if(pressedKeys[SDLK_e])
        {
            level_editor->active_button = NULL;
        }*/



        //switch mobs AI
        if(pressedKeys[SDLK_a] &&
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
			pressedKeys[controls->e.key.keysym.sym] = true;

			controls->pressedMods[controls->e.key.keysym.mod] = true;
		}

		if (controls->e.type == SDL_KEYUP)
		{
			pressedKeys[controls->e.key.keysym.sym] = false;
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
	controls->mouseTileX = (controls->mousePositionInWorldX - (int)controls->mousePositionInWorldX % TILE_SIZE) / TILE_SIZE;

	controls->mouseTileY = (controls->mousePositionInWorldY - (int)controls->mousePositionInWorldY % TILE_SIZE) / TILE_SIZE;

    controls->tileInPixelsX = controls->mouseTileX * TILE_SIZE;
    controls->tileInPixelsY = controls->mouseTileY * TILE_SIZE;

    //Graphics_DrawUI();

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
    if(controls->timer_menu <= 0 && (pressedKeys[SDLK_ESCAPE] == true ||
       SDL_JoystickGetButton(controller, BUTTON_START)))
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

        if(!using_controller_g)
        {
            //determine if running
            if(!world->player.playerC->running &&
                   pressedKeys[SDL_SCANCODE_LSHIFT])
            {
                Player_StartRunning(&world->player);
            }
            else if(world->player.playerC->running &&
                   !pressedKeys[SDL_SCANCODE_LSHIFT])
            {
                Player_StopRunning(&world->player);
            }
        }

        //apply keyboard controls to player movement
        if (pressedKeys[SDLK_z] == true)
        {
            player->movementC->dy = -1 * player->movementC->speed * delta_g;
        }
        if (pressedKeys[SDLK_s] == true)
        {
            player->movementC->dy = 1 * player->movementC->speed * delta_g;
        }
        if (pressedKeys[SDLK_d] == true)
        {
            player->movementC->dx = 1 * player->movementC->speed * delta_g;
        }
        if (pressedKeys[SDLK_q] == true)
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
            bool running = false;

            if(controller_leftTrigger > 0)
            {
                running = true;
            }

            float axis_divide = 150000;

            if(player->playerC->stamina > 0 && running)
            {
                axis_divide = 100000;
                Player_Run(player);
            }
            else
            {
                Player_Walk(player);
            }

            if(controller_leftAxisX < -deadzone || controller_leftAxisX > deadzone)
            {
                player->movementC->dx = ((float)controller_leftAxisX / axis_divide) * delta_g;
            }
            if(controller_leftAxisY < -deadzone || controller_leftAxisY > deadzone)
            {
                player->movementC->dy = ((float)controller_leftAxisY / axis_divide) * delta_g;
            }


            if(controller_rightAxisX > deadzone * 2 || controller_rightAxisX < -deadzone * 2 ||
               controller_rightAxisY > deadzone * 2 || controller_rightAxisY < -deadzone * 2)
            {

                    player->angle = atan2(controller_rightAxisY, controller_rightAxisX);

            }

            /*if(controller_rightAxisX > deadzone || controller_rightAxisX < -deadzone  ||
               controller_rightAxisY > deadzone  || controller_rightAxisY < -deadzone  &&
               bullet_time_g)
            {
                 aimPosX += ((float)controller_rightAxisX / axis_divide) * 2 * delta_g;
                aimPosY += ((float)controller_rightAxisY / axis_divide) * 2 * delta_g;
            }*/

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
            if(SDL_JoystickGetButton(controller, BUTTON_LEFT))
            {
                WeaponsComponent_ChangeWeapon(player->weaponsC, Weapon_TheBigGun);
            }
            if(SDL_JoystickGetButton(controller, BUTTON_LEFT_SHOULDER) && !bullet_time_g &&
               player->playerC->time_stop > 0)
            {
                aimPosX = player->x;
                aimPosY = player->y;
                bullet_time_g = true;
            }
            else if(!SDL_JoystickGetButton(controller, BUTTON_LEFT_SHOULDER) && bullet_time_g)
            {
                bullet_time_g = false;

            }

        }

        if(game_state_g != GameState_Editing_Map)
        {
            //reloading
            if(reloading_g)
            {
                if(pressedKeys[SDLK_r])
                {
                    WeaponsComponent_Reload(player->weaponsC);
                }
            }


            float muzzleX = 0;
            float muzzleY = 0;
            Entity_GetMiddleCoordinates(player, &muzzleX, &muzzleY);
            muzzleX -= player->weaponsC->current_weapon->bullet_height / 2;
            muzzleY -= player->weaponsC->current_weapon->bullet_width / 2;

            if(!using_controller_g)
            {
                //calculating angle to mouse
                float mouse_angle = C_AngleBetween2Points(
                                                muzzleX,
                                                muzzleY,
                                                controls->mousePositionInWorldX,
                                                controls->mousePositionInWorldY  );
                player->angle = mouse_angle;
            }


            //position of the targeted position in the map array
            int position_in_array = controls->mouseTileY * world->map_width + controls->mouseTileX;


            //opening/closing doors
            if(pressedKeys[SDLK_SPACE])
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

            if(!bullet_time_g)
            {
                aimPosX = 0;
                aimPosY = 0;
            }

            //shooting
            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] ||
               controller_rightTrigger > 0)
            {
                WeaponsComponent_TryToShoot(player->weaponsC,
                                              muzzleX,
                                              muzzleY,
                                              player->angle,
                                              bullets_vector,
                                              aimPosX,
                                              aimPosY);
                //bullet_time_g = false;
            }

        }


        /* LEVEL EDITOR STUFF */
        if(game_state_g == GameState_Editing_Map)
        {
            Inputs_ApplyInputsLevelEditor(controls, world, level_editor, gm);
        }

        if(pressedKeys[SDLK_F7] && SDL_GetTicks() - switch_timer > 5000)
        {
            if(game_state_g == GameState_Map_Editor_Testing_Level)
            {

                switch_timer = SDL_GetTicks();

                LevelEditor_BackToEditing(world);
            }
        }
    }
}

void Inputs_SavePressedKeys()
{
    for(int i = 0 ; i < 200 ; i++)
    {
        previousPressedKeys_g[i] = pressedKeys[i];
    }
}

bool* Inputs_GetPressedKeys()
{
    return pressedKeys;
}

void Inputs_GetCursor(int* x, int* y, Cursor_Type* type)
{
    *x = aimPosX;
    *y = aimPosY;
    *type = cursor_type;
}

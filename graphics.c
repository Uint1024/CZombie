#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "math.h"
#include "graphics.h"
#include "vector.h"
#include "entity.h"
#include "menu.h"
#include "menu_button.h"
#include "world.h"
#include "controls.h"
#include "weapons_component.h"
#include "weapon.h"
#include "window.h"
#include "zombie.h"
#include "gameManager.h"
#include "player.h"
#include "movement_component.h"


static SDL_Texture* textures_g[NB_OF_CAT][100];

static int cursor_size = 21;
static float cameraX = 0;
static float cameraY = 0;

void Graphics_SetCamera(float x, float y)
{
    cameraX = x;
    cameraY = y;
}

void Graphics_MoveCamera(float dx, float dy)
{
    cameraX += dx;
    cameraY += dy;
}
void Graphics_Create(int screen_width, int screen_height)
{
    SDL_ShowCursor(0);

    screen_width = screen_width;
    screen_height = screen_height;

    window = SDL_CreateWindow(   "C Game",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    screen_width,
                                    screen_height,
                                    SDL_WINDOW_SHOWN
                                );


	renderer = SDL_CreateRenderer(   window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED
                                    );


    textures_g[Cat_Player][Player_Normal] = IMG_LoadTexture(renderer, "player.png");

    textures_g[Cat_Zombie][Zombie_Normal] = IMG_LoadTexture(renderer, "zombie_normal.png");
    textures_g[Cat_Zombie][Zombie_Fast] = IMG_LoadTexture(renderer, "zombie_fast.png");
    textures_g[Cat_Zombie][Zombie_Heavy] = IMG_LoadTexture(renderer, "zombie_heavy.png");
    textures_g[Cat_Zombie][Zombie_Trooper] = IMG_LoadTexture(renderer, "zombie_trooper.png");
    textures_g[Cat_Zombie][Zombie_Huge] = IMG_LoadTexture(renderer, "zombie_huge.png");
    textures_g[Cat_Zombie][Zombie_Raptor] = IMG_LoadTexture(renderer, "zombie_raptor.png");
    textures_g[Cat_Zombie][Zombie_Slow] = IMG_LoadTexture(renderer, "zombie_slow.png");

    textures_g[Cat_Bonus][Bonus_Rifle] = IMG_LoadTexture(renderer, "bonus_automaticRifle.png");
    textures_g[Cat_Bonus][Bonus_GrenadeLauncher] = IMG_LoadTexture(renderer, "bonus_grenadeLauncher.png");
    textures_g[Cat_Bonus][Bonus_Shotgun] = IMG_LoadTexture(renderer, "bonus_shotgun.png");
    textures_g[Cat_Bonus][Bonus_TheBigGun] = IMG_LoadTexture(renderer, "bonus_biggun.png");
    textures_g[Cat_Bonus][Bonus_Handgun] = IMG_LoadTexture(renderer, "bonus_handgun.png");

    textures_g[Cat_Bullet][Weapon_Handgun] = IMG_LoadTexture(renderer, "bullet_normal.png");
    textures_g[Cat_Bullet][Weapon_AutomaticRifle] = IMG_LoadTexture(renderer, "bullet_normal.png");
    textures_g[Cat_Bullet][Weapon_TheBigGun] = IMG_LoadTexture(renderer, "bullet_normal.png");
    textures_g[Cat_Bullet][Weapon_Shotgun] = IMG_LoadTexture(renderer, "bullet_normal.png");
    textures_g[Cat_Bullet][Weapon_TripleFireball] = IMG_LoadTexture(renderer, "bullet_fireball.png");
    textures_g[Cat_Bullet][Weapon_FireballMachineGun] = IMG_LoadTexture(renderer, "bullet_fireball.png");
    textures_g[Cat_Bullet][Weapon_Fireball] = IMG_LoadTexture(renderer, "bullet_fireball.png");

    textures_g[Cat_Grenade][Grenade_Normal] = IMG_LoadTexture(renderer, "bullet_normal.png");

    textures_g[Cat_Wall][Wall_Normal] = IMG_LoadTexture(renderer, "wall_normal.png");
    textures_g[Cat_Wall][Wall_Cool] = IMG_LoadTexture(renderer, "wall_cool.png");
    textures_g[Cat_Wall][Wall_Glass] = IMG_LoadTexture(renderer, "wall_glass.png");
    textures_g[Cat_Wall][Wall_Reinforced] = IMG_LoadTexture(renderer, "wall_reinforced.png");
    textures_g[Cat_Wall][Wall_Good_Center] = IMG_LoadTexture(renderer, "wall_good_center.png");
    textures_g[Cat_Wall][Wall_Good_Corner] = IMG_LoadTexture(renderer, "wall_good_corner.png");

    textures_g[Cat_Explosion][Explosion_Normal] = IMG_LoadTexture(renderer, "explosion_normal.png");

    textures_g[Cat_Event][Event_Teleport_To_Other_Map] = IMG_LoadTexture(renderer, "event_mapchange.png");
    textures_g[Cat_Event][Event_End_Level] = IMG_LoadTexture(renderer, "event_mapend.png");
    textures_g[Cat_Event][Event_Player_Start] = IMG_LoadTexture(renderer, "event_playerstart.png");

    textures_g[Cat_Ground][Ground_Black] = IMG_LoadTexture(renderer, "ground_black.png");
    textures_g[Cat_Ground][Ground_Dirt] = IMG_LoadTexture(renderer, "ground_dirt.png");
    textures_g[Cat_Ground][Ground_Grass] = IMG_LoadTexture(renderer, "ground_grass.png");
    textures_g[Cat_Ground][Ground_Grey] = IMG_LoadTexture(renderer, "ground_grey.png");
    textures_g[Cat_Ground][Ground_MaroonLight] = IMG_LoadTexture(renderer, "ground_maroonLight.png");
    textures_g[Cat_Ground][Ground_Red] = IMG_LoadTexture(renderer, "ground_red.png");
    textures_g[Cat_Ground][Ground_WhiteDark] = IMG_LoadTexture(renderer, "ground_whiteDark.png");
    textures_g[Cat_Ground][Ground_Yellow] = IMG_LoadTexture(renderer, "ground_yellow.png");

    textures_g[Cat_Door][Door_Normal] = IMG_LoadTexture(renderer, "door_normal.png");
    textures_g[Cat_Door][Door_Reinforced] = IMG_LoadTexture(renderer, "door_reinforced.png");

    textures_g[Cat_Cursor][Cursor_Aim] = IMG_LoadTexture(renderer, "cursor_aim.png");
    textures_g[Cat_Cursor][Cursor_Resize_Left_Right] = IMG_LoadTexture(renderer, "cursor_resize_left_right.png");
    textures_g[Cat_Cursor][Cursor_Resize_Up_Down] = IMG_LoadTexture(renderer, "cursor_resize_up_down.png");

    textures_g[Cat_Decal][Decal_Corpse_Normal] = IMG_LoadTexture(renderer, "decal_corpse_normal.png");
    textures_g[Cat_Decal][Decal_Corpse_Fast] = IMG_LoadTexture(renderer, "decal_corpse_fast.png");
    textures_g[Cat_Decal][Decal_Corpse_Heavy] = IMG_LoadTexture(renderer, "decal_corpse_heavy.png");
    textures_g[Cat_Decal][Decal_Corpse_Trooper] = IMG_LoadTexture(renderer, "decal_corpse_trooper.png");
    textures_g[Cat_Decal][Decal_Corpse_Huge] = IMG_LoadTexture(renderer, "decal_corpse_huge.png");


    fonts[Small]             =   TTF_OpenFont("cour.ttf", 12);
	fonts[Medium]            =   TTF_OpenFont("cour.ttf", 16);
	fonts[Large]            =   TTF_OpenFont("cour.ttf", 28);
	fonts[Menu_font]         =   TTF_OpenFont("verdana.ttf", 46);

	text_surface             =   NULL;
	text_texture             =   NULL;
}


void Graphics_RenderGame(World* world,
                         Controls* controls, float fps, Window* level_editor,
                         GameManager* gm)
 {
     SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0xFF);
     SDL_RenderClear(renderer);



     Graphics_RenderWorld(world);
     Graphics_RenderUI(world, controls, fps, level_editor, gm);
     Graphics_Flip();
 }

void Graphics_RenderWorld(World* world)
{

    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;
    Vector* events_vector = &world->events_vector;
    Vector* decals_vector = &world->decals_vector;


    for (int i = 0; i < world->map_size; i++)
    {
        if (world->ground_map[i] != NULL)
        {
            if(Entity_CheckNear(&world->player, world->ground_map[i]))
            {
                Graphics_RenderObject(world->ground_map[i], world->player.playerC);
            }
        }
    }


    for(int i = 0 ; i < Vector_Count(decals_vector) ; i++)
    {
        Entity* decal = (Entity*)Vector_Get(decals_vector, i);
        if(Entity_CheckNear(&world->player, decal))
        {
            Graphics_RenderObject(decal, world->player.playerC);

        }
    }

    for(int i = 0 ; i < Vector_Count(&world->non_null_walls); i++)
    {
        Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);
        if (wall != NULL && wall->visible)
        {
            if(Entity_CheckNear(&world->player, wall))
            {
                Graphics_RenderObject(wall, world->player.playerC);

            }
        }
    }


    for(int i = 0 ; i < Vector_Count(events_vector) ; i++)
    {

        Entity* event = (Entity*)Vector_Get(events_vector, i);

        if(Entity_CheckNear(&world->player, event))
        {

            Graphics_RenderObject(event, world->player.playerC);

        }
    }




    Graphics_RenderObject(&world->player, world->player.playerC);

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {

        Entity* bullet = (Entity*)Vector_Get(bullets_vector, i);
        if(Entity_CheckNear(&world->player, bullet))
            Graphics_RenderObject(bullet, world->player.playerC);

    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);

            if(Entity_CheckNear(&world->player, bonus))
                Graphics_RenderObject(bonus, world->player.playerC);
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);

           if(Entity_CheckNear(&world->player, mob))
                Graphics_RenderObject(mob, world->player.playerC);

        }
    }

    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        if(Vector_Get(explosions_vector, i) != NULL)
        {
            Entity* explosion = (struct Entity*)Vector_Get(explosions_vector, i);
            Graphics_RenderObject( explosion, world->player.playerC);
        }
    }
}

void Graphics_RenderObject(Entity* object, PlayerC* playerC)
{
    float cameraX = playerC->cameraX;
    float cameraY = playerC->cameraY;
    if(object->t == Cat_Player)
    {
        int alpha_value = 255;


        if(object->playerC->invulnerability_timer > 0)
        {
            if(object->playerC->invulnerability_timer > 1500)
            {
                alpha_value = 100;
            }
            else if(object->playerC->invulnerability_timer > 1000)
            {
                alpha_value = 255;
            }
            else if(object->playerC->invulnerability_timer > 500)
            {
                alpha_value = 100;
            }
            else if(object->playerC->invulnerability_timer > 0)
            {
                alpha_value = 255;
            }

            SDL_SetTextureBlendMode(textures_g[object->t][object->sub_category], SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(textures_g[object->t][object->sub_category], alpha_value);
        }
    }

    if(object->t == Cat_Door && !object->solid)
    {
        SDL_SetTextureBlendMode(textures_g[object->t][object->sub_category], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(textures_g[object->t][object->sub_category], 50);
    }

    /*because of some rounding of the position
      in SDL, I have to correct the object position*/
    int errorX = 0;
    int errorY = 0;

    if(object->angle < HALF_PI)
    {
        //errorX and errorY = 0
    }
    else if(object->angle < PI)
    {
        errorX = 1;
    }
    else if(object->angle < PI + HALF_PI)
    {
        errorX = 1;
        errorY = 1;
    }
    else if(object->angle < 2 * PI)
    {
        errorY = 1;
    }
    const SDL_Rect rect = { object->box.left - cameraX - object->box.offsetX - errorX,
                            object->box.top - cameraY - object->box.offsetY - errorY,
                            object->box.width + object->box.offsetX * 2 ,
                            object->box.height + object->box.offsetY * 2};


    if(!object->in_dark || game_state_g == GameState_Editing_Map)
    {
        if(object->angle != 0 && object->visible)
        {
            SDL_RenderCopyEx(renderer,
                             textures_g[object->t][object->sub_category],
                             NULL,
                             &rect,
                             object->angle * 57.2957795f,//convert radian to degree
                             NULL,
                             SDL_FLIP_NONE);
        }
        else if(object->angle == 0 && object->visible)
        {
            SDL_RenderCopy(renderer, textures_g[object->t][object->sub_category],
                           NULL, &rect);
        }
    }



	if(debug_mode)
    {

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

        char positionX_str[6];
        snprintf(positionX_str, sizeof(positionX_str), "%d", (int)object->x);
        char positionY_str[6];
        snprintf(positionY_str, sizeof(positionY_str), "%d", (int)object->y);

        Graphics_RenderText(
                            positionX_str,
                            Small,
                            object->x - cameraX,
                            object->y - cameraY,
                            true,
                            White
                            );

        Graphics_RenderText(
                            positionY_str,
                            Small,
                            object->x - cameraX,
                            object->y - cameraY + 10,
                            true,
                            White
                                                        );
    }

    SDL_SetTextureBlendMode(textures_g[object->t][object->sub_category], SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(textures_g[object->t][object->sub_category], 255);
}

void Graphics_RenderCursor(int x, int y, Cursor_Type type)
{
   /* SDL_Rect cursor_rect = {x - cameraX, y - cameraY, cursor_size, cursor_size};
    SDL_RenderCopy(renderer, textures_g[Cat_Cursor][type],
                           NULL, &cursor_rect);*/
}

void Graphics_RenderText(char* text, Font_Size size,
                         int x, int y, bool shaded, Font_Color color)
{

    SDL_Color text_color = font_color_g[color];
    SDL_Color shading_color = { 0, 0, 0, 150 };

    if(shaded)
    {
        text_surface = TTF_RenderText_Shaded(fonts[size], text,
                                                        text_color, shading_color);
    }
    else
    {
        text_surface = TTF_RenderText_Solid(fonts[size], text,
                                                        text_color);
    }

    text_texture = SDL_CreateTextureFromSurface(renderer,
                                                text_surface);
    int textH = 0;
    int textW = 0;

    SDL_QueryTexture(text_texture, NULL, NULL, &textW, &textH);

    SDL_Rect textRect = { x, y, textW, textH };

    SDL_RenderCopy(renderer, text_texture, NULL, &textRect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);

    //free(text);
}

void Graphics_RenderFillBottomFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3)
{
    //v1 is at the top
    float invslope1 = C_GetSlopeBetween2Vec(v2, v1, false);
    float invslope2 = C_GetSlopeBetween2Vec(v3, v1, false);

    float curX1 = v1.x;
    float curX2 = v1.x;
    SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
    SDL_RenderDrawLine(renderer, v1.x, v1.y, v3.x, v3.y);
    SDL_RenderDrawLine(renderer, v3.x, v3.y, v2.x, v2.y);

    for(int scanlineY = v1.y ; scanlineY < v2.y ; scanlineY++)
    {
        SDL_RenderDrawLine(renderer, curX1, scanlineY, curX2, scanlineY);
        curX1 += invslope1;
        curX2 += invslope2;
    }
}

void Graphics_RenderFillTopFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3)
{
    //v3 is at the bottom
    //v1.y = v2.y
    float slope1 = -C_GetSlopeBetween2Vec(v2, v3, false);
    float slope2 = -C_GetSlopeBetween2Vec(v1, v3, false);

    float curX1 = v3.x;
    float curX2 = v3.x;


    for(int scanlineY = v3.y ; scanlineY > v2.y ; scanlineY--)
    {
        SDL_RenderDrawLine(renderer, curX1, scanlineY, curX2, scanlineY);
        curX1 += slope1;
        curX2 += slope2;

    }
}

void Graphics_RenderFillTriangle(Vec2 v1, Vec2 v2, Vec2 v3)
{
    SDL_SetRenderDrawColor(renderer, 250, 0, 60, 255);

    C_Sort3Vectors(&v1,&v2,&v3);



    if(v2.y == v3.y) //the bottom is flat
    {
        Graphics_RenderFillBottomFlatTriangle(v1, v2, v3);
    }
    else if(v1.y == v2.y) //the top is flat
    {
        Graphics_RenderFillTopFlatTriangle(v1, v2, v3);
    }
    else //split the triangle in 2 triangles and draw each of them
    {

        Vec2 v4 = {v1.x + ((v2.y - v1.y) / (v3.y - v1.y)) * (v3.x - v1.x), v2.y};

        Graphics_RenderFillBottomFlatTriangle(v1, v2, v4);
        Graphics_RenderFillTopFlatTriangle(v4, v2, v3);

    }

}
void Graphics_RenderMenu(Menu* menu, Controls* controls)
{
    SDL_SetRenderDrawColor(renderer, 0xE5, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    Vec2 v3 = {350,200};
    Vec2 v2 = {300,500};
    Vec2 v1 = {controls->mouseX, controls->mouseY};

    Graphics_RenderFillTriangle(v1, v2, v3);

    //render buttons of currently active menu
    for(int i = 0 ; i < Vector_Count(&menu->buttons) ; i++)
    {
        MenuButton* button = (MenuButton*)Vector_Get(&menu->buttons, i);

        SDL_Rect rect = BoundingBox_GetSDLRect(&button->box);
        /*SDL_RenderCopy(renderer,
                       button->text_texture,
                       NULL,
                       &rect);
*/
        Graphics_RenderText(button->text, Medium, rect.x, rect.y, false, Black);
        if(button->hover)
        {
            SDL_Rect underline;
            underline.x = button->text_rect.x;
            underline.y = button->box.bottom + 1;
            underline.h = 10;
            underline.w = button->text_rect.w;

        }
    }

    //render textfields (only useful in rare occasions, why did I make a vector, I dunno)
    for(int i = 0 ; i < Vector_Count(&menu->textfields) ; i++)
    {
        TextField* tf = (TextField*)Vector_Get(&menu->textfields, i);

        SDL_Rect rect = BoundingBox_GetSDLRect(&tf->box);
        SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
        SDL_RenderDrawRect(renderer, &rect);

        if(tf->visible_caret && tf == menu->active_textfield)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect caret_rect = {tf->caretX, tf->caretY, tf->caretWidth, tf->caretHeight};
            SDL_RenderFillRect(renderer, &caret_rect);
        }

        Graphics_RenderText(tf->text, Medium, tf->first_caretX, tf->caretY, false, Black);
    }

    //render the buttons of the load map/save menu
    if(menu->name == LoadMap_menu)
    {
        for(int i = 0 ; i < Vector_Count(&menu->file_list) ; i++)
        {
            MenuButton* button = (MenuButton*)Vector_Get(&menu->file_list, i);

            Graphics_RenderText(button->text, Large,
                                button->box.left, button->box.top,
                                true, White);
            SDL_Rect rect = BoundingBox_GetSDLRect(&button->box);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    //display text of the save menu
    else if(menu->name == SaveMap_menu)
    {
        Graphics_RenderText("Save map", Large,
                                400, 50,
                                false, Black);
    }

    //display mouse cursor
    SDL_Rect cursor_rect;
    cursor_rect.x = controls->mouseX - 10;
    cursor_rect.y = controls->mouseY - 10;
    cursor_rect.h = 21;
    cursor_rect.w = 21;

    SDL_RenderCopy(renderer, textures_g[Cat_Cursor][Cursor_Aim], NULL, &cursor_rect);

    Graphics_Flip();
}

void Graphics_RenderLevelEditorUI(World* world, Controls* controls,
                                  Window* level_editor, GameManager* gm)
{
    int cameraX = world->player.playerC->cameraX;
    int cameraY = world->player.playerC->cameraY;
    if(draw_grid_g)
    {
        for(int x = 0 ; x < world->map_width ; x++)
        {
            SDL_RenderDrawLine(renderer, x * TILE_SIZE - cameraX - 1,
                                   0,
                                   x * TILE_SIZE - cameraX,
                                   world->map_height * TILE_SIZE - cameraY);
        }
        for(int y = 0 ; y < world->map_height ; y++)
        {
            SDL_RenderDrawLine(renderer, 0,
                                   y * TILE_SIZE - cameraY - 1,
                                   world->map_width * TILE_SIZE - cameraX,
                                   y * TILE_SIZE - cameraY - 1);
        }
    }

    //---render level editor window
    SDL_SetRenderDrawColor(renderer, 220, 220,220, 255);
    SDL_Rect editor_rect = { level_editor->x, level_editor->y, level_editor->box.width, level_editor->box.height};
    SDL_RenderFillRect(renderer, &editor_rect);

    //--render level editor icons
    for(int i = 0; i < level_editor->nb_of_buttons ; i++)
    {
        SDL_Rect button_rect;
        button_rect.x = level_editor->buttons[i].x;
        button_rect.y = level_editor->buttons[i].y;
        button_rect.h = level_editor->buttons[i].box.height;
        button_rect.w = level_editor->buttons[i].box.width;

        SDL_RenderCopy(renderer, textures_g[level_editor->buttons[i].main_category][level_editor->buttons[i].button_type],
                       NULL, &button_rect);
    }

    /*if(level_editor->active_button != NULL)
    {
     //-------render rectangle around selected icon
        SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
        SDL_Rect rect = BoundingBox_GetSDLRect(&controls->active_button->box);
        SDL_RenderDrawRect(renderer, &rect);
    }*/


    if(controls->temp_object_to_create != NULL)
    {

        if(!controls->hovering_on_window)
        {
            Graphics_RenderObject(controls->temp_object_to_create, &world->player);

        }
    }

    if(!gm->ai_on)
    {
        char deactivated[] = "AI deactivated";

        Graphics_RenderText(deactivated, Medium, 300, 20, true, Black);
    }

}

/*void Graphics_SetTextureAlpha(Graphics* graphics, Texture_Type texture, int alpha)
{
        SDL_SetTextureBlendMode(graphics->textures[texture], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(graphics->textures[texture], alpha);
}*/

void Graphics_RenderGameUI(World* world)
{
    float cameraX = world->player.playerC->cameraX;
    float cameraY = world->player.playerC->cameraY;


    int aimPointX = world->player.x + cos(world->player.angle) * 600;
    int aimPointY = world->player.y + sin(world->player.angle) * 600;

    float playerMiddleX = 0;
    float playerMiddleY = 0;
    Entity_GetMiddleCoordinates(&world->player, &playerMiddleX, &playerMiddleY);
    SDL_RenderDrawLine(renderer, playerMiddleX - cameraX,
                       playerMiddleY - cameraY,
                       aimPointX - cameraX + 10,
                       aimPointY - cameraY + 10);


    if(using_controller_g && bullet_time_g)
    {
        int cursorX, cursorY;
        Cursor_Type type;
        Inputs_GetCursor(&cursorX, &cursorY, &type);

        Graphics_RenderCursor(cursorX, cursorY, type);
    }

    //stamina
    char stamina[] = "Stamina";
    Graphics_RenderText(stamina, Medium, 50, 700, true, White);

    SDL_Rect stamina_rect_back = {150,700,100,20};
    SDL_Rect stamina_rect_front = {150,700,world->player.playerC->stamina,20};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &stamina_rect_back);

    SDL_SetRenderDrawColor(renderer, 0, 255, 50, 255);
    SDL_RenderFillRect(renderer, &stamina_rect_front);


    //time_stop
    char time_stop[] = "Time Stop";
    Graphics_RenderText(time_stop, Medium, screen_width_g  - 260, 700, true, White);

    SDL_Rect time_stop_rect_back = {screen_width_g  - 150,700,100,20};
    SDL_Rect time_stop_rect_front = {screen_width_g  - 150,700,world->player.playerC->time_stop,20};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &time_stop_rect_back);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &time_stop_rect_front);

    //hp
    char hp[8];
    snprintf(hp, sizeof(hp), "%d hp", world->player.hp);
    Graphics_RenderText(hp, Medium, 700, 110, true, White);

    //name of weapon
    Graphics_RenderText(world->player.weaponsC->current_weapon->name, Medium, 700, 130, true, White);

    //bullets left
    char nb_of_bullets_on_player[70];
    if(reloading_g)
    {
        snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d%s%d%s%d",
                 world->player.weaponsC->current_weapon->magazine_bullets, " / ",
                 world->player.weaponsC->current_weapon->magazine_max_bullets, " / ",
                 world->player.weaponsC->bullets[world->player.weaponsC->current_weapon->type]
                 );
    }
    else
    {
        snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d ammos",
             world->player.weaponsC->bullets[world->player.weaponsC->current_weapon->type]
             );
    }
    Graphics_RenderText(nb_of_bullets_on_player, Medium, 700, 150, true, White);

    //reloading! text
    if(world->player.weaponsC->reloading)
    {
        char reloading_str[25] = "";
        snprintf(reloading_str, sizeof(reloading_str), "Reloading (%d)",
                 world->player.weaponsC->reload_timer);
        Graphics_RenderText(reloading_str,
                            Medium,
                            world->player.x - cameraX - 20,
                            world->player.y - 20 - cameraY
                            , true, White);
    }
}

void Graphics_RenderUI(World* world, Controls* controls,
                       float fps, Window* level_editor,
                       GameManager* gm)
{
    if(game_state_g == GameState_Editing_Map)
    {
       Graphics_RenderLevelEditorUI(world, controls, level_editor, gm);
    }

    else if(game_state_g != GameState_Editing_Map)
    {
        Graphics_RenderGameUI(world);

    }

    //====Cursor stuff
    if(!using_controller_g || game_state_g == GameState_Editing_Map ||
       game_state_g == GameState_Main_Menu)
    {
        if(!controls->cursor_resize_left_right)
        {
            SDL_Rect cursor_rect;
            cursor_rect.x = controls->mouseX - 10;
            cursor_rect.y = controls->mouseY - 10;
            cursor_rect.h = 21;
            cursor_rect.w = 21;
            SDL_RenderCopy(renderer, textures_g[Cat_Cursor][Cursor_Aim], NULL, &cursor_rect);
        }
        else if(controls->cursor_resize_left_right)
        {
            SDL_Rect cursor_rect = {controls->mouseX - 20, controls->mouseY - 10, 40, 20};
            SDL_RenderCopy(renderer, textures_g[Cat_Cursor][Cursor_Resize_Left_Right], NULL, &cursor_rect);
        }
    }


    //NON-GAMEPLAY
    //fps
    if (fps > 0)
    {
        char full_txt_fps[80];
        snprintf(full_txt_fps, sizeof full_txt_fps, "%f%s%d%s", fps, " FPS (", delta_g, " ms)");
        Graphics_RenderText(full_txt_fps, Medium, 700, 50, true, White);
    }

    //debug stuff
    if(debug_mode)
    {
        char nb_of_monsters[50];
        snprintf(nb_of_monsters, sizeof(nb_of_monsters), "%d%s", Vector_Count(&world->monsters_vector), " monsters on screen.");
        Graphics_RenderText(nb_of_monsters, Medium, 700, 100, true, White);

        char nb_of_bullets_on_screen[50];
        snprintf(nb_of_bullets_on_screen, sizeof(nb_of_bullets_on_screen), "%d%s", Vector_Count(&world->bullets_vector), " bullets on screen.");
        Graphics_RenderText(nb_of_bullets_on_screen, Medium, 700, 120, true, White);
    }
}
void Graphics_Flip()
{
    SDL_RenderPresent(renderer);
}

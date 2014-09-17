#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

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

Graphics* Graphics_Create(int screen_width, int screen_height)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_ShowCursor(0);

    Graphics* g = (Graphics*)malloc(sizeof(Graphics));

    g->screen_width = screen_width;
    g->screen_height = screen_height;

    g->window = SDL_CreateWindow(   "C Game",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    screen_width,
                                    screen_height,
                                    SDL_WINDOW_SHOWN
                                );


	g->renderer = SDL_CreateRenderer(   g->window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED
                                    );


    g->textures_names[Player_tex]                   =   "player.png";
    g->textures_names[Zombie_tex]                   =   "zombie.png";
    g->textures_names[FastZombie_tex]               =   "fastzombie.png";
    g->textures_names[HeavyZombie_tex]              =   "heavyzombie.png";
    g->textures_names[TrooperZombie_tex]            =   "trooperzombie.png";
    g->textures_names[HugeZombie_tex]               =   "hugezombie.png";
    g->textures_names[Ammo_Bonus_tex]               =   "bullet_bonus.png";
    g->textures_names[Rifle_Bonus_tex]              =   "rifle.png";
    g->textures_names[Shotgun_Bonus_tex]            =   "shotgun.png";
    g->textures_names[Bullet_tex]                   =   "bullet.png";
    g->textures_names[Fireball_tex]                 =   "fireball.png";
    g->textures_names[Wall_tex]                     =   "wall.png";
    g->textures_names[Explosion1_tex]               =   "explosion.png";
    g->textures_names[Cursor_aiming_tex]            =   "aim.png";
    g->textures_names[GrassGround_tex]              =   "ground_grass.png";
    g->textures_names[DirtGround_tex]               =   "ground_dirt.png";
    g->textures_names[GrenadeLauncher_Bonus_tex]    =   "grenadeLauncher.png";
    g->textures_names[Cursor_resize_up_down_tex]    =   "resize_up_down.png";
    g->textures_names[Cursor_resize_left_right_tex]  =   "resize_left_right.png";

    for(int i = 0 ; i < NB_OF_TEXTURES ; i++)
    {
        g->textures[i] =  IMG_LoadTexture(g->renderer, g->textures_names[i]);
    }


    g->fonts[Small]             =   TTF_OpenFont("cour.ttf", 12);
	g->fonts[Medium]            =   TTF_OpenFont("cour.ttf", 16);
	g->fonts[Menu_font]         =   TTF_OpenFont("verdana.ttf", 46);

	g->text_surface             =   NULL;
	g->text_texture             =   NULL;

    return g;
}


void Graphics_RenderGame(Graphics* g, World* world,
                         Controls* controls, float fps, int delta, Window* level_editor,
                         GameManager* gm)
 {
     SDL_SetRenderDrawColor(g->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
     SDL_RenderClear(g->renderer);
     Graphics_RenderWorld(g, world);
     Graphics_RenderUI(g, world, controls, fps, delta, level_editor, gm);
     Graphics_Flip(g);
 }

void Graphics_RenderWorld(Graphics* graphics, World* world)
{

    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;
    Entity* camera = world->player.camera;


    for (int i = 0; i < world->map_size; i++)
    {
        if (world->ground_map[i] != NULL)
        {
            if(Entity_CheckNear(&world->player, world->ground_map[i]))
                Graphics_RenderObject(graphics, world->ground_map[i], camera);
        }
    }

    for (int i = 0; i < world->map_size; i++)
    {
        if (world->map[i] != NULL)
        {
            if(Entity_CheckNear(&world->player, world->map[i]))
                Graphics_RenderObject(graphics, world->map[i], camera);
        }
    }
    Graphics_RenderObject(graphics, &world->player, camera);

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {

            Entity* bullet = (Entity*)Vector_Get(bullets_vector, i);
        if(Entity_CheckNear(&world->player, bullet))
            Graphics_RenderObject(graphics, bullet, camera);

    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);

            if(Entity_CheckNear(&world->player, bonus))
            Graphics_RenderObject(graphics, bonus, camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);

           if(Entity_CheckNear(&world->player, mob))
            Graphics_RenderObject(graphics, mob, camera);

        }
    }

    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        if(Vector_Get(explosions_vector, i) != NULL)
        {
            Entity* explosion = (struct Entity*)Vector_Get(explosions_vector, i);
            Graphics_RenderObject(graphics, explosion, camera);
        }
    }
}

void Graphics_RenderObject(Graphics* graphics, Entity* object, Entity* camera)
{
    if(object->t == Player_cat)
    {
        int alpha_value = 255;
        if(object->invulnerability_timer > 0)
        {
            if(object->invulnerability_timer > 1500)
            {
                alpha_value = 100;
            }
            else if(object->invulnerability_timer > 1000)
            {
                alpha_value = 255;
            }
            else if(object->invulnerability_timer > 500)
            {
                alpha_value = 100;
            }
            else if(object->invulnerability_timer > 0)
            {
                alpha_value = 255;
            }

            SDL_SetTextureBlendMode(graphics->textures[object->texture], SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(graphics->textures[object->texture], alpha_value);
        }
    }

    const SDL_Rect rect = { object->box.left - camera->x,
                            object->box.top - camera->y,
                            object->box.width,
                            object->box.height };

    SDL_RenderCopyEx(graphics->renderer,
                     graphics->textures[object->texture],
                     NULL,
                     &rect,
                     object->angle * 57.32f,
                     NULL,
                     SDL_FLIP_NONE);

	if(debug_mode)
    {

        SDL_SetRenderDrawColor(graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

        char positionX_str[6];
        snprintf(positionX_str, sizeof(positionX_str), "%d", (int)object->x);
        char positionY_str[6];
        snprintf(positionY_str, sizeof(positionY_str), "%d", (int)object->y);

        Graphics_RenderText(graphics,
                            positionX_str,
                            Small,
                            object->x - camera->x,
                            object->y - camera->y
                            );

        Graphics_RenderText(graphics,
                            positionY_str,
                            Small,
                            object->x - camera->x,
                            object->y - camera->y + 10
                            );
    }
}

void Graphics_RenderText(Graphics* graphics, char* text, Font_Size size, int x, int y)
{

    SDL_Color text_color = { 50, 255, 250, 255 };
    SDL_Color shading_color = { 0, 0, 0, 150 };

    graphics->text_surface = TTF_RenderText_Shaded(graphics->fonts[size], text, text_color, shading_color);

    graphics->text_texture = SDL_CreateTextureFromSurface(graphics->renderer, graphics->text_surface);
    int textH = 0;
    int textW = 0;

    SDL_QueryTexture(graphics->text_texture, NULL, NULL, &textW, &textH);

    SDL_Rect textRect = { x, y, textW, textH };

    SDL_RenderCopy(graphics->renderer, graphics->text_texture, NULL, &textRect);

    SDL_FreeSurface(graphics->text_surface);
    SDL_DestroyTexture(graphics->text_texture);
}

void Graphics_RenderMenu(Graphics* g, Menu* menu, Controls* controls)
{
    SDL_SetRenderDrawColor(g->renderer, 0xE5, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(g->renderer);

    for(int i = 0 ; i < Vector_Count(&menu->buttons) ; i++)
    {
        MenuButton* button = (MenuButton*)Vector_Get(&menu->buttons, i);

        SDL_RenderCopy(g->renderer,
                       button->text_texture,
                       NULL,
                       &button->text_rect);

        if(button->hover)
        {
            SDL_Rect underline;
            underline.x = button->text_rect.x;
            underline.y = button->box.bottom + 1;
            underline.h = 10;
            underline.w = button->text_rect.w;

            SDL_RenderCopy(g->renderer, g->textures[Wall_tex], NULL, &underline);
        }
    }

    SDL_Rect cursor_rect;
    cursor_rect.x = controls->mouseX - 10;
    cursor_rect.y = controls->mouseY - 10;
    cursor_rect.h = 21;
    cursor_rect.w = 21;

    SDL_RenderCopy(g->renderer, g->textures[Cursor_aiming_tex], NULL, &cursor_rect);

    Graphics_Flip(g);
}

void Graphics_RenderUI(Graphics* g, World* world, Controls* controls,
                       float fps, int delta, Window* level_editor,
                       GameManager* gm)
{
    if(controls->active_button != NULL)
    {
        if(!controls->hovering_on_window)
        {
            //-------render temp object on the map
            int obj_w = 5;
            int obj_h = 5;
            int obj_y = 5;
            int obj_x = 5;

            if(controls->active_button->main_category == Ground_cat ||
               controls->active_button->main_category == Wall_cat)
            {
                obj_w = TILE_SIZE;
                obj_h = TILE_SIZE;
                obj_x = controls->tileInPixelsX - world->player.camera->x;
                obj_y = controls->tileInPixelsY - world->player.camera->y;
            }
            else if(controls->active_button->main_category == Zombie_cat)
            {
                LevelEditor_Button button_type = controls->active_button->button_type;
                Zombie_Type zombie_type = gm->button_object_type_correspondance[button_type];


                obj_w = gm->zombie_templates[zombie_type]->box.width;
                obj_h = gm->zombie_templates[zombie_type]->box.height;

                obj_x = controls->mousePositionInWorldX - world->player.camera->x;
                obj_y = controls->mousePositionInWorldY - world->player.camera->y;
            }

            SDL_Rect blueprint_rect = {obj_x, obj_y, obj_w, obj_h };
        SDL_RenderCopy(g->renderer, g->textures[controls->active_button->texture],
                           NULL, &blueprint_rect);
        }


    }

    //---render level editor window
    SDL_SetRenderDrawColor(g->renderer, 255, 255,255, 255);
    SDL_Rect editor_rect = { level_editor->x, level_editor->y, level_editor->box.width, level_editor->box.height};
    SDL_RenderFillRect(g->renderer, &editor_rect);

    //--render level editor icons
    for(int i = 0; i < NB_OF_LEVEL_EDITOR_BUTTONS ; i++)
    {
        SDL_Rect button_rect;
        button_rect.x = level_editor->buttons[i].x;
        button_rect.y = level_editor->buttons[i].y;
        button_rect.h = level_editor->buttons[i].box.height;
        button_rect.w = level_editor->buttons[i].box.width;

        SDL_RenderCopy(g->renderer, g->textures[level_editor->buttons[i].texture],
                       NULL, &button_rect);
    }

    if(controls->active_button)
    {
     //-------render rectangle around selected icon
        SDL_SetRenderDrawColor(g->renderer, 255, 0,0, 255);
        SDL_Rect rect = BoundingBox_GetSDLRect(&controls->active_button->box);
        SDL_RenderDrawRect(g->renderer, &rect);
    }




    //====Cursor stuff
    if(!controls->cursor_resize_left_right)
    {
        SDL_Rect cursor_rect;
        cursor_rect.x = controls->mouseX - 10;
        cursor_rect.y = controls->mouseY - 10;
        cursor_rect.h = 21;
        cursor_rect.w = 21;
        SDL_RenderCopy(g->renderer, g->textures[Cursor_aiming_tex], NULL, &cursor_rect);
    }
    else if(controls->cursor_resize_left_right)
    {
        SDL_Rect cursor_rect = {controls->mouseX - 20, controls->mouseY - 10, 40, 20};
        SDL_RenderCopy(g->renderer, g->textures[Cursor_resize_left_right_tex], NULL, &cursor_rect);
    }

    if(!gm->ai_on)
    {
        char deactivated[] = "AI deactivated";

        Graphics_RenderText(g, deactivated, Medium, 300, 20);
    }

    //stamina
    char stamina[] = "Stamina";
    Graphics_RenderText(g, stamina, Medium, 50, 700);

    SDL_Rect stamina_rect_back = {150,700,100,20};
    SDL_Rect stamina_rect_front = {150,700,world->player.stamina,20};

    SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(g->renderer, &stamina_rect_back);

    SDL_SetRenderDrawColor(g->renderer, 0, 255, 50, 255);
    SDL_RenderFillRect(g->renderer, &stamina_rect_front);

    //hp
    char hp[8];
    snprintf(hp, sizeof(hp), "%d hp", world->player.hp);
    Graphics_RenderText(g, hp, Medium, 700, 110);

    //name of weapon
    Graphics_RenderText(g, world->player.weapons_component->current_weapon->name, Medium, 700, 130);

    //bullets left
    char nb_of_bullets_on_player[70];
    snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d%s%d%s%d",
             world->player.weapons_component->current_weapon->magazine_bullets, " / ",
             world->player.weapons_component->current_weapon->magazine_max_bullets, " / ",
             world->player.weapons_component->bullets[world->player.weapons_component->current_weapon->type]
             );
    Graphics_RenderText(g, nb_of_bullets_on_player, Medium, 700, 150);

    //reloading! text
    if(world->player.weapons_component->reloading)
    {
        char reloading_str[25] = "";
        snprintf(reloading_str, sizeof(reloading_str), "Reloading (%d)",
                 world->player.weapons_component->reload_timer);
        Graphics_RenderText(g, reloading_str,
                            Medium,
                            world->player.x - world->player.camera->x - 20,
                            world->player.y - 20 - world->player.camera->y);
    }


    //NON-GAMEPLAY
    //fps
    if (fps > 0)
    {
        char full_txt_fps[80];
        snprintf(full_txt_fps, sizeof full_txt_fps, "%f%s%d%s", fps, " FPS (", delta, " ms)");
        Graphics_RenderText(g, full_txt_fps, Medium, 700, 50);
    }

    //debug stuff
    if(debug_mode)
    {
        char nb_of_monsters[50];
        snprintf(nb_of_monsters, sizeof(nb_of_monsters), "%d%s", Vector_Count(&world->monsters_vector), " monsters on screen.");
        Graphics_RenderText(g, nb_of_monsters, Medium, 700, 100);

        char nb_of_bullets_on_screen[50];
        snprintf(nb_of_bullets_on_screen, sizeof(nb_of_bullets_on_screen), "%d%s", Vector_Count(&world->bullets_vector), " bullets on screen.");
        Graphics_RenderText(g, nb_of_bullets_on_screen, Medium, 700, 120);
    }
}
void Graphics_Flip(Graphics* graphics)
{
    SDL_RenderPresent(graphics->renderer);
}

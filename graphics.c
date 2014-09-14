#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "vector.h"
#include "entity.h"
#include "menu.h"
#include "menu_button.h"
#include "world.h"

Graphics* Graphics_Create(int screen_width, int screen_height)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

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


    g->textures[Player_tex]     =   IMG_LoadTexture(g->renderer, "player.png");
	g->textures[Zombie_tex]     =   IMG_LoadTexture(g->renderer, "zombie.png");
	g->textures[Ammo_Bonus_tex] =   IMG_LoadTexture(g->renderer, "bullet_bonus.png");
	g->textures[Bullet_tex]     =   IMG_LoadTexture(g->renderer, "bullet.png");
	g->textures[Wall_tex]       =   IMG_LoadTexture(g->renderer, "wall.png");
	g->textures[Explosion1_tex] =   IMG_LoadTexture(g->renderer, "explosion.png");


    g->fonts[Small]             =   TTF_OpenFont("cour.ttf", 12);
	g->fonts[Medium]            =   TTF_OpenFont("cour.ttf", 16);
	g->fonts[Menu_font]         =   TTF_OpenFont("verdana.ttf", 46);

	g->text_surface             =   NULL;
	g->text_texture             =   NULL;

    return g;
}

void Graphics_RenderWorld(Graphics* graphics, World* world)
{

    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;
    Entity* camera = world->player.camera;

    Graphics_RenderObject(graphics, &world->player, camera);

    for (int i = 0; i < world->map_size; i++)
    {
        if (world->map[i].t == Wall)
        {
            Graphics_RenderObject(graphics, &world->map[i], camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {
            Entity* bullet = (Entity*)Vector_Get(bullets_vector, i);
            Graphics_RenderObject(graphics, bullet, camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
            Graphics_RenderObject(graphics, bonus, camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
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

        char* positionX_str[6];
        snprintf(positionX_str, sizeof(positionX_str), "%d", (int)object->x);
        char* positionY_str[6];
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

void Graphics_RenderMenu(Graphics* g, Menu* menu)
{
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
}

void Graphics_Flip(Graphics* graphics)
{
    SDL_RenderPresent(graphics->renderer);
}

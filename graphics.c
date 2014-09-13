#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "vector.h"

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
                                    SDL_WINDOW_SHOWN |
                                    SDL_WINDOW_OPENGL
                                );

    SDL_GL_SwapWindow(g->window);
	SDL_GL_CreateContext(g->window);

	g->renderer = SDL_CreateRenderer(   g->window,
                                        -1,
                                        SDL_RENDERER_ACCELERATED
                                    );


    g->textures[Player_tex] =       IMG_LoadTexture(g->renderer, "player.png");
	g->textures[Zombie_tex] =       IMG_LoadTexture(g->renderer, "zombie.png");
	g->textures[Ammo_Bonus_tex] =   IMG_LoadTexture(g->renderer, "bullet_bonus.png");
	g->textures[Bullet_tex] =       IMG_LoadTexture(g->renderer, "bullet.png");
	g->textures[Wall_tex] =         IMG_LoadTexture(g->renderer, "wall.png");
	g->textures[Explosion1_tex] =   IMG_LoadTexture(g->renderer, "explosion.png");


    g->fonts[Small] =               TTF_OpenFont("cour.ttf", 12);
	g->fonts[Medium] =              TTF_OpenFont("cour.ttf", 16);

	g->text_surface = NULL;
	g->text_texture = NULL;

    return g;
}

void Graphics_RenderWorld(Graphics* graphics, Entity* map,
                            int map_size, Entity* player, Vector* bullets_vector,
                            Vector* bonus_vector, Vector* monsters_vector,
                            Vector* explosions_vector)
{
    //RenderPlayer(renderer, player, textures[Player_tex]);
    Graphics_RenderObject(graphics, player, player->camera);
    for (int i = 0; i < map_size; i++)
    {
        if (map[i].t == Wall)
        {
            Graphics_RenderObject(graphics, &map[i], player->camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {
            Entity* bullet = (Entity*)Vector_Get(bullets_vector, i);
            Graphics_RenderObject(graphics, bullet, player->camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
            Graphics_RenderObject(graphics, bonus, player->camera);
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
            Graphics_RenderObject(graphics, mob, player->camera);

        }
    }

    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        if(Vector_Get(explosions_vector, i) != NULL)
        {
            Entity* explosion = (struct Entity*)Vector_Get(explosions_vector, i);
            Graphics_RenderObject(graphics, explosion, player->camera);
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
                            object->y - camera->y);

        Graphics_RenderText(graphics,
                            positionY_str,
                            Small,
                            object->x - camera->x,
                            object->y - camera->y + 10);
    }
}

void Graphics_RenderText(Graphics* graphics, char* text, Font_Size size, int x, int y)
{

    SDL_Color text_color = { 50, 255, 250, 255 };
    SDL_Color shading_color = { 0, 0, 0, 150 };

    graphics->text_surface = TTF_RenderText_Shaded(graphics->fonts[size], text, text_color, shading_color);
//graphics->text_surface = TTF_RenderText_Solid(graphics->fonts[size], text, text_color);
    graphics->text_texture = SDL_CreateTextureFromSurface(graphics->renderer, graphics->text_surface);
    int textH = 0;
    int textW = 0;

    SDL_QueryTexture(graphics->text_texture, NULL, NULL, &textW, &textH);

    SDL_Rect textRect = { x, y, textW, textH };

    SDL_RenderCopy(graphics->renderer, graphics->text_texture, NULL, &textRect);

    SDL_FreeSurface(graphics->text_surface);
    SDL_DestroyTexture(graphics->text_texture);
}

void Graphics_Flip(Graphics* graphics)
{
    SDL_RenderPresent(graphics->renderer);
}

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "common.h"

typedef struct Entity   Entity;
typedef struct Vector   Vector;
typedef struct MainMenu MainMenu;

typedef struct Graphics
{
    SDL_Texture*            textures[15];
    TTF_Font*               fonts[10];
    SDL_Window*             window;
    SDL_Renderer*           renderer;
    int                     screen_width;
    int                     screen_height;
    SDL_Surface*            text_surface;
    SDL_Texture*            text_texture;
} Graphics;

Graphics* Graphics_Create(  int screen_width, int screen_height);

void Graphics_Flip(         Graphics* graphics);

void Graphics_RenderWorld(  Graphics* graphics, Entity* map, int map_size,
                            Entity* player, Vector* bullets_vector,
                            Vector* bonus_vector, Vector* monsters_vector,
                            Vector* explosions_vector);

void Graphics_RenderObject( Graphics* graphics, Entity* object, Entity* camera);

void Graphics_RenderText(   Graphics* graphics, char* text, Font_Size size,
                            int x, int y);

void Graphics_RenderMenu(   Graphics* graphics, MainMenu* main_menu);
#endif // GRAPHICS_H

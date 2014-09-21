#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "common.h"

typedef struct Entity   Entity;
typedef struct Vector   Vector;
typedef struct Menu     Menu;
typedef struct World    World;
typedef struct Controls    Controls;
typedef struct Window    Window;
typedef struct GameManager GameManager;


typedef struct Graphics
{
    char*                   textures_names[NB_OF_TEXTURES];
    SDL_Texture*            textures[NB_OF_TEXTURES];
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

void Graphics_RenderWorld(  Graphics* graphics, World* world);

void Graphics_RenderObject( Graphics* graphics, Entity* object, Entity* camera);

void Graphics_RenderText(Graphics* graphics, char* text, Font_Size size,
                         int x, int y, Jbool shaded,Font_Color color);

void Graphics_RenderMenu(   Graphics* g, Menu* menu, Controls* controls);

void Graphics_RenderUI(Graphics* g, World* world, Controls* controls, float fps, Window* level_editor, GameManager* gm);

void Graphics_RenderGame(Graphics* g, World* world,
                         Controls* controls, float fps, Window* level_editor, GameManager* gm);

void Graphics_RenderGameUI(Graphics* g, World* world);

void Graphics_RenderLevelEditorUI(Graphics* g, World* world, Controls* controls,Window* level_editor, GameManager* gm);


#endif // GRAPHICS_H

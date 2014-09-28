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
typedef struct PlayerC PlayerC;

/*typedef struct Graphics
{
    //char*                   textures_names[NB_OF_TEXTURES];
    //SDL_Texture*            textures[NB_OF_TEXTURES];


} Graphics;
*/

static TTF_Font*               fonts[10];
static SDL_Window*             window;
static SDL_Renderer*           renderer;
static int                     screen_width;
static int                     screen_height;
static SDL_Surface*            text_surface;
static SDL_Texture*            text_texture;

void Graphics_Create(  int screen_width, int screen_height);

void Graphics_Flip();

void Graphics_RenderWorld(World* world);

void Graphics_RenderObject(Entity* object, PlayerC* playerC);

void Graphics_RenderText(char* text, Font_Size size,
                         int x, int y, bool shaded,Font_Color color);

void Graphics_RenderMenu(Menu* menu, Controls* controls);

void Graphics_RenderUI(World* world, Controls* controls, float fps, Window* level_editor, GameManager* gm);

//void Graphics_DrawUI(G)
void Graphics_RenderGame(World* world,
                         Controls* controls, float fps, Window* level_editor, GameManager* gm);

void Graphics_RenderGameUI(World* world);

void Graphics_RenderLevelEditorUI(World* world, Controls* controls,Window* level_editor, GameManager* gm);

//void Graphics_SetTextureAlpha(, Texture_Type texture, int alpha);

void Graphics_RenderFillTriangle(Vec2 v1, Vec2 v2, Vec2 v3);

void Graphics_RenderFillBottomFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3);
void Graphics_RenderFillTopFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3);

void Graphics_RenderCursor(int x, int y, Cursor_Type type);

void Graphics_MoveCamera(float dx, float dy);
void Graphics_SetCamera(float x, float y);

#endif // GRAPHICS_H

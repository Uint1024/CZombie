#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H
#include <SDL.h>
#include "boundingBox.h"
typedef struct Graphics Graphics;

typedef struct MenuButton
{
    float             x;
    float             y;
    Box             box;
    char*           text;
    SDL_Texture*    text_texture;
    SDL_Rect        text_rect;
} MenuButton;

MenuButton MenuButton_Create(float x, float y, char* text, Jbool centered, Graphics* graphics);

#endif // MAIN_MENU_H

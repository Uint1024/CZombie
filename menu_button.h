#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H
#include <SDL.h>
#include "boundingBox.h"
typedef struct Graphics Graphics;

typedef struct MenuButton
{
    Menu_Button_Name    name;
    float               x;
    float               y;
    Box                 box;
    char*               text;
    SDL_Texture*        text_texture;
    SDL_Rect            text_rect;
    Jbool               hover;

} MenuButton;

MenuButton* MenuButton_Create(Menu_Button_Name name,
                              float x, float y,
                              char* text, Jbool centered,
                              Graphics* graphics);
MenuButton* FileNameButton_Create(float y, char* text);
void MenuButton_UpdateBox(MenuButton* b, int x, int y);
#endif // MAIN_MENU_H

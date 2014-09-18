#include "menu_button.h"
#include "graphics.h"
#include <SDL_ttf.h>
#include <stdlib.h>

MenuButton* MenuButton_Create(Menu_Button_Name name,
                              float x, float y,
                              char* text, Jbool centered,
                              Graphics* graphics)
{
    int textH = 0;
    int textW = 0;
    SDL_Color text_color = { 40, 40, 40, 255 };
    SDL_Surface* text_surface = TTF_RenderText_Solid(graphics->fonts[Menu_font],
                                                     text, text_color);


    MenuButton* button = (MenuButton*)malloc(sizeof(MenuButton));

    button->text_texture =  SDL_CreateTextureFromSurface(graphics->renderer,
                                                         text_surface);

    SDL_QueryTexture(button->text_texture, NULL, NULL, &textW, &textH);
    SDL_FreeSurface(text_surface);


    button->name         =   name;
    button->text         =   text;

    button->x            =   centered ? graphics->screen_width / 2 - textW / 2 : x;
    button->y            =   y;

    button->box          =   BoundingBox_CreateFromAllValues(textW, textH, button->x, button->y);
    SDL_Rect text_rect   =   { button->x , button->y, textW, textH };
    button->text_rect    =   text_rect;
    button->hover        =   Jfalse;



    return button;
}

MenuButton* FileNameButton_Create(float y, char* text)
{
    MenuButton* button = (MenuButton*)malloc(sizeof(MenuButton));

    button->name         =   FILE_NAME_BUTTON;
    button->text         =   text;

    button->x            =   100;
    button->y            =   y;

    button->box          =   BoundingBox_CreateFromAllValues(500, 100, 100, button->y);
    SDL_Rect text_rect   =   { 100 , button->y, 500, 100 };
    button->text_rect    =   text_rect;
    button->hover        =   Jfalse;



    return button;
}

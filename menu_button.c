#include "menu_button.h"
#include "graphics.h"
#include <SDL_ttf.h>

MenuButton MenuButton_Create(float x, float y, char* text, Jbool centered, Graphics* graphics)
{


    MenuButton button;

    button.text = text;

    SDL_Color text_color = { 50, 255, 250, 255 };
    SDL_Color shading_color = { 0, 0, 0, 150 };


    SDL_Surface* text_surface = TTF_RenderText_Shaded(graphics->fonts[Medium], text, text_color, shading_color);
    button.text_texture = SDL_CreateTextureFromSurface(graphics->renderer, text_surface);

    int textH = 0;
    int textW = 0;

    SDL_QueryTexture(button.text_texture, NULL, NULL, &textW, &textH);


    SDL_FreeSurface(text_surface);

    if(centered)
    {
        button.x        =   graphics->screen_width / 2 - textW / 2;
    }
    else
    {
        button.x        =   x;
    }

    button.y            =   y;
    button.box          =   BoundingBox_CreateFromAllValues(textW, textH, button.x, button.y);


    SDL_Rect text_rect  =   { button.x , button.y, textW, textH };

    button.text_rect    =   text_rect;

    return button;
}

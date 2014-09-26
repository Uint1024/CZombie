#include "menu_button.h"
#include "graphics.h"
#include <SDL_ttf.h>
#include <stdlib.h>

MenuButton* MenuButton_Create(Menu_Button_Name name,
                              float x, float y,
                              char* text, bool centered
                              )
{
    int textH = 100;
    int textW = 400;
  //  SDL_Color text_color = { 40, 40, 40, 255 };
/*    SDL_Surface* text_surface = TTF_RenderText_Solid(graphics->fonts[Menu_font],
                                                     text, text_color);
*/

    MenuButton* button = (MenuButton*)malloc(sizeof(MenuButton));

    /*button->text_texture =  SDL_CreateTextureFromSurface(graphics->renderer,
                                                         text_surface);

    SDL_QueryTexture(button->text_texture, NULL, NULL, &textW, &textH);
    SDL_FreeSurface(text_surface);*/


    switch(name)
    {
    case Play_button:
        textW = 400;
        break;
    case LevelEditor_button:
        textW = 400;
        break;
    case NewMap_button:
        textW = 400;
        break;
    case LoadMap_button:
        textW = 400;
        break;
    case SaveGame_button:
        textW = 400;
        break;
    case LoadGame_button:
        textW = 400;
        break;
    case SaveMap_button:
        textW = 400;
        break;
    case Options_button:
        textW = 400;
        break;
    case Quit_button:
        textW = 400;
        break;
    case Back_button:
        textW = 400;
        break;
    case Exit_Level_Editor_button:
        textW = 400;
        break;
    }


    button->name         =   name;
    button->text         =   text;

    button->x            =   centered ? screen_width_g / 2 - textW / 2 : x;
    button->y            =   y;

    button->box          =   BoundingBox_CreateBetter(button->x, button->y, textW, textH);
    SDL_Rect text_rect   =   { button->x , button->y, textW, textH };
    button->text_rect    =   text_rect;
    button->hover        =   false;


    return button;
}

MenuButton* FileNameButton_Create(float y, char* text)
{
    MenuButton* button = (MenuButton*)malloc(sizeof(MenuButton));

    button->name         =   FILE_NAME_BUTTON;
    button->text         =   text;

    button->x            =   100;
    button->y            =   y;

    button->box          =   BoundingBox_CreateBetter(100, button->y, 500, 50);
    SDL_Rect text_rect   =   { 100 , button->y, 500, 50 };
    button->text_rect    =   text_rect;
    button->hover        =   false;



    return button;
}

void MenuButton_UpdateBox(MenuButton* b, int x, int y)
{
    b->x = x;
    b->y = y;
    b->box = BoundingBox_CreateBetter(
                                    b->x,
                                    b->y, b->box.width,
                                    b->box.height);
    b->text_rect.w = b->box.width;
    b->text_rect.h = b->box.height;
    b->text_rect.x = b->x;
    b->text_rect.y = b->y;
}

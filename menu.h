#ifndef MENU_H
#define MENU_H
#include "menu_button.h"
#include "vector.h"

typedef struct Menu
{
    Menu_Name       name;
    Vector          buttons;
    Vector          texts;
    int             total_buttons;
    int             total_texts;
} Menu;

Menu* OptionMenu_Create(Graphics* graphics);
Menu* MainMenu_Create(Graphics* graphics);
#endif // MENU_H

#ifndef MENU_H
#define MENU_H
#include "menu_button.h"
#include "vector.h"
#include "boundingBox.h"

typedef struct TextField
{
    char*           text;
    int             x;
    int             y;
    int             width;
    int             height;
    Box             box;
    int             caret_blinking_timer;
    Jbool           visible_caret;
    int             caret_blinking_delay;
    int             caretX;
    int             caretY;
    int             caretHeight;
    int             caretWidth;
} TextField;

typedef struct Menu
{
    Menu_Name       name;
    Vector          buttons;
    Vector          texts;
    Vector          textfields;
    int             total_buttons;
    int             total_texts;
    TextField*      active_textfield;
} Menu;



Menu* OptionMenu_Create(Graphics* graphics);
Menu* MainMenu_Create(Graphics* graphics);
Menu* SaveLevelMenu_Create(Graphics* graphics);
TextField* TextField_Create(int x, int y, int width, int height);
#endif // MENU_H

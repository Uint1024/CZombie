#ifndef MENU_H
#define MENU_H
#include "menu_button.h"
#include "vector.h"
#include "boundingBox.h"
#include "controls.h"

typedef struct MenuManager MenuManager;

typedef struct TextField
{
    char*           text;
    int             x;
    int             y;
    int             max_char;
    Box             box;
    int             caret_blinking_timer;
    bool           visible_caret;
    int             caret_blinking_delay;
    int             first_caretX;
    int             caretX;
    int             caretY;
    int             caretHeight;
    int             caretWidth;
    int             input_timer;
    int             input_delay;
    int             can_type;

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
    Vector          file_list;
} Menu;



Menu* OptionMenu_Create(Graphics* graphics);
Menu* MainMenu_Create(MenuManager* menu_manager);
Menu* SaveLevelMenu_Create(Graphics* graphics);
Menu* LoadLevelMenu_Create(Graphics* graphics);
Menu* LevelEditorMainMenu_Create(MenuManager* menu_manager);
Menu*  LevelEditorEditing_Create(MenuManager* menu_manager);

TextField* TextField_Create(int x, int y, int width, int height);
void TextField_Update(TextField* tf);
void TextField_Caret_Update(TextField* tf);
void TextField_Input(TextField* tf, char c);
#endif // MENU_H

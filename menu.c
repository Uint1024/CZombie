#include "menu.h"
#include "menu_button.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "menu_button.h"
#include "menu_manager.h"


Menu* MainMenu_Create(MenuManager* menu_manager)
{
    Menu* mainMenuMenu = (Menu*)malloc(sizeof(Menu));
    mainMenuMenu->buttons = Vector_Create();
    mainMenuMenu->texts = Vector_Create();
    mainMenuMenu->name                       =   MainMenu_menu;
    mainMenuMenu->active_textfield = NULL;


    Menu_Button_Name Menu_Button_Name[4] = {Play_button,
                                            LevelEditor_button,
                                            Options_button,
                                            Quit_button};

    for(int j = 0 ; j < 4 ; j++)
    {
        for(int i = 0 ; i < TOTAL_MAIN_MENU_BUTTONS ; i++)
        {
            MenuButton* button = Vector_Get(&menu_manager->all_buttons, i);


            if(button->name == Menu_Button_Name[j])
            {
                //new button position
                button->y = 100 * j + 50;
                button->box = BoundingBox_CreateBetter(button->x,
                                                       button->y,
                                                       button->box.width,
                                                       button->box.height);

                MenuButton* new_button = (MenuButton*)malloc(sizeof(MenuButton));

                memcpy(new_button, button, sizeof(MenuButton));
                Vector_Push(&mainMenuMenu->buttons, new_button);
            }
        }
    }

    return mainMenuMenu;
}


Menu*  LevelEditorEditing_Create(MenuManager* menu_manager)
{

    Menu* levelEditorEditingMenu = (Menu*)malloc(sizeof(Menu));
    levelEditorEditingMenu->buttons = Vector_Create();
    levelEditorEditingMenu->texts = Vector_Create();
    levelEditorEditingMenu->name                       =   LevelEditorEditing_menu;
    levelEditorEditingMenu->active_textfield = NULL;

    Menu_Button_Name Menu_Button_Name[5] = {NewMap_button,
                                            SaveMap_button,
                                            LoadMap_button,
                                            Options_button,
                                            Exit_Level_Editor_button};
    for(int j = 0 ; j < 5 ; j++)
    {
        for(int i = 0 ; i < TOTAL_MAIN_MENU_BUTTONS ; i++)
        {
            MenuButton* button = Vector_Get(&menu_manager->all_buttons, i);


            if(button->name == Menu_Button_Name[j])
            {

                //new button position
                button->y = 100 * j + 50;
                button->box = BoundingBox_CreateBetter(button->x,
                                                       button->y,
                                                       button->box.width,
                                                       button->box.height);
                MenuButton* new_button = (MenuButton*)malloc(sizeof(MenuButton));
                memcpy(new_button, button, sizeof(MenuButton));
                Vector_Push(&levelEditorEditingMenu->buttons, new_button);
            }

        }
    }

    return levelEditorEditingMenu;
}

Menu* OptionMenu_Create(Graphics* graphics)
{
    Menu* optionsMenu = (Menu*)malloc(sizeof(Menu));


    optionsMenu->buttons = Vector_Create();
    optionsMenu->texts = Vector_Create();

    optionsMenu->name                       =   Options_menu;

    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 400,
                                                      "Back",
                                                      true, graphics);
    Vector_Push(&optionsMenu->buttons, back_button);
    optionsMenu->active_textfield = NULL;
    return optionsMenu;
}

Menu* LevelEditorMainMenu_Create(MenuManager* menu_manager)
{

    Menu* levelEditorMenu = (Menu*)malloc(sizeof(Menu));
    levelEditorMenu->buttons = Vector_Create();
    levelEditorMenu->texts = Vector_Create();
    levelEditorMenu->name                       =   LevelEditor_menu;
    levelEditorMenu->active_textfield = NULL;

    Menu_Button_Name Menu_Button_Name[3] = {
                                            NewMap_button,
                                            LoadMap_button,
                                            Back_button};
    for(int j = 0 ; j < 3 ; j++)
    {
        for(int i = 0 ; i < TOTAL_MAIN_MENU_BUTTONS ; i++)
        {
            MenuButton* button = Vector_Get(&menu_manager->all_buttons, i);


            if(button->name == Menu_Button_Name[j])
            {
                //new button position
                button->y = 100 * j + 50;
                button->box = BoundingBox_CreateBetter(button->x,
                                                       button->y,
                                                       button->box.width,
                                                       button->box.height);
                MenuButton* new_button = (MenuButton*)malloc(sizeof(MenuButton));
                memcpy(new_button, button, sizeof(MenuButton));
                Vector_Push(&levelEditorMenu->buttons, new_button);
            }
        }
    }

    return levelEditorMenu;
}

Menu* SaveLevelMenu_Create(Graphics* graphics)
{
    Menu* saveMenu = (Menu*)malloc(sizeof(Menu));


    saveMenu->buttons = Vector_Create();
    saveMenu->texts = Vector_Create();
    saveMenu->textfields = Vector_Create();

    saveMenu->name                       =   SaveMap_menu;
    TextField* field = TextField_Create(200, 100, 500, 60);
    Vector_Push(&saveMenu->textfields, field);
    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 400,
                                                      "Back",
                                                      true, graphics);


    Vector_Push(&saveMenu->buttons, back_button);
    saveMenu->active_textfield = field;
    return saveMenu;
}

Menu* LoadLevelMenu_Create(Graphics* graphics)
{
    Menu* optionsMenu = (Menu*)malloc(sizeof(Menu));


    optionsMenu->buttons = Vector_Create();
    optionsMenu->texts = Vector_Create();
    optionsMenu->file_list = Vector_Create();

    optionsMenu->name                       =   LoadMap_menu;

    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 500,
                                                      "Back",
                                                      true, graphics);

    Vector_Push(&optionsMenu->buttons, back_button);
    optionsMenu->active_textfield = NULL;
    return optionsMenu;
}

TextField* TextField_Create(int x, int y, int width, int height)
{
    TextField* tf = (TextField*)malloc(sizeof(TextField));
    tf->input_delay = 70;
    tf->input_timer = 0;
    tf->max_char = 20;
    tf->text = calloc(tf->max_char, tf->max_char);

    tf->box.left = x;
    tf->box.top = y;
    tf->box.right = x + width;
    tf->box.bottom = y + height;
    tf->box.width = width;
    tf->box.height = height;

    tf->caret_blinking_delay = 400;
    tf->caret_blinking_timer = 0;
    tf->visible_caret = true;
    tf->caretX = x + 24;
    tf->first_caretX = tf->caretX;
    tf->caretY = y + 20;
    tf->caretWidth = 5;
    tf->caretHeight = 19;
    return tf;
}

void TextField_Input(TextField* tf, char c)
{
    size_t len = strlen(tf->text);
    if(len < tf->max_char)
    {
        tf->text[len] = c;
        tf->caretX += 10;
    }
}

void TextField_Backspace(TextField* tf)
{
    size_t len = strlen(tf->text);
    tf->text[len - 1] = 0;
    tf->caretX -= 10;
}

void TextField_Update(TextField* tf)
{

    TextField_Caret_Update(tf);
    size_t len = strlen(tf->text);
    bool* pressedKeys = Inputs_GetPressedKeys();

    for(int i = 0 ; i < 200 ; i ++)
{


        if(pressedKeys[i] == true && previousPressedKeys_g[i] == false &&
           (char)pressedKeys[i] != tf->text[len] &&
           !pressedKeys[SDLK_RETURN] &&
           !pressedKeys[SDLK_BACKSPACE] &&
           !pressedKeys[SDLK_RETURN2])
        {
            TextField_Input(tf, (char)(i));
        }
    }

    if(pressedKeys[SDLK_BACKSPACE] &&
            !previousPressedKeys_g[SDLK_BACKSPACE])
    {

        TextField_Backspace(tf);
    }
}

void TextField_Caret_Update(TextField* tf)
{
    tf->caret_blinking_timer += delta_g;
    if(tf->caret_blinking_timer > tf->caret_blinking_delay)
    {
        tf->visible_caret = tf->visible_caret ? false : true;
        tf->caret_blinking_timer = 0;
    }
}

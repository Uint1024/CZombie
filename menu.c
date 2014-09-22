#include "menu.h"
#include "menu_button.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "menu_button.h"
#include "menu_manager.h"


/*Menu* MainMenu_Create(Graphics* graphics)
{
    Menu* mainMenu = (Menu*)malloc(sizeof(Menu));

    mainMenu->name                      =   Main_Menu_menu;

    mainMenu->all_buttons = Vector_Create();
    MenuManager_LoadAllButtons(graphics, mainMenu);


    mainMenu->buttons = Vector_Create();



    //MainMenu_LoadMainEmptyMenu(mainMenu);

    mainMenu->texts = Vector_Create();
    mainMenu->active_textfield = NULL;
    return mainMenu;
}*/

/*void MainMenu_LoadAllButtons(Graphics* graphics, Menu* mainMenu)
{
    Menu_Button_Name Menu_Button_Name[11] = {Play_button,
                                            LevelEditor_button,
                                            NewMap_button,
                                            LoadMap_button,
                                            SaveMap_button,
                                            SaveGame_button,
                                            LoadGame_button,
                                            Exit_Level_Editor_button,
                                            Options_button,
                                            Quit_button,
                                            Back_button};

    char* text[11] = {"Play", "Level Editor", "New Map", "Load Map", "Save Map",
                        "Save Game", "Load Game",
                        "Exit Level Editor", "Options", "Quit", "Back"};

    for(int i = 0 ; i < 11 ; i++)
    {
        MenuButton* button = MenuButton_Create(Menu_Button_Name[i],
                                      100.0f, 100.0f,
                                      text[i], Jtrue,
                                      graphics);

        Vector_Push(&mainMenu->all_buttons,
                    button
                    );
    }
}*/


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
                                                      Jtrue, graphics);
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
                                                      Jtrue, graphics);


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
                                                      Jtrue, graphics);

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
    tf->visible_caret = Jtrue;
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

void TextField_Update(TextField* tf, Controls* controls)
{
    TextField_Caret_Update(tf);
    size_t len = strlen(tf->text);

    for(int i = 0 ; i < 200 ; i ++)
    {
        if(controls->pressedKeys[i] == Jtrue && previousPressedKeys_g[i] == Jfalse &&
           (char)controls->pressedKeys[i] != tf->text[len] &&
           !controls->pressedKeys[SDLK_RETURN] &&
           !controls->pressedKeys[SDLK_BACKSPACE] &&
           !controls->pressedKeys[SDLK_RETURN2])
        {
            TextField_Input(tf, (char)(i));
        }
    }
    if(controls->pressedKeys[SDLK_BACKSPACE] &&
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
        tf->visible_caret = tf->visible_caret ? Jfalse : Jtrue;
        tf->caret_blinking_timer = 0;
    }
}

#include "menu.h"
#include "menu_button.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
Menu* MainMenu_Create(Graphics* graphics)
{
    Menu* mainMenu = (Menu*)malloc(sizeof(Menu));

    mainMenu->name                      =   Main_Menu_menu;
    MenuButton* play_button             =   MenuButton_Create(Play_button,
                                                              100.0f, 100.0f,
                                                              "Play", Jtrue,
                                                              graphics);

    MenuButton* save_level_button       =   MenuButton_Create(SaveLevel_button,
                                                              100.0f, 220.0f,
                                                              "Save Level", Jtrue,
                                                              graphics);
    MenuButton* load_level_button       =   MenuButton_Create(LoadLevel_button,
                                                              100.0f, 340.0f,
                                                              "Load Level", Jtrue,
                                                              graphics);
    MenuButton* options_button          =   MenuButton_Create(Options_button,
                                                              100.0f, 460.0f,
                                                              "Options", Jtrue,
                                                              graphics);
    MenuButton* quit_button              =   MenuButton_Create(Quit_button,
                                                               100.0f, 580.0f,
                                                              "Quit", Jtrue,
                                                              graphics);
    mainMenu->buttons = Vector_Create();
    Vector_Push(&mainMenu->buttons, play_button);
    Vector_Push(&mainMenu->buttons, save_level_button);
    Vector_Push(&mainMenu->buttons, load_level_button);
    Vector_Push(&mainMenu->buttons, options_button);
    Vector_Push(&mainMenu->buttons, quit_button);

    mainMenu->texts = Vector_Create();
    mainMenu->active_textfield = NULL;
    return mainMenu;
}

Menu* OptionMenu_Create(Graphics* graphics)
{
    Menu* optionsMenu = (Menu*)malloc(sizeof(Menu));


    optionsMenu->buttons = Vector_Create();
    optionsMenu->texts = Vector_Create();

    optionsMenu->name                       =   Options_menu;
    //optionsMenu->texts[FPS_options_button]  =   "FPS";

    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 400,
                                                      "Back",
                                                      Jtrue, graphics);
    Vector_Push(&optionsMenu->buttons, back_button);
    optionsMenu->active_textfield = NULL;
    return optionsMenu;
}


Menu* SaveLevelMenu_Create(Graphics* graphics)
{
    Menu* saveMenu = (Menu*)malloc(sizeof(Menu));


    saveMenu->buttons = Vector_Create();
    saveMenu->texts = Vector_Create();
    saveMenu->textfields = Vector_Create();

    saveMenu->name                       =   SaveLevel_menu;
    //optionsMenu->texts[FPS_options_button]  =   "FPS";

    Vector_Push(&saveMenu->textfields, TextField_Create(20, 100, 500, 60));
    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 400,
                                                      "Back",
                                                      Jtrue, graphics);


    Vector_Push(&saveMenu->buttons, back_button);
    saveMenu->active_textfield = NULL;
    return saveMenu;
}

Menu* LoadLevelMenu_Create(Graphics* graphics)
{
    Menu* optionsMenu = (Menu*)malloc(sizeof(Menu));


    optionsMenu->buttons = Vector_Create();
    optionsMenu->texts = Vector_Create();
    optionsMenu->file_list = Vector_Create();

    optionsMenu->name                       =   LoadLevel_menu;

    MenuButton* back_button     =   MenuButton_Create(Back_button,
                                                      100, 400,
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
    if(tf->can_type)
    {
        size_t len = strlen(tf->text);
        if(len < tf->max_char)
        {
            tf->text[len] = c;
            tf->can_type = Jfalse;
            tf->input_timer = 0;
            tf->caretX += 10;
        }

        printf("%s\n", tf->text);
    }
}

void TextField_Update(TextField* tf, int delta, Controls* controls)
{
    TextField_Caret_Update(tf, delta);
    size_t len = strlen(tf->text);

    for(int i = 0 ; i < 200 ; i ++)
    {
        if(controls->pressedKeys[i] == Jtrue &&
           (char)controls->pressedKeys[i] != tf->text[len] &&
           !controls->pressedKeys[SDLK_RETURN] &&
           !controls->pressedKeys[SDLK_BACKSPACE] &&
           !controls->pressedKeys[SDLK_RETURN2])
        {
            TextField_Input(tf, (char)(i));
        }
        else if(controls->pressedKeys[SDLK_BACKSPACE])
        {
            //TextField_Backspace(tf);
        }
    }

    if(tf->input_timer < tf->input_delay)
    {
        tf->input_timer += delta;
    }

    if(tf->input_timer >= tf->input_delay)
    {
        tf->can_type = Jtrue;
    }

}

void TextField_Caret_Update(TextField* tf, int delta)
{
    tf->caret_blinking_timer += delta;
    if(tf->caret_blinking_timer > tf->caret_blinking_delay)
    {
        tf->visible_caret = tf->visible_caret ? Jfalse : Jtrue;
        tf->caret_blinking_timer = 0;
    }
}

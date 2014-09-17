#include "menu.h"
#include "menu_button.h"
#include <stdlib.h>

Menu* MainMenu_Create(Graphics* graphics)
{
    Menu* mainMenu = (Menu*)malloc(sizeof(Menu));

    mainMenu->name                      =   Main_Menu_menu;
    MenuButton* play_button             =   MenuButton_Create(Play_button,
                                                              100.0f, 100.0f,
                                                              "Play", Jtrue,
                                                              graphics);

    MenuButton* save_level_button       =   MenuButton_Create(SaveLevel_button,
                                                              100.0f, 250.0f,
                                                              "Save Level", Jtrue,
                                                              graphics);
    MenuButton* options_button          =   MenuButton_Create(Options_button,
                                                              100.0f, 400.0f,
                                                              "Options", Jtrue,
                                                              graphics);
    MenuButton* quit_button              =   MenuButton_Create(Quit_button,
                                                               100.0f, 550.0f,
                                                              "Quit", Jtrue,
                                                              graphics);
    mainMenu->buttons = Vector_Create();
    Vector_Push(&mainMenu->buttons, play_button);
    Vector_Push(&mainMenu->buttons, save_level_button);
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


TextField* TextField_Create(int x, int y, int width, int height)
{
    TextField* tf = (TextField*)malloc(sizeof(TextField));

    tf->text = malloc(30 * sizeof(tf->text));


    tf->box.left = x;
    tf->box.top = y;
    tf->box.right = x + width;
    tf->box.bottom = y + height;
    tf->box.width = width;
    tf->box.height = height;

    tf->caret_blinking_delay = 400;
    tf->caret_blinking_timer = 0;
    tf->visible_caret = Jtrue;
    tf->caretX = x + 20;
    tf->caretY = y + 20;
    tf->caretWidth = 5;
    tf->caretHeight = 20;
    return tf;
}

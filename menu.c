#include "menu.h"
#include "menu_button.h"


Menu* MainMenu_Create(Graphics* graphics)
{
    Menu* mainMenu = (Menu*)malloc(sizeof(Menu));

    mainMenu->name                      =   Main_Menu_menu;
    MenuButton* play_button             =   MenuButton_Create(Play_button,
                                                              100.0f, 100.0f,
                                                              "Play", Jtrue,
                                                              graphics);

    MenuButton* options_button          =   MenuButton_Create(Options_button,
                                                              100.0f, 250.0f,
                                                              "Options", Jtrue,
                                                              graphics);
    MenuButton* quit_button              =   MenuButton_Create(Quit_button,
                                                               100.0f, 400.0f,
                                                              "Quit", Jtrue,
                                                              graphics);
    mainMenu->buttons = Vector_Create();
    Vector_Push(&mainMenu->buttons, play_button);
    Vector_Push(&mainMenu->buttons, options_button);
    Vector_Push(&mainMenu->buttons, quit_button);

    mainMenu->texts = Vector_Create();

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
    return optionsMenu;
}

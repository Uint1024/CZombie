#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "menu_button.h"
typedef struct Controls Controls;


typedef struct MainMenu
{
    MenuButton buttons[5];

} MainMenu;

MainMenu MainMenu_Create(Graphics* graphics);
void MainMenu_Update(MainMenu* menu, Controls* controls, Jbool* game_started);
#endif // MAIN_MENU_H

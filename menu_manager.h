#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H
#include "common.h"
#include "world.h"

typedef struct Controls Controls;
typedef struct Menu Menu;
typedef struct Graphics Graphics;


typedef struct MenuManager
{
    Menu*           sub_menus[15];
    Menu*           active_menu;
    Menu*           previous_active_menu;

    //to prevent clicking too fast
    int             click_timer;
    Vector          all_buttons;
} MenuManager;



MenuManager MenuManager_Create(Graphics* graphics);
void MenuManager_Update(MenuManager* mm,
                        Controls* controls,
                        Jbool* running,
                        World* world);
void MenuManager_LoadAllButtons(Graphics* graphics, MenuManager* menu_manager);
#endif // MENU_MANAGER_H

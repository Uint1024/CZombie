#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H
#include "common.h"

typedef struct Controls Controls;
typedef struct Menu Menu;
typedef struct Graphics Graphics;


typedef struct MenuManager
{
    Menu*           sub_menus[5];
    Menu*           active_menu;

    //to prevent clicking too fast
    int             click_timer;
} MenuManager;



MenuManager MenuManager_Create(Graphics* graphics);
void MenuManager_Update(MenuManager* mm, Controls* controls, Jbool* game_started, Jbool* running, int delta);

#endif // MENU_MANAGER_H

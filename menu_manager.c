#include "menu_manager.h"
#include "graphics.h"
#include "controls.h"
#include "menu_button.h"
#include "menu.h"

MenuManager MenuManager_Create(Graphics* graphics)
{
    MenuManager mm;

    mm.sub_menus[Main_Menu_menu]    =   MainMenu_Create(graphics);
    mm.sub_menus[Options_menu]      =   OptionMenu_Create(graphics);
    mm.active_menu                  =   mm.sub_menus[Main_Menu_menu];
    mm.click_timer                  =   0;
    return mm;
}


void MenuManager_Update(MenuManager* mm,
                        Controls* controls,
                        Jbool* game_started,
                        Jbool* running,
                        int delta)
{
    mm->click_timer -= delta;

    if(mm->click_timer <= 0)
    {
        Menu* menu = mm->active_menu;

        for(int i = 0 ; i < Vector_Count(&menu->buttons) ; i++)
        {
            MenuButton* button = Vector_Get(&menu->buttons, i);

            button->hover = Jfalse;
            Jbool hover_on_button =
                        BoundingBox_CheckPointCollision(controls->mouseX,
                                                        controls->mouseY,
                                                        &button->box
                                                        );

            if(hover_on_button)
            {
                button->hover = Jtrue;

                if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
                {
                    if(menu->name == Main_Menu_menu)
                    {
                        if(button->name == Play_button)
                        {
                            *game_started = Jtrue;
                        }
                        else if(button->name == Options_button)
                        {
                            mm->active_menu = mm->sub_menus[Options_menu];
                                        button->hover = Jfalse;
                        }
                        else if(button->name == Quit_button)
                        {
                            *running = Jfalse;
                        }
                    }
                    if(menu->name == Options_menu)
                    {
                        if(button->name == Back_button)
                        {
                            mm->active_menu = mm->sub_menus[Main_Menu_menu];
                                        button->hover = Jfalse;
                        }
                    }

                    mm->click_timer         =   200;
                    controls->timer_menu    =   100;
                }
            }


        }

    }
}

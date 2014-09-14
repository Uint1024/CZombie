#include "main_menu.h"
#include "graphics.h"
#include "controls.h"
#include "menu_button.h"

MainMenu MainMenu_Create(Graphics* graphics)
{
    MainMenu menu;

    menu.buttons[Play]      =   MenuButton_Create(100.0f, 100.0f, "Play", Jtrue, graphics);
    menu.buttons[Options]   =   MenuButton_Create(100.0f, 400.0f, "Options", Jtrue, graphics);
    menu.buttons[Quit]      =   MenuButton_Create(100.0f, 500.0f, "Quit", Jtrue, graphics);

    return menu;
}


void MainMenu_Update(MainMenu* menu, Controls* controls, Jbool* game_started)
{
    if(controls->pressedMouseButtons[SDL_BUTTON_LEFT])
    {
        for(int i = 0 ; i < TOTAL_MAIN_MENU_BUTTONS ; i++)
        {
            Jbool clicked_on_button =
                        BoundingBox_CheckPointCollision(controls->mouseX,
                                                        controls->mouseY,
                                                        &menu->buttons[i].box
                                                        );

            if(clicked_on_button && i == Play)
            {
                *game_started = Jtrue;
            }
        }
    }
}

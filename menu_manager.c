#include <stdio.h>
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
   mm.sub_menus[SaveLevel_menu]      =   SaveLevelMenu_Create(graphics);
    mm.active_menu                  =   mm.sub_menus[Main_Menu_menu];
    mm.click_timer                  =   0;



    return mm;
}

void MenuManager_Update(MenuManager* mm,
                        Controls* controls,
                        Jbool* game_started,
                        Jbool* running,
                        int delta,
                        World* world)
{
    Menu* menu = mm->active_menu;
    mm->click_timer -= delta;

    for(int i = 0 ; i < Vector_Count(&menu->textfields) ; i++)
    {
        TextField* tf = Vector_Get(&menu->textfields, i);

        if(BoundingBox_CheckPointCollision(controls->mouseX,
                                            controls->mouseY,
                                            &tf->box
                                            ))
        {
            menu->active_textfield = tf;

        }
    }

    if(menu->name == SaveLevel_menu &&
       menu->active_textfield != NULL)
    {
        TextField_Update( menu->active_textfield , delta, controls);

        if(controls->pressedKeys[SDLK_RETURN])
        {
            FILE *save_file;
            save_file = fopen(menu->active_textfield->text, "wb");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fwrite(&world->map[i], sizeof(Entity), 1, save_file);
            }

            fclose(save_file);
        }
        /*if (controls->pressedKeys[SDL_SCANCODE_H] == Jtrue)
        {
            FILE *save_file;
            save_file = fopen("test.txt", "wb");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fwrite(&world->map[i], sizeof(Entity), 1, save_file);
            }

            fclose(save_file);
        }

        if (controls->pressedKeys[SDL_SCANCODE_G] == Jtrue)
        {
            FILE *save_file;
            save_file = fopen("test.txt", "rb");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fread(&world->map[i], sizeof(Entity), 1, save_file);
            }

            fclose(save_file);
        }*/
    }


    if(mm->click_timer <= 0)
    {
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
                        else if(button->name == SaveLevel_button)
                        {
                            mm->active_menu = mm->sub_menus[SaveLevel_menu];
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

                    if(menu->name == SaveLevel_menu)
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

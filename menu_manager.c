#include <stdio.h>
#include "string.h"
#include "levelEditor.h"
#include "menu_manager.h"
#include "graphics.h"
#include "controls.h"
#include "menu_button.h"
#include "menu.h"
#include "dirent.h"


MenuManager MenuManager_Create(Graphics* graphics)
{
    MenuManager mm;

    mm.sub_menus[Main_Menu_menu]    =   MainMenu_Create(graphics);
    mm.sub_menus[Options_menu]      =   OptionMenu_Create(graphics);
   mm.sub_menus[SaveLevel_menu]      =   SaveLevelMenu_Create(graphics);
   mm.sub_menus[LoadLevel_menu]      =   LoadLevelMenu_Create(graphics);
    mm.active_menu                  =   mm.sub_menus[Main_Menu_menu];
    mm.click_timer                  =   0;

    return mm;
}

void MenuManager_Update(MenuManager* mm,
                        Controls* controls,
                        Jbool* running,
                        World* world)
{
    Menu* menu = mm->active_menu;
    mm->click_timer -= delta_g;

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
        TextField_Update( menu->active_textfield, controls);

        if(controls->pressedKeys[SDLK_RETURN])
        {
            char file_name[50];
            strcpy(file_name, "saves/");
            strcat(file_name, menu->active_textfield->text);
            strcat(file_name, ".sav");
            Level_Save(file_name, world);

        }
    }
    else if(menu->name == LoadLevel_menu)
    {
        Vector_Clear(&menu->file_list);

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir ("saves\\")) != NULL) {
                int i = 0;
          while ((ent = readdir (dir)) != NULL) {
                if(ent->d_name[0] != '.')
                {
                    char** name = malloc(sizeof(ent->d_name));
                    strcpy(name, ent->d_name);

                    Vector_Push(&menu->file_list, FileNameButton_Create(i * 80 + 50, name));
                    i++;
                }

          }
          closedir (dir);
        } else {
          printf ("Can't find file\n");
        }

        for(int i = 0 ; i < Vector_Count(&menu->file_list) ; i++)
        {
            MenuButton* button = (MenuButton*)Vector_Get(&menu->file_list, i);

            //if clicking on file name
            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &button->box))
            {
                char* complete_name = malloc(sizeof(button->text) + 6);
                strcpy(complete_name, "saves/");
                strcat(complete_name, button->text);

                Level_Load(complete_name, world);
            }
        }

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
                            display_menu_g = Jfalse;
                            game_state_g = Playing;
                        }
                        else if(button->name == LevelEditor_button)
                        {
                            display_menu_g = Jfalse;
                            //Game_SwitchToLevelEditor();
                            world->player.visible = Jfalse;
                            world->player.solid = Jfalse;
                            game_state_g = Level_Editor;
                            MainMenu_LoadLevelEditorMainMenu(mm->sub_menus[Main_Menu_menu]);

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
                        else if(button->name == LoadLevel_button)
                        {
                            mm->active_menu = mm->sub_menus[LoadLevel_menu];
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

                    if(menu->name == LoadLevel_menu)
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

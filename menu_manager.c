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

    mm.all_buttons = Vector_Create();
    MenuManager_LoadAllButtons(graphics, &mm);

    mm.sub_menus[MainMenu_menu]    =   MainMenu_Create(&mm);
        mm.sub_menus[LevelEditorEditing_menu]   =   LevelEditorEditing_Create(&mm);
    //mm.sub_menus[Options_menu]      =   OptionMenu_Create(graphics);
    mm.sub_menus[SaveMap_menu]      =   SaveLevelMenu_Create(graphics);
    mm.sub_menus[LoadMap_menu]      =   LoadLevelMenu_Create(graphics);
    mm.sub_menus[LevelEditor_menu]   =   LevelEditorMainMenu_Create(&mm);


    mm.active_menu                  =   mm.sub_menus[MainMenu_menu];
    mm.previous_active_menu = NULL;
    mm.click_timer                  =   0;

    return mm;
}

void MenuManager_LoadAllButtons(Graphics* graphics, MenuManager* menu_manager)
{
    Menu_Button_Name Menu_Button_Name[11] = {Play_button,
                                            LevelEditor_button,
                                            NewMap_button,
                                            LoadMap_button,
                                            SaveMap_button,
                                            SaveGame_button,
                                            LoadGame_button,
                                            Exit_Level_Editor_button,
                                            Options_button,
                                            Quit_button,
                                            Back_button};

    char* text[11] = {"Play", "Level Editor", "New Map", "Load Map", "Save Map",
                        "Save Game", "Load Game",
                        "Exit Level Editor", "Options", "Quit", "Back"};

    for(int i = 0 ; i < 11 ; i++)
    {
        MenuButton* button = MenuButton_Create(Menu_Button_Name[i],
                                      100.0f, 100.0f,
                                      text[i], Jtrue,
                                      graphics);

        Vector_Push(&menu_manager->all_buttons,
                    button
                    );
    }
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

    //typing in the map name textfield
    if(menu->name == SaveMap_menu &&
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

    //map load menu -> loading all the saves in the saves/ folder
    else if(menu->name == LoadMap_menu)
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

            //if clicking on file name, we load the save
            if(mm->click_timer <= 0 &&
               controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &button->box))
            {
                char* complete_name = malloc(sizeof(button->text) + 6);
                strcpy(complete_name, "saves/");
                strcat(complete_name, button->text);

                //TODO :
                //determine if we're loading the file to modify it or play it
                //if we want to modify it, configure the game to level editor mode...
                //problem : how do I know why the user wants to load it??
                display_menu_g = Jfalse;
                game_state_g = GameState_Editing_Map;
                world->player.visible = Jfalse;
                world->player.solid = Jfalse;
                mm->active_menu = mm->sub_menus[LevelEditorEditing_menu];


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

                    if(button->name == Play_button)
                    {
                        display_menu_g = Jfalse;
                        game_state_g = GameState_Playing;
                    }
                    else if(button->name == LevelEditor_button)
                    {
                        mm->previous_active_menu = mm->active_menu;
                        mm->active_menu = mm->sub_menus[LevelEditor_menu];
                    }
                    if(button->name == SaveMap_button)
                    {
                        mm->previous_active_menu = mm->active_menu;
                        mm->active_menu = mm->sub_menus[SaveMap_menu];
                        button->hover = Jfalse;
                    }
                    else if(button->name == LoadMap_button)
                    {
                        mm->previous_active_menu = mm->active_menu;
                        mm->active_menu = mm->sub_menus[LoadMap_menu];
                        button->hover = Jfalse;
                    }
                    else if(button->name == Quit_button)
                    {
                        *running = Jfalse;
                    }
                    else if(button->name == NewMap_button)
                    {
                        World_Reset(world, 70, 70);
                        mm->previous_active_menu = mm->active_menu;
                        mm->active_menu = mm->sub_menus[LevelEditorEditing_menu];

                        display_menu_g = Jfalse;
                        world->player.visible = Jfalse;
                        world->player.solid = Jfalse;
                        game_state_g = GameState_Editing_Map;

                    }
                    else if(button->name == Back_button)
                    {
                        mm->active_menu = mm->previous_active_menu;
                    }
                    else if(button->name == Exit_Level_Editor_button)
                    {
                        mm->active_menu = mm->sub_menus[MainMenu_menu];
                    }

                    mm->click_timer         =   200;
                    controls->timer_menu    =   100;
                }
            }


        }

    }
}

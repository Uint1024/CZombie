#include <stdio.h>
#include "string.h"
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
            char file_name[50];
            strcpy(file_name, "saves/");
            strcat(file_name, menu->active_textfield->text);

            FILE *save_file;
            save_file = fopen(file_name, "w");
            if(!save_file)
            {
                printf("Can't open file");

            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fwrite(world->map[i], sizeof(Entity), 1, save_file);
                printf("writing walls %d\n", ftell(save_file));
            }
            for(int i = 0 ; i < world->map_size ; i++)
            {
                fwrite(world->ground_map[i], sizeof(Entity), 1, save_file);
                printf("writing ground %d\n", ftell(save_file));

            }

            int num_of_zombies = Vector_Count(&world->monsters_vector);
            fwrite(&num_of_zombies, sizeof(int), 1, save_file);

            for(int i = 0 ; i < num_of_zombies ; i++)
            {
                Entity* buffer = (Entity*)Vector_Get(&world->monsters_vector, i);

                fwrite(buffer, sizeof(Entity), 1, save_file);
                if(buffer->weapons_component != NULL)
                {
                    fwrite(buffer->weapons_component, sizeof(WeaponsComponent),
                            1, save_file);
                }
            }

            fclose(save_file);
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
          perror ("");
          return EXIT_FAILURE;
        }

        for(int i = 0 ; i < Vector_Count(&menu->file_list) ; i++)
        {
            MenuButton* button = (MenuButton*)Vector_Get(&menu->file_list, i);

            //if clicking on file name
            if(controls->pressedMouseButtons[SDL_BUTTON_LEFT] &&
               BoundingBox_CheckPointCollision(controls->mouseX, controls->mouseY, &button->box))
            {
                Vector_Clear(&world->monsters_vector);
                Vector_Clear(&world->bonus_vector);

                //LOADING LEVEL
                FILE *save_file;
                char* complete_name = malloc(sizeof(button->text) + 6);
                strcpy(complete_name, "saves/");
                strcat(complete_name, button->text);
                save_file = fopen(complete_name, "r");
                if(!save_file)
                {
                    printf("Can't open file");

                }

                for(int i = 0 ; i < world->map_size ; i++)
                {
                    Entity* buffer = Entity_Spawn();
                    fread(buffer, sizeof(Entity), 1, save_file);

                    world->map[i] = buffer;
                }

                for(int i = 0 ; i < world->map_size ; i++)
                {
                    Entity* buffer = Entity_Spawn();
                    fread(buffer, sizeof(Entity), 1, save_file);


                    world->ground_map[i] = buffer;
                }

                int num_of_zombies = 0;
                fread(&num_of_zombies, sizeof(int), 1, save_file);

                for(int i = 0 ; i < num_of_zombies ; i++)
                {
                    Entity* buffer = Entity_Spawn();

                    //(Entity*)Vector_Get(&world->monsters_vector, i);

                    fread(buffer, sizeof(Entity), 1, save_file);
                    if(buffer->weapons_component != NULL)
                    {
                        WeaponsComponent* wc_buffer = WeaponsComponent_Create(Jtrue);
                        fread(wc_buffer, sizeof(WeaponsComponent), 1, save_file);
                        buffer->weapons_component = wc_buffer;
                    }

                    Vector_Push(&world->monsters_vector, buffer);
                }

                printf("loading");
                fclose(save_file);

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

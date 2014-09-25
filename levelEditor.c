#include "levelEditor.h"
#include "stdio.h"
#include "world.h"
#include "vector.h"
#include "zombie.h"
#include "weapon.h"
#include "weapons_component.h"
#include "movement_component.h"
#include "player.h"
#include "gameManager.h"

static int building_time = 0;

void LevelEditor_CreateObject(Main_Category category, int obj_type, int x, int y,
                              int position_in_array, int mousePositionInWorldX,
                              int mousePositionInWorldY, World* world, bool unlimited)
{
    if(category == Cat_Wall || category == Cat_Door)
    {
        free(world->map[position_in_array]);
        world->map[position_in_array] = Entity_Create(category, obj_type, x, y, 0);
    }

    else if(category == Cat_Ground)
    {
        Entity_Destroy(world->ground_map[position_in_array]);
        free(world->ground_map[position_in_array]);
        world->ground_map[position_in_array] = Entity_Create(category, obj_type, x, y, 0);
    }

    else if(category == Cat_Zombie &&
       (SDL_GetTicks() - building_time > 150 || unlimited))
    {
        Vector_Push(&world->monsters_vector,
                   Entity_Create(category, obj_type,
                                 mousePositionInWorldX,
                                 mousePositionInWorldY,
                                 0));

        building_time = SDL_GetTicks();
    }
    else if(category == Cat_Event &&
            (SDL_GetTicks() - building_time > 150))
    {
        //there can be only 1 player start and end of level
        for(int i = 0 ; i < Vector_Count(&world->events_vector) ; i++)
        {
            Entity* map_event = (Entity*)Vector_Get(&world->events_vector, i);
            if((obj_type == Event_Player_Start || obj_type == Event_End_Level )&&
               obj_type == map_event->sub_category)
            {
                Vector_Delete(&world->events_vector, i);
            }
        }
        Vector_Push(&world->events_vector, Entity_Create(category, obj_type, x, y, 0));
        building_time = SDL_GetTicks();
    }
    else if(category == Cat_Bonus && (SDL_GetTicks() - building_time > 150 ||
                                       unlimited))
    {
        Vector_Push(&world->bonus_vector, Entity_Create(category, obj_type, x, y, 0));
        building_time = SDL_GetTicks();
    }

}
void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer)
{

    fwrite(&buffer->sub_category, sizeof(int), 1, save_file);
    fwrite(&buffer->damage, sizeof(float), 1, save_file);
    fwrite(&buffer->x, sizeof(float), 1, save_file);
    fwrite(&buffer->y, sizeof(float), 1, save_file);
    fwrite(&buffer->t, sizeof(Main_Category), 1, save_file);
    fwrite(&buffer->box, sizeof(Box), 1, save_file);
    fwrite(&buffer->alive, sizeof(bool), 1, save_file);
    fwrite(&buffer->alive_timer, sizeof(bool), 1, save_file);
    fwrite(&buffer->is_ennemy, sizeof(bool), 1, save_file);
    fwrite(&buffer->hp, sizeof(int), 1, save_file);
    fwrite(&buffer->solid, sizeof(bool), 1, save_file);
    fwrite(&buffer->collision_direction, sizeof(Direction), 1, save_file);

    if(buffer->movementC != NULL)
    {
        bool has_movementC = true;
        fwrite(&has_movementC, sizeof(bool), 1, save_file);
        MovementC* mc = buffer->movementC;
        fwrite(&mc->angle, sizeof(float), 1, save_file);
        fwrite(&mc->speed, sizeof(float), 1, save_file);
        fwrite(&mc->dx, sizeof(float), 1, save_file);
        fwrite(&mc->dy, sizeof(float), 1, save_file);
    }
    else
    {
        bool has_movementC = false;
        fwrite(&has_movementC, sizeof(bool), 1, save_file);
    }

    if(buffer->zombieC != NULL)
    {
        bool is_zombie = true;
        fwrite(&is_zombie, sizeof(bool), 1, save_file);

        ZombieC* zc = buffer->zombieC;
        fwrite(&zc->aggressive, sizeof(bool), 1, save_file);
        fwrite(&zc->idling, sizeof(bool), 1, save_file);
        fwrite(&zc->rand_move_every, sizeof(int), 1, save_file);
        fwrite(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fwrite(&zc->vision_distance, sizeof(int), 1, save_file);
        fwrite(&zc->vision_width, sizeof(float), 1, save_file);
        fwrite(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
    }
    else
    {
        bool is_zombie = false;
        fwrite(&is_zombie, sizeof(bool), 1, save_file);
    }

    if(buffer->weaponsC != NULL)
    {
        bool has_weaponC = true;
        fwrite(&has_weaponC, sizeof(bool), 1, save_file);

        WeaponsC* wc = buffer->weaponsC;
        for(int i = 0 ; i < NB_WEAPON_TYPES ; i++)
        {
            fwrite(&wc->bullets[i], sizeof(int), 1, save_file);
        }
        fwrite(&wc->is_monster, sizeof(bool), 1, save_file);
        fwrite(&wc->last_reload, sizeof(int), 1, save_file);
        fwrite(&wc->reloading, sizeof(bool), 1, save_file);
        fwrite(&wc->reload_timer, sizeof(int), 1, save_file);

        //we write only the weapon type, then in the loading function
        //we'll find the weapon in the inventory and set the pointer to it
        fwrite(&wc->current_weapon->type, sizeof(Weapon_Type), 1, save_file);

        //to fwrite the weapon inventory, we have to loop through the inventory array
        //and write every individual weapon... but first we have to count them and fwrite
        //the number of weapons or else we won't know how much to fread
        int nb_of_weapons = 0;

        for(int i = 0 ; i < NB_WEAPON_TYPES ; i++)
        {
            if(wc->weapons_inventory[i] != NULL)
            {
                nb_of_weapons ++;
            }
        }

        fwrite(&nb_of_weapons, sizeof(int), 1, save_file);

        for(int i = 0 ; i < NB_WEAPON_TYPES ; i++)
        {
            if(wc->weapons_inventory[i] != NULL)
            {
                //we only save these 3 variables because we can load the rest with Weapon_Create

                //first we write the weapon type, because in fread we'll need it to call Weapon_Create
                fwrite(&wc->weapons_inventory[i]->type, sizeof(Weapon_Type), 1, save_file);
                fwrite(&wc->weapons_inventory[i]->last_shot, sizeof(int), 1, save_file);
                fwrite(&wc->weapons_inventory[i]->magazine_bullets, sizeof(int), 1, save_file);

            }
        }

    }
    else
    {
        bool has_weaponC = false;
        fwrite(&has_weaponC, sizeof(bool), 1, save_file);
    }
}

void LevelEditor_ReadEntity(FILE* save_file, Entity* buffer)
{
    fread(&buffer->sub_category, sizeof(int), 1, save_file);
    fread(&buffer->damage, sizeof(float), 1, save_file);
    fread(&buffer->x, sizeof(float), 1, save_file);
    fread(&buffer->y, sizeof(float), 1, save_file);
    fread(&buffer->t, sizeof(Main_Category), 1, save_file);
    fread(&buffer->box, sizeof(Box), 1, save_file);
    fread(&buffer->alive, sizeof(bool), 1, save_file);
    fread(&buffer->alive_timer, sizeof(bool), 1, save_file);
    fread(&buffer->is_ennemy, sizeof(bool), 1, save_file);
    fread(&buffer->hp, sizeof(int), 1, save_file);
    fread(&buffer->solid, sizeof(bool), 1, save_file);
    fread(&buffer->collision_direction, sizeof(Direction), 1, save_file);

    bool has_movementC = false;
    fread(&has_movementC, sizeof(bool), 1, save_file);

    if(has_movementC != false)
    {
        MovementC* mc = MovementC_Create();
        fread(&mc->angle, sizeof(float), 1, save_file);
        fread(&mc->speed, sizeof(float), 1, save_file);
        fread(&mc->dx, sizeof(float), 1, save_file);
        fread(&mc->dy, sizeof(float), 1, save_file);
        buffer->movementC = mc;
    }


    bool is_zombie = false;
    fread(&is_zombie, sizeof(bool), 1, save_file);

    if(is_zombie != false)
    {
        ZombieC* zc = ZombieC_Create();

        fread(&zc->aggressive, sizeof(bool), 1, save_file);
        fread(&zc->idling, sizeof(bool), 1, save_file);

        fread(&zc->rand_move_every, sizeof(int), 1, save_file);
        fread(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fread(&zc->vision_distance, sizeof(int), 1, save_file);
        fread(&zc->vision_width, sizeof(float), 1, save_file);
        fread(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
        buffer->zombieC = zc;

    }

    bool has_weaponC = true;
    fread(&has_weaponC, sizeof(bool), 1, save_file);
    if(has_weaponC)
    {
        WeaponsC* wc = WeaponsComponent_Create();
        for(int i = 0 ; i < NB_WEAPON_TYPES ; i++)
        {
            fread(&wc->bullets[i], sizeof(int), 1, save_file);
        }
        fread(&wc->is_monster, sizeof(bool), 1, save_file);
        fread(&wc->last_reload, sizeof(int), 1, save_file);
        fread(&wc->reloading, sizeof(bool), 1, save_file);
        fread(&wc->reload_timer, sizeof(int), 1, save_file);


        //we read only the weapon type, then
        //we'll find the weapon in the inventory and set the pointer to it
        Weapon_Type current_weapon = No_Weapon;
        fread(&current_weapon, sizeof(Weapon_Type), 1, save_file);


        int nb_of_weapons = 0;

        fread(&nb_of_weapons, sizeof(int), 1, save_file);

        for(int i = 0 ; i < nb_of_weapons ; i++)
        {
            Weapon_Type weapon_type = No_Weapon;
            fread(&weapon_type, sizeof(Weapon_Type), 1, save_file);

            Weapon* weapon = Weapon_Create(weapon_type);

            fread(&weapon->last_shot, sizeof(int), 1, save_file);
            fread(&weapon->magazine_bullets, sizeof(int), 1, save_file);

            if(weapon_type == current_weapon)
            {
                wc->current_weapon = weapon;
            }

            weapon->parent = wc;

            //adding the weapon to the inventory
            WeaponsComponent_AddWeaponToInventory(wc, weapon);
        }

        buffer->weaponsC = wc;
    }
}

void Level_Save(char* file_name, World* w)
{
        FILE *save_file;
        save_file = fopen(file_name, "wb");
        if(!save_file)
        {
            printf("Can't open file\n");

        }

        printf("SAVING\n");

        //the NULL walls aren't written
        //we need to tell fread how much to read
        printf("ftell nb_walls : %d\n", ftell(save_file));
        int nb_of_walls = 0;

        for(int i = 0 ; i < w->map_size ; i++)
        {
            if(w->map[i] != NULL)
            {
                nb_of_walls++;
            }
        }

        printf("%d walls", nb_of_walls);

        fwrite(&nb_of_walls, sizeof(int), 1, save_file);


        for(int i = 0 ; i < w->map_size ; i++)
        {
            fwrite(w->map[i], sizeof(Entity), 1, save_file);
        }

        //there are no NULL ground
        printf("ftell ground : %d\n", ftell(save_file));
        for(int i = 0 ; i < w->map_size ; i++)
        {
            fwrite(w->ground_map[i], sizeof(Entity), 1, save_file);
        }

        int num_of_events = Vector_Count(&w->events_vector);
        fwrite(&num_of_events, sizeof(int), 1, save_file);
        for(int i = 0 ; i < num_of_events ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->events_vector, i);
            LevelEditor_WriteEntity(save_file, buffer);
        }

        int num_of_zombies = Vector_Count(&w->monsters_vector);

        printf("saving %d zombies", num_of_zombies);
        fwrite(&num_of_zombies, sizeof(int), 1, save_file);

        for(int i = 0 ; i < num_of_zombies ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->monsters_vector, i);
            LevelEditor_WriteEntity(save_file, buffer);
        }

        int num_of_bonus = Vector_Count(&w->bonus_vector);
        fwrite(&num_of_bonus, sizeof(int), 1, save_file);

        for(int i = 0 ; i < num_of_bonus ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->bonus_vector, i);
            LevelEditor_WriteEntity(save_file, buffer);
        }

        fclose(save_file);
}

//pressing F7 to quickly try a level inside the level editor
void LevelEditor_QuickTry(World* world)
{
    game_state_g = GameState_Map_Editor_Testing_Level;
    Game_StartMap(world);
    world->player.movementC->speed = BASE_PLAYER_SPEED;
    Level_Save("saves/tempLevelEditor.sav", world);
}

//quitting the "quick try" mode : loading the temp save and returning to level editor
void LevelEditor_BackToEditing(World* world)
{
    display_menu_g = false;
    game_state_g = GameState_Editing_Map;
    world->player.visible = false;
    world->player.solid = false;
    Level_Load("saves/tempLevelEditor.sav", world);
}

void LevelEditor_LoadMapToEdit(char* complete_name, World* world)
{
    display_menu_g = false;
    game_state_g = GameState_Editing_Map;

    world->player.visible = false;
    world->player.solid = false;
    //mm->active_menu = mm->sub_menus[LevelEditorEditing_menu];


    Level_Load(complete_name, world);
}

void Level_Clear(World* w)
{
    for(int i = 0 ; i < Vector_Count(&w->monsters_vector) ; i++)
    {
        Entity* ent = Vector_Get(&w->monsters_vector, i);
        Entity_Destroy(ent);
    }
    for(int i = 0 ; i < Vector_Count(&w->bonus_vector) ; i++)
    {
        Entity* ent = Vector_Get(&w->bonus_vector, i);
        Entity_Destroy(ent);

    }
    for(int i = 0 ; i < Vector_Count(&w->bullets_vector) ; i++)
    {
        Entity* ent = Vector_Get(&w->bullets_vector, i);
        Entity_Destroy(ent);
    }
    for(int i = 0 ; i < Vector_Count(&w->explosions_vector) ; i++)
    {
        Entity* ent = Vector_Get(&w->explosions_vector, i);
        Entity_Destroy(ent);
    }

    Vector_Clear(&w->monsters_vector);
    Vector_Clear(&w->bonus_vector);
    Vector_Clear(&w->events_vector);
    Vector_Clear(&w->bullets_vector);
    Vector_Clear(&w->explosions_vector);
    Vector_Clear(&w->decals_vector);

    for(int i = 0 ; i < w->map_size ; i++)
    {
        free(w->map[i]);

    }

    for(int i = 0 ; i < w->map_size ; i++)
    {
        free(w->ground_map[i]);
    }

    //Player_Reset(&w->player);

}
void Level_Load(char* file_name, World* w)
{
    Level_Clear(w);

    printf("LOADING\n");
    FILE *save_file;

    save_file = fopen(file_name, "rb");
    if(!save_file)
    {
        printf("Can't open file");

    }

    int nb_of_walls = 0;
    printf("ftell nb_walls : %d\n", ftell(save_file));
    fread(&nb_of_walls, sizeof(int), 1, save_file);
    printf("%d walls\n", nb_of_walls);
    for(int i = 0 ; i < nb_of_walls ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity)); //Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);

        //always relative to the x and y coordinate
        int position_in_array = buffer->x / TILE_SIZE + ((buffer->y/ TILE_SIZE) * w->map_width);
        w->map[position_in_array] = buffer;
    }

printf("ftell ground : %d\n", ftell(save_file));
    for(int i = 0 ; i < w->map_size ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity));//Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);

        w->ground_map[i] = buffer;
    }


    int num_of_events = 0;

    fread(&num_of_events, sizeof(int), 1, save_file);


    for(int i = 0 ; i < num_of_events ; i++)
    {
        Entity* buffer = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer);

        Vector_Push(&w->events_vector, buffer);

    }


    int num_of_zombies = 0;
    fread(&num_of_zombies, sizeof(int), 1, save_file);
    printf("%d zombies\n", num_of_zombies);
    //"temporary" fix: fread sometimes read 1 instead of 0. I don't know why?
    //of course this will completely crash if there's really 1 zombie
    /*if(num_of_zombies == 1)
    {
        num_of_zombies = 0;
   }*/

    if(num_of_zombies != 0)
    {
        for(int i = 0 ; i < num_of_zombies ; i++)
        {
            Entity* buffer = Entity_Spawn();
            LevelEditor_ReadEntity(save_file, buffer);
            Vector_Push(&w->monsters_vector, buffer);

        }
    }

    int num_of_bonus = 0;
    fread(&num_of_bonus, sizeof(int), 1, save_file);
    for(int i = 0 ; i < num_of_bonus ; i++)
    {
        Entity* buffer = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer);
        Vector_Push(&w->bonus_vector, buffer);

    }




    fclose(save_file);
}

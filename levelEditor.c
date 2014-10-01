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
#include "entity.h"
#include "prop.h"



static int building_time = 0;
static int test = 0;

void LevelEditor_CreateObject(Main_Category category, int obj_type, int x, int y,
                              int position_in_array, float mousePositionInWorldX,
                              float mousePositionInWorldY, World* world, bool unlimited,
                              float angle)
{

    if(category == Cat_Wall || category == Cat_Door)
    {
        free(world->map[position_in_array]);
        world->map[position_in_array] = Entity_Create(category, obj_type, x, y, angle, world);
    }

    else if(category == Cat_Ground)
    {
        Entity_Destroy(world->ground_map[position_in_array]);
        free(world->ground_map[position_in_array]);
        world->ground_map[position_in_array] = Entity_Create(category, obj_type, x, y, angle, world);
    }

    else if(category == Cat_Zombie &&
       (SDL_GetTicks() - building_time > 150 || unlimited))
    {
        Vector_Push(&world->monsters_vector,
                   Entity_Create(category, obj_type,
                                 mousePositionInWorldX,
                                 mousePositionInWorldY,
                                 0, world));

        building_time = SDL_GetTicks();
    }
    else if(category == Cat_Prop &&
       (SDL_GetTicks() - building_time > 150 || unlimited))
    {
        Vector_Push(&world->props_vector,
                   Entity_Create(category, obj_type,
                                 mousePositionInWorldX,
                                 mousePositionInWorldY,
                                 angle, world));

        building_time = SDL_GetTicks();
    }
    else if(category == Cat_Decal &&
       (SDL_GetTicks() - building_time > 150 || unlimited))
    {
        Vector_Push(&world->decals_vector,
                   Entity_Create(category, obj_type,
                                 mousePositionInWorldX,
                                 mousePositionInWorldY,
                                 angle, world));

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
        Vector_Push(&world->events_vector, Entity_Create(category, obj_type, x, y, 0, world));
        building_time = SDL_GetTicks();
    }
    else if(category == Cat_Bonus && (SDL_GetTicks() - building_time > 150 ||
                                       unlimited))
    {
        Vector_Push(&world->bonus_vector, Entity_Create(category, obj_type, mousePositionInWorldX, mousePositionInWorldY, 0, world));
        building_time = SDL_GetTicks();
    }

}

/*update the walls corner around a wall*/
void LevelEditor_UpdateWallsCorners(int map_position, World* world)
{

    Entity** map = world->map;

   // int map_position = (wall->y/TILE_SIZE) * world->map_width + wall->x/TILE_SIZE;


    Entity* map_left = map[map_position - 1];
    Entity* map_left_left = map[map_position - 2];
    Entity* map_left_up = map[map_position - 1 - world->map_width];
    Entity* map_left_down = map[map_position - 1 + world->map_width];


    Entity* map_right = map[map_position + 1];
    Entity* map_right_up = map[map_position + 1  - world->map_width];
    Entity* map_right_down = map[map_position + 1 + world->map_width];

    Entity* map_up = map[map_position - world->map_width];
    Entity* map_up_up = map[map_position - 2 * world->map_width];
    Entity* map_down = map[map_position + world->map_width];
    Entity* map_down_down = map[map_position + 2 * world->map_width];



    if(map_left != NULL)
    Wall_Update_Tile_Type(map_left, world);
    if(map_left_up != NULL)
    Wall_Update_Tile_Type(map_left_up, world);
    if(map_left_down != NULL)
    Wall_Update_Tile_Type(map_left_down, world);
    if(map_right != NULL)
    Wall_Update_Tile_Type(map_right, world);
    if(map_right_up != NULL)
    Wall_Update_Tile_Type(map_right_up, world);
    if(map_right_down != NULL)
    Wall_Update_Tile_Type(map_right_down, world);
    if(map_up != NULL)
    Wall_Update_Tile_Type(map_up, world);
    if(map_down != NULL)
    Wall_Update_Tile_Type(map_down, world);

}
void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer)
{
    fwrite(&buffer->t, sizeof(Main_Category), 1, save_file);
    fwrite(&buffer->sub_category, sizeof(int), 1, save_file);
    fwrite(&buffer->x, sizeof(float), 1, save_file);
    fwrite(&buffer->y, sizeof(float), 1, save_file);
    //printf("writing entity at %f %f")

    fwrite(&buffer->angle, sizeof(float), 1, save_file);


    fwrite(&buffer->box, sizeof(Box), 1, save_file);
    fwrite(&buffer->alive, sizeof(bool), 1, save_file);
    fwrite(&buffer->alive_timer, sizeof(bool), 1, save_file);
    fwrite(&buffer->is_ennemy, sizeof(bool), 1, save_file);
    fwrite(&buffer->solid, sizeof(bool), 1, save_file);
    fwrite(&buffer->collision_direction, sizeof(Direction), 1, save_file);

    if(buffer->movementC != NULL)
    {
        bool has_movementC = true;
        fwrite(&has_movementC, sizeof(bool), 1, save_file);
        MovementC* mc = buffer->movementC;
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
        fwrite(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fwrite(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
        fwrite(&zc->dodging_timer, sizeof(int), 1, save_file);
        fwrite(&zc->ai_timer, sizeof(int), 1, save_file);
        fwrite(&zc->attack_timer, sizeof(int), 1, save_file);
        fwrite(&zc->shoot_timer, sizeof(int), 1, save_file);
        fwrite(&zc->pattern_timer, sizeof(int), 1, save_file);
        fwrite(&zc->shoots_fired_in_pattern, sizeof(int), 1, save_file);
        fwrite(&zc->pattern_direction_right, sizeof(bool), 1, save_file);
        fwrite(&zc->spot_timer, sizeof(bool), 1, save_file);
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

        /*we write only the weapon type, then in the loading function
        we'll find the weapon in the inventory and set the pointer to it*/
        fwrite(&wc->current_weapon->type, sizeof(Weapon_Type), 1, save_file);

        /*to fwrite the weapon inventory, we have to loop through the inventory array
        and write every individual weapon... but first we have to count them and fwrite
        the number of weapons or else we won't know how much to fread*/
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

void LevelEditor_ReadEntity(FILE* save_file, Entity* buffer, World* world)
{
    Main_Category main_cat;
    int sub_category;
    float angle, x, y;

    fread(&main_cat, sizeof(Main_Category), 1, save_file);
    fread(&sub_category, sizeof(int), 1, save_file);
    fread(&x, sizeof(float), 1, save_file);
    fread(&y, sizeof(float), 1, save_file);
    fread(&angle, sizeof(float), 1, save_file);

    free(buffer);
    buffer = Entity_Create(main_cat, sub_category, x, y, angle, world);




    fread(&buffer->box, sizeof(Box), 1, save_file);
    fread(&buffer->alive, sizeof(bool), 1, save_file);
    fread(&buffer->alive_timer, sizeof(bool), 1, save_file);
    fread(&buffer->is_ennemy, sizeof(bool), 1, save_file);
    fread(&buffer->solid, sizeof(bool), 1, save_file);
    fread(&buffer->collision_direction, sizeof(Direction), 1, save_file);


    bool has_movementC = false;
    fread(&has_movementC, sizeof(bool), 1, save_file);

    if(has_movementC != false)
    {
        MovementC* mc = buffer->movementC;
        fread(&mc->speed, sizeof(float), 1, save_file);
        fread(&mc->dx, sizeof(float), 1, save_file);
        fread(&mc->dy, sizeof(float), 1, save_file);
    }


    bool is_zombie = false;
    fread(&is_zombie, sizeof(bool), 1, save_file);
    if(is_zombie != false)
    {
        ZombieC* zc = buffer->zombieC;

        fread(&zc->aggressive, sizeof(bool), 1, save_file);
        fread(&zc->idling, sizeof(bool), 1, save_file);
        fread(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fread(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
        fread(&zc->dodging_timer, sizeof(int), 1, save_file);
        fread(&zc->ai_timer, sizeof(int), 1, save_file);
        fread(&zc->attack_timer, sizeof(int), 1, save_file);
        fread(&zc->shoot_timer, sizeof(int), 1, save_file);
        fread(&zc->pattern_timer, sizeof(int), 1, save_file);
        fread(&zc->shoots_fired_in_pattern, sizeof(int), 1, save_file);
        fread(&zc->pattern_direction_right, sizeof(bool), 1, save_file);
        fread(&zc->spot_timer, sizeof(bool), 1, save_file);
    }

    bool has_weaponC = true;
    fread(&has_weaponC, sizeof(bool), 1, save_file);
    if(has_weaponC)
    {
        WeaponsC* wc = buffer->weaponsC;
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

            WeaponsComponent_AddWeaponToInventory(wc, weapon);
        }
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

        int num_of_props = Vector_Count(&w->props_vector);
        fwrite(&num_of_props, sizeof(int), 1, save_file);
        for(int i = 0 ; i < num_of_props ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->props_vector, i);
            LevelEditor_WriteEntity(save_file, buffer);
        }

        int num_of_zombies = Vector_Count(&w->monsters_vector);
        fwrite(&num_of_zombies, sizeof(int), 1, save_file);
        printf("Saving %d mobs\n", num_of_zombies);
        for(int i = 0 ; i < num_of_zombies ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->monsters_vector, i);
            LevelEditor_WriteEntity(save_file, buffer);
        }

        int num_of_decals = Vector_Count(&w->decals_vector);
        fwrite(&num_of_decals, sizeof(int), 1, save_file);
        for(int i = 0 ; i < num_of_decals ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->decals_vector, i);
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

void Level_Load(char* file_name, World* w)
{
    Level_Clear(w);


    FILE *save_file;

    save_file = fopen(file_name, "rb");
    if(!save_file)
    {
        printf("Can't open file");

    }
    printf("gggggg");
    int nb_of_walls = 0;
    fread(&nb_of_walls, sizeof(int), 1, save_file);

    for(int i = 0 ; i < nb_of_walls ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity)); //Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);
        int position_in_array = buffer->x / TILE_SIZE + ((buffer->y/ TILE_SIZE) * w->map_width);

        w->map[position_in_array] = buffer;

    }


    for(int i = 0 ; i < w->map_size ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity)); //Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);
        int position_in_array = buffer->x / TILE_SIZE + ((buffer->y/ TILE_SIZE) * w->map_width);

        w->ground_map[position_in_array] = buffer;
    }

    printf("mdr");
    int num_of_events = 0;
    fread(&num_of_events, sizeof(int), 1, save_file);
    for(int i = 0 ; i < num_of_events ; i++)
    {
        Entity* buffer  = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer, w);

        Vector_Push(&w->events_vector, buffer);

    }
printf("mdr");
    int num_of_props = 0;
    fread(&num_of_props, sizeof(int), 1, save_file);
    for(int i = 0 ; i < num_of_props; i++)
    {
        Entity* buffer  = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer, w);

        Vector_Push(&w->props_vector, buffer);

    }

    int num_of_zombies = 0;
    fread(&num_of_zombies, sizeof(int), 1, save_file);

    printf("Loading %d mobs\n", num_of_zombies);
    if(num_of_zombies != 0)
    {
        for(int i = 0 ; i < num_of_zombies ; i++)
        {
            Entity* buffer  = Entity_Spawn();
            LevelEditor_ReadEntity(save_file, buffer, w);
            Vector_Push(&w->monsters_vector, buffer);

        }
    }

    int num_of_decalss = 0;
    fread(&num_of_decalss, sizeof(int), 1, save_file);
        printf("Loading %d decals\n", num_of_decalss);
    for(int i = 0 ; i < num_of_decalss ; i++)
    {
        Entity* buffer  = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer, w);
        Vector_Push(&w->decals_vector, buffer);

    }


    int num_of_bonus = 0;
    fread(&num_of_bonus, sizeof(int), 1, save_file);
    for(int i = 0 ; i < num_of_bonus ; i++)
    {
        Entity* buffer = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer, w);
        Vector_Push(&w->bonus_vector, buffer);

    }



    fclose(save_file);
}



/*pressing F7 to quickly try a level inside the level editor*/
void LevelEditor_QuickTry(World* world)
{
    game_state_g = GameState_Map_Editor_Testing_Level;
    Game_StartMap(world);
    world->player.movementC->speed = BASE_PLAYER_SPEED;
    Level_Save("saves/tempLevelEditor.sav", world);
}

/*quitting the "quick try" mode : loading the temp save and returning to level editor*/
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
    Vector_Clear(&w->props_vector);

    for(int i = 0 ; i < w->map_size ; i++)
    {
        free(w->map[i]);

    }

    for(int i = 0 ; i < w->map_size ; i++)
    {
        free(w->ground_map[i]);
    }

}

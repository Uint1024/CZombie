#include "levelEditor.h"
#include "stdio.h"
#include "world.h"
#include "vector.h"
#include "zombie.h"
#include "weapon.h"
#include "weapons_component.h"
#include "movement_component.h"

void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer)
{

    fwrite(&buffer->texture, sizeof(Texture_Type), 1, save_file);
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
        fwrite(true, sizeof(bool), 1, save_file);
        MovementC* mc = buffer->movementC;
        fwrite(&mc->angle, sizeof(float), 1, save_file);
        fwrite(&mc->speed, sizeof(float), 1, save_file);
        fwrite(&mc->dx, sizeof(float), 1, save_file);
        fwrite(&mc->dy, sizeof(float), 1, save_file);
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
}

void LevelEditor_ReadEntity(FILE* save_file, Entity* buffer)
{
    fread(&buffer->texture, sizeof(Texture_Type), 1, save_file);
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

        if(zombie_weapon_g[zc->zombie_type] != No_Weapon)
        {
            buffer->weaponsC = WeaponsComponent_Create(true);
            WeaponsComponent_AddWeaponToInventory(buffer->weaponsC,
                                              Weapon_Create(zombie_weapon_g[buffer->zombieC->zombie_type]));
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
        printf("savin %d events", num_of_events);
        for(int i = 0 ; i < num_of_events ; i++)
        {
            Entity* buffer = (Entity*)Vector_Get(&w->events_vector, i);
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

        fclose(save_file);
}


void Level_Load(char* file_name, World* w)
{
    Vector_Clear(&w->monsters_vector);
    Vector_Clear(&w->bonus_vector);
    Vector_Clear(&w->events_vector);
    Vector_Clear(&w->bullets_vector);
    Vector_Clear(&w->explosions_vector);

    printf("LOADING\n");
    FILE *save_file;

    save_file = fopen(file_name, "rb");
    if(!save_file)
    {
        printf("Can't open file");

    }

    for(int i = 0 ; i < w->map_size ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity)); //Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);
        w->map[i] = buffer;
    }


    for(int i = 0 ; i < w->map_size ; i++)
    {
        Entity* buffer = (Entity*)malloc(sizeof(Entity));//Entity_Spawn();
        fread(buffer, sizeof(Entity), 1, save_file);

        w->ground_map[i] = buffer;
    }

    int num_of_events = 0;
    fread(&num_of_events, sizeof(int), 1, save_file);
    printf("loading %d events\n", num_of_events);
    for(int i = 0 ; i < num_of_events ; i++)
    {
        Entity* buffer = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer);

        Vector_Push(&w->events_vector, buffer);

    }

    int num_of_zombies = 0;
    fread(&num_of_zombies, sizeof(int), 1, save_file);
    printf("Loading %d mobs\n", num_of_zombies);
    for(int i = 0 ; i < num_of_zombies ; i++)
    {
        Entity* buffer = Entity_Spawn();
        LevelEditor_ReadEntity(save_file, buffer);
        Vector_Push(&w->monsters_vector, buffer);

    }
    fclose(save_file);
}

#include "levelEditor.h"
#include "stdio.h"
#include "world.h"
#include "vector.h"
#include "zombie.h"
#include "weapon.h"
#include "weapons_component.h"

void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer)
{

    fwrite(&buffer->texture, sizeof(Texture_Type), 1, save_file);
    fwrite(&buffer->sub_category, sizeof(int), 1, save_file);
    fwrite(&buffer->damage, sizeof(float), 1, save_file);
    fwrite(&buffer->x, sizeof(float), 1, save_file);
    fwrite(&buffer->y, sizeof(float), 1, save_file);
    fwrite(&buffer->t, sizeof(Main_Category), 1, save_file);
    fwrite(&buffer->dx, sizeof(float), 1, save_file);
    fwrite(&buffer->dy, sizeof(float), 1, save_file);
    fwrite(&buffer->muzzleX, sizeof(float), 1, save_file);
    fwrite(&buffer->muzzleY, sizeof(float), 1, save_file);
    fwrite(&buffer->speed, sizeof(float), 1, save_file);
    fwrite(&buffer->bullet_type, sizeof(Weapon_Type), 1, save_file);
    fwrite(&buffer->explosion_timer, sizeof(int), 1, save_file);
    fwrite(&buffer->box, sizeof(Box), 1, save_file);
    fwrite(&buffer->alive, sizeof(Jbool), 1, save_file);
    fwrite(&buffer->time_traveled, sizeof(int), 1, save_file);
    fwrite(&buffer->is_ennemy_bullet, sizeof(Jbool), 1, save_file);
    fwrite(&buffer->hp, sizeof(int), 1, save_file);
    fwrite(&buffer->last_creation, sizeof(int), 1, save_file);
    fwrite(&buffer->invulnerability_timer, sizeof(int), 1, save_file);
    fwrite(&buffer->blinking_timer, sizeof(int), 1, save_file);
    fwrite(&buffer->blinking_frame, sizeof(int), 1, save_file);
    fwrite(&buffer->stamina, sizeof(float), 1, save_file);
    fwrite(&buffer->max_stamina, sizeof(float), 1, save_file);
    fwrite(&buffer->running, sizeof(Jbool), 1, save_file);
    fwrite(&buffer->angle, sizeof(float), 1, save_file);
    fwrite(&buffer->solid, sizeof(Jbool), 1, save_file);
    fwrite(&buffer->collision_direction, sizeof(Direction), 1, save_file);
    fwrite(&buffer->door_opening_timer, sizeof(int), 1, save_file);

    if(buffer->zombieC != NULL)
    {
        Jbool is_zombie = Jtrue;
        fwrite(&is_zombie, sizeof(Jbool), 1, save_file);

        ZombieC* zc = buffer->zombieC;
        fwrite(&zc->aggressive, sizeof(Jbool), 1, save_file);
        fwrite(&zc->idling, sizeof(Jbool), 1, save_file);
        fwrite(&zc->rand_move_every, sizeof(int), 1, save_file);
        fwrite(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fwrite(&zc->vision_distance, sizeof(int), 1, save_file);
        fwrite(&zc->vision_width, sizeof(float), 1, save_file);
        fwrite(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
    }
    else
    {
        Jbool is_zombie = Jfalse;
        fwrite(&is_zombie, sizeof(Jbool), 1, save_file);
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
    fread(&buffer->dx, sizeof(float), 1, save_file);
    fread(&buffer->dy, sizeof(float), 1, save_file);
    fread(&buffer->muzzleX, sizeof(float), 1, save_file);

    fread(&buffer->muzzleY, sizeof(float), 1, save_file);
    fread(&buffer->speed, sizeof(float), 1, save_file);
    fread(&buffer->bullet_type, sizeof(Weapon_Type), 1, save_file);
    fread(&buffer->explosion_timer, sizeof(int), 1, save_file);
    fread(&buffer->box, sizeof(Box), 1, save_file);
    fread(&buffer->alive, sizeof(Jbool), 1, save_file);
    fread(&buffer->time_traveled, sizeof(int), 1, save_file);
    fread(&buffer->is_ennemy_bullet, sizeof(Jbool), 1, save_file);
    fread(&buffer->hp, sizeof(int), 1, save_file);
    fread(&buffer->last_creation, sizeof(int), 1, save_file);
    fread(&buffer->invulnerability_timer, sizeof(int), 1, save_file);
    fread(&buffer->blinking_timer, sizeof(int), 1, save_file);

    fread(&buffer->blinking_frame, sizeof(int), 1, save_file);
    fread(&buffer->stamina, sizeof(float), 1, save_file);
    fread(&buffer->max_stamina, sizeof(float), 1, save_file);
    fread(&buffer->running, sizeof(Jbool), 1, save_file);
    fread(&buffer->angle, sizeof(float), 1, save_file);
    fread(&buffer->solid, sizeof(Jbool), 1, save_file);

    fread(&buffer->collision_direction, sizeof(Direction), 1, save_file);


    fread(&buffer->door_opening_timer, sizeof(int), 1, save_file);

    Jbool is_zombie = Jfalse;

    fread(&is_zombie, sizeof(Jbool), 1, save_file);

    if(is_zombie != Jfalse)
    {
        ZombieC* zc = ZombieC_Create();

        fread(&zc->aggressive, sizeof(Jbool), 1, save_file);
        fread(&zc->idling, sizeof(Jbool), 1, save_file);

        fread(&zc->rand_move_every, sizeof(int), 1, save_file);
        fread(&zc->rand_move_timer, sizeof(int), 1, save_file);
        fread(&zc->vision_distance, sizeof(int), 1, save_file);
        fread(&zc->vision_width, sizeof(float), 1, save_file);
        fread(&zc->zombie_type, sizeof(Zombie_Type), 1, save_file);
        buffer->zombieC = zc;

        if(zombie_weapon_g[zc->zombie_type] != No_Weapon)
        {
            buffer->weapons_component = WeaponsComponent_Create(Jtrue);
            WeaponsComponent_AddWeaponToInventory(buffer->weapons_component,
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

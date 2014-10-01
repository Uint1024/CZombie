#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H
#include "stdio.h"
#include "common.h"

typedef struct World World;
typedef struct Entity Entity;

void Level_Save(char* file_name, World* w);
void Level_Load(char* file_name, World* w);

void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer);
void LevelEditor_ReadEntity(FILE* save_file, Entity* buffer, World* world);

void LevelEditor_QuickTry(World* world);
void LevelEditor_BackToEditing(World* world);
void LevelEditor_LoadMapToEdit(char* complete_name, World* world);
void Level_Clear(World* world);
void LevelEditor_CreateObject(Main_Category category, int obj_type,
                              int x, int y, int position_in_array,float mousePositionInWorldX,
                              float mousePositionInWorldY,
                              World* world, bool unlimited, float angle);
void LevelEditor_UpdateWallsCorners(int map_position, World* world);
#endif // LEVELEDITOR_H

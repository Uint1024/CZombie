#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H
#include "stdio.h"

typedef struct World World;
typedef struct Entity Entity;

void Level_Save(char* file_name, World* w);
void Level_Load(char* file_name, World* w);

void LevelEditor_WriteEntity(FILE* save_file, Entity* buffer);
void LevelEditor_ReadEntity(FILE* save_file, Entity* buffer);
#endif // LEVELEDITOR_H

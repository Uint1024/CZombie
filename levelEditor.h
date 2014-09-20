#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

typedef struct World World;

void Level_Save(char* file_name, World* w);
void Level_Load(char* file_name, World* w);


#endif // LEVELEDITOR_H

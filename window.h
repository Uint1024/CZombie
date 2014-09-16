#ifndef WINDOW_H
#define WINDOW_H

#include "BoundingBox.h"

typedef struct Button
{
    int                     x;
    int                     y;
    Box                     box;
    LevelEditor_Button      button_type;
    Texture_Type            texture;
} Button;


typedef struct Window
{
    int         x;
    int         y;
    Button      buttons[10];
    Box         box;
} Window;

Window Window_CreateLevelEditor();
Button Button_Create(LevelEditor_Button type, int x, int y, Window* parent_window);
#endif // WINDOW_H

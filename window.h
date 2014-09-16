#ifndef WINDOW_H
#define WINDOW_H

#include "BoundingBox.h"

typedef struct Button
{
    Box                    box;
    LevelEditor_Button     button_type;
    Texture_Type           texture;
} Button;


typedef struct Window
{
    int         x;
    int         y;
    Button      buttons[10];
    Box         box;
} Window;

Window Window_Create();
#endif // WINDOW_H

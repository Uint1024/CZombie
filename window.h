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
    int         buttons_size;
    int         margin;
    int         space_between_buttons;
    int         buttons_per_row;
} Window;

Window Window_CreateLevelEditor();
Button Button_Create(LevelEditor_Button type, int x, int y, Window* parent_window);
void Window_ResizeRight(Window *w, int changeW);
void Window_ResizeLeft(Window *w, int changeW);
void Window_UpdateButtonsPositions(Window* w);
#endif // WINDOW_H

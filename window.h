#ifndef WINDOW_H
#define WINDOW_H

#include "BoundingBox.h"

typedef struct Button
{
    int                     x;
    int                     y;

    Box                     box;
    int      button_type;
//    Texture_Type            texture;
    Main_Category                    main_category;
} Button;


typedef struct Window
{
    int         x;
    int         y;
    Button      buttons[150];
    Box         box;
    int         buttons_size;
    int         margin;
    int         space_between_buttons;
    int         buttons_per_row;
    int         nb_of_buttons;
    Button*     active_button;
} Window;

Window Window_CreateLevelEditor();
void Window_ResizeRight(Window *w, int changeW);
void Window_ResizeLeft(Window *w, int changeW);
void Window_UpdateButtonsPositions(Window* w);
void Window_Move(Window* w, int dx, int dy);
void Button_Create(Main_Category cat, int type, int icon_nb, Window* w);
#endif // WINDOW_H

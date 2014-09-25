#include "window.h"
#include "boundingBox.h"
#include "stdio.h"


Window Window_CreateLevelEditor()
{
    Window w;

    w.x = 20;
    w.y = 20;
    w.buttons_size = 20;
    int w_width = 200;
    int w_height = 200;
    w.space_between_buttons = 5;
    w.margin = 10;



    BoundingBox_CreateWindow(&w, w_width, w_height);


    Window_UpdateButtonsPositions(&w);

    return w;
}

void Window_Move(Window* w, int dx, int dy)
{
    w->x += dx;
    w->y += dy;

    for(int i = 0 ; i < w->nb_of_buttons ; i++)
    {
        w->buttons[i].x += dx;
        w->buttons[i].y += dy;
        BoundingBox_UpdateWindow(&w->buttons[i].box, w->buttons[i].x, w->buttons[i].y);
    }

    BoundingBox_UpdateWindow(&w->box, w->x, w->y);
}

void Window_ResizeRight(Window *w, int changeW)
{
    int newW = w->box.width + changeW;
    if(newW > 50)
    {
        BoundingBox_UpdateNewSize(&w->box, newW, w->box.height);

        Window_UpdateButtonsPositions(w);
    }
}

void Window_ResizeLeft(Window *w, int changeW)
{
    //changeW is negative because the mouse moved to left
    int newW = w->box.width - changeW;

    if(newW > 50)
    {
        //move window to left and then increase its width
        Window_Move(w, changeW, 0);
        BoundingBox_UpdateNewSize(&w->box, newW, w->box.height);

        Window_UpdateButtonsPositions(w);
    }
}


void Window_UpdateButtonsPositions(Window* w)
{
   w->buttons_per_row = ((float)(w->box.width - w->margin  * 2) /
                        ((float)(w->buttons_size + w->space_between_buttons)));


    int buttons_nb = 0;
    for(int i = 0 ; i < NB_OF_WALL_TYPES ; i++)
    {
        Button_Create(Cat_Wall, i, buttons_nb, w);
        buttons_nb++;
    }


    for(int i = 0 ; i < NB_OF_GROUND_TYPES ; i++)
    {
        Button_Create(Cat_Ground, i, buttons_nb, w);
        buttons_nb++;
    }

    for(int i = 0 ; i < NB_OF_DOOR_TYPES ; i++)
    {
        Button_Create(Cat_Door, i, buttons_nb, w);
        buttons_nb++;
    }

    for(int i = 0 ; i < NB_ZOMBIE_TYPES ; i++)
    {
        Button_Create(Cat_Zombie, i, buttons_nb, w);
        buttons_nb++;
    }

    for(int i = 0 ; i < NB_OF_BONUS_TYPES ; i++)
    {
        Button_Create(Cat_Bonus, i, buttons_nb, w);
        buttons_nb++;
    }

    for(int i = 0 ; i < NB_EVENT_TYPES ; i++)
    {
        Button_Create(Cat_Event, i, buttons_nb, w);
        buttons_nb++;

    }

    w->nb_of_buttons = buttons_nb;

}

void Button_Create(Main_Category cat, int type, int buttons_nb, Window* w)
{
    int row = buttons_nb / w->buttons_per_row;
    int column = buttons_nb - (row * w->buttons_per_row);
    int y = row * (w->buttons_size + w->space_between_buttons) + w->margin;
    int x = column * 25 + 10;

    Button button;
    button.main_category = cat;
    button.button_type = type;
//    button.texture = 0;
  //  button.texture = all_textures_g[cat][type];

    button.x = x + w->x;
    button.y = y + w->y;
    button.box = BoundingBox_CreateBetter(button.x, button.y , 20, 20);
    w->buttons[buttons_nb] = button;

}

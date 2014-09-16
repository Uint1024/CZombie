#include "window.h"
#include "boundingBox.h"

Window Window_CreateLevelEditor()
{
    Window w;

    w.x = 20;
    w.y = 20;
    w.buttons_size = 20;
    int w_width = 100;
    int w_height = 100;
    w.space_between_buttons = 5;
    w.margin = 10;



    BoundingBox_CreateWindow(&w, w_width, w_height);


    Window_UpdateButtonsPositions(&w);

    /*w.buttons_per_row = ((float)(w.box.width - w.buttons_size) /
                        ((float)(w.buttons_size + w.space_between_buttons)));

    for(int i = 0 ; i < NB_OF_LEVEL_EDITOR_BUTTONS ; i++)
    {
        int row = i / w.buttons_per_row;

        int y = row * (w.buttons_size + w.space_between_buttons) + w.margin;
        int x = ((w.buttons_size + w.space_between_buttons) * i) -
                    (w.box.width * row) + w.margin +
                    (w.buttons_per_row * w.space_between_buttons * row);

        w.buttons[i] = Button_Create(i, x, y, &w);
    }*/

    return w;
}

void Window_Move(Window* w, int dx, int dy)
{
    w->x += dx;
    w->y += dy;

    for(int i = 0 ; i < NB_OF_LEVEL_EDITOR_BUTTONS ; i++)
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

    BoundingBox_UpdateNewSize(&w->box, newW, w->box.height);

    Window_UpdateButtonsPositions(w);
}

void Window_ResizeLeft(Window *w, int changeW)
{
    //changeW is negative because the mouse moved to left

    int newW = w->box.width - changeW;

    //move window to left and then increase its width
    Window_Move(w, changeW, 0);
    BoundingBox_UpdateNewSize(&w->box, newW, w->box.height);

    Window_UpdateButtonsPositions(w);
}


void Window_UpdateButtonsPositions(Window* w)
{
   w->buttons_per_row = ((float)(w->box.width - w->margin  * 2) /
                        ((float)(w->buttons_size + w->space_between_buttons)));

    for(int i = 0 ; i < NB_OF_LEVEL_EDITOR_BUTTONS ; i++)
    {
        int row = i / w->buttons_per_row;
        int column = i - (row * w->buttons_per_row);
        int y = row * (w->buttons_size + w->space_between_buttons) + w->margin;
        int x = column * 25 + 10;
        w->buttons[i] = Button_Create(i, x, y, w);
    }
}


Button Button_Create(LevelEditor_Button type, int x, int y, Window* parent_window)
{
    Button button;

    button.button_type = type;
    button.x = x + parent_window->x;
    button.y = y + parent_window->y;
    button.box = BoundingBox_CreateBetter(x, y, 20, 20);

    switch(type)
    {
    case GrassGround_button:
        button.texture = GrassGround_tex;
        break;
    case DirtGround_button:
        button.texture = DirtGround_tex;
        break;
    case NormalZombie_button:
        button.texture = Zombie_tex;
        break;
    case FastZombie_button:
        button.texture = FastZombie_tex;
        break;
    case HeavyZombie_button:
        button.texture = HeavyZombie_tex;
        break;
    }

    return button;
}

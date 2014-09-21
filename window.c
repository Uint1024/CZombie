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

    for(int i = 0 ; i < NB_EVENT_TYPES ; i++)
    {
        printf("event button n %d\n", i);
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
    button.texture = 0;
    switch(cat)
    {
    case Cat_Wall:
        button.texture = wall_textures_g[type];
        break;
    case Cat_Zombie:
        button.texture = zombie_textures_g[type];
        break;
    case Cat_Ground:
        button.texture = ground_textures_g[type];
        break;
    case Cat_Door:
        button.texture = door_textures_g[type];
        break;
    case Cat_Grenade:
        //todo
        break;
    case Cat_Event:
        button.texture = event_textures_g[type];
        break;
    case NB_OF_CAT:
        printf("error, tried to create NB_OF_CAT button\n");
        break;
    case Nothing:
        printf("error, tried to create Nothing button\n");
        break;
    case Cat_Player:
        printf("error, tried to create Player button\n");
        break;
    case Cat_Weapon:
        printf("error, tried to create Weapon button\n");
        break;
    case Cat_Bonus:
        printf("error, tried to create Bonus button\n");
        break;
    case Cat_Texture:
        printf("error, tried to create Texture button\n");
        break;
    case Cat_Bullet:
        printf("error, tried to create Bullet button\n");
        break;
    }

    button.x = x + w->x;
    button.y = y + w->y;
    button.box = BoundingBox_CreateBetter(button.x, button.y , 20, 20);
    w->buttons[buttons_nb] = button;

    printf("button nb %d\n", buttons_nb);


}

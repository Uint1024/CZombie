#include "window.h"

Window Window_CreateLevelEditor()
{
    Window w;

    w.x = 20;
    w.x = 20;
    BoundingBox_CreateWindow(&w, 100, 100);

    w.buttons[0] = Button_Create(DirtGround_button, 10, 10, &w);
    return w;
}

void Window_Move(Window* w, int dx, int dy)
{
    w->x += dx;
    w->y += dy;
    //w->buttons[0].x += dx;
   // w->buttons[0].y += dy;
    BoundingBox_UpdateWindow(&w->box, w->x, w->y);
    //BoundingBox_UpdateWindow(&w->buttons[0].box, w->buttons[0].x, w->buttons[0].y);
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
    }

    return button;
}

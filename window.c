#include "window.h"

Window Window_Create()
{
    Window w;

    w.x = 20;
    w.x = 20;
    BoundingBox_CreateWindow(&w, 100, 100);
    return w;
}

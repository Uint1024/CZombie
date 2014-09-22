#include "movement_component.h"

MovementC* MovementC_Create()
{
    MovementC* mc = (MovementC*)malloc(sizeof(MovementC));
    mc->dx      = 0;
    mc->dy      = 0;
    mc->speed   = 0;
    mc->angle   = 0;
    return mc;
}

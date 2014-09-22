
#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

typedef struct MovementC
{
	float                   dx;
	float                   dy;
	float                   speed;
	float                   angle;
}MovementC;

MovementC* MovementC_Create();


#endif // MOVEMENT_COMPONENT_H

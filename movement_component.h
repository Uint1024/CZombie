
#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

typedef struct MovementC
{
	float                   dx;
	float                   dy;
	float                   speed;
	//float                   angle;
	float                   normal_speed;
	float                   running_speed;
}MovementC;

MovementC* MovementC_Create();


#endif // MOVEMENT_COMPONENT_H

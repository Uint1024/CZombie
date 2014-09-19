#include "common.h"
#include "entity.h"
#include "math.h"

Main_Category  button_category_g[NB_OF_LEVEL_EDITOR_BUTTONS];
//Main_Category button_category_g = (Main_Category)malloc(sizeof(Main_Category) * NB_OF_CAT);

/*
            Normal, Heavy,  Fast,   Huge,   Trooper,
            NB_ZOMBIE_TYPES, Not_a_zombie
*/
float   calm_speed_g[NB_ZOMBIE_TYPES] =
        {   0.1,   0.05,    0.05,   0.05,   0.05,
            0,      0
        };

float   angry_speed_g[NB_ZOMBIE_TYPES] =
        {   0.15,   0.15,   0.45,   0.1,    0.1,
            0,      0
        };

float C_AngleBetween2Points(int xa, int ya, int xb, int yb)
{
	float angle     =    atan2f(yb - ya, xb - xa);

	return angle;
}

float C_DifferenceBetweenAngles(float angle1, float angle2)
{
    float difference = C_ConvertAngle2PiCirlce(angle1) -
                        C_ConvertAngle2PiCirlce(angle2);


    return difference;
}

float C_ConvertAngle2PiCirlce(float angle)
{
    if(angle < 0)
    {
        angle = (angle + 2 * PI);
    }
    return angle;
}
float C_DistanceBetween2Points(int xa, int ya, int xb, int yb)
{
    float distanceX =  xa - xb;
    float distanceY = ya - yb;
    float pythDistance = sqrt(distanceX * distanceX +
                                distanceY * distanceY);

    return pythDistance;
}

//generate angle between -pi and +pi
float C_GenerateRandomAngle()
{
    float new_angle =  (float)((rand() % 628) / 100.0f);

    if(new_angle > PI)
    {
        new_angle = new_angle - 2*PI;
    }

    return new_angle;
}
float C_GenerateAngleInOppositeDirection(Direction direction)
{
    float angle = 0;
    if(direction == Left)
    {
        //we move either up or down to the right
        int up_or_down = rand() % 2;

        //generate number between 0 and 157 ((pi/2) * 100), and divide it by 100
        //to obtain an angle between 0 and 157
        angle = (float)((rand() % 157 ) / 100.0f);

        //multiply either by -1 or 1 to obtain an angle in range [-1.57 ; 1.57]
        angle *= up_or_down ? -1 : 1;
    }

    //same as Left, but we generate an angle in range [1.57 ; 3.14] or [-3.14 ; -1.57]
    else if(direction == Right)
    {
        int up_or_down = rand() % 2;

        //generate number between 0 and 157, add 157 to it, so we have a number
        //between 157 and 314
        //then divide it by 100 to get an angle in the range 1.57 3.14
        angle = (float)(((rand() % 157) + 157.0f) / 100.0f);

        //multiply by either -1 or 1
        angle *= up_or_down ? -1 : 1;
    }

    //angle between 0 and PI
    else if(direction == Top)
    {
        angle = (float)((rand() % 314) / 100.0f);
    }

    //angle between -PI and 0
    else if(direction == Bottom)
    {
        angle = (float)((rand() % 314) / 100.0f) * -1;
    }

    return angle;
}

//angle between the middle of 2 entities
float C_AngleBetween2Entities(Entity* a, Entity* b)
{
    int xa = a->x       +   a->box.width/2;
    int ya = a->y       +   a->box.height/2;
    int xb = b->x       +   b->box.width/2;
    int yb = b->y       +   b->box.height/2;



	return C_AngleBetween2Points(xa, ya, xb, yb);
}

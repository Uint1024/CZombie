#include "common.h"
#include "entity.h"
#include "math.h"


    SDL_Color font_color_g[NBCOLORS];

//Texture_Type     all_textures_g[NB_OF_CAT][100];

    /*Bonus_Rifle,
    Bonus_Shotgun,
    Bonus_GrenadeLauncher,
    Bonus_Handgun,*/

    int     bonus_width_g[NB_OF_BONUS_TYPES] =
    {
        30, 30, 30, 20
    };

    int     bonus_height_g[NB_OF_BONUS_TYPES] =
    {
        15, 15, 15, 15
    };

void Game_InitData()
{
    SDL_Color black = {0,0,0,255};
    SDL_Color white = {255,255,255,255};

    font_color_g[Black] = black;
    font_color_g[White] = white;

}

void C_Sort3Vectors(Vec2* v1, Vec2* v2, Vec2* v3)
{
    //sort vectors so v1 has the lowest y
    if(v3->y < v2->y)
    {
        Vec2 temp = *v2;
        *v2 = *v3;
        *v3 = temp;
    }

    if(v2->y < v1->y)
    {
        Vec2 temp = *v1;
        *v1 = *v2;
        *v2 = temp;
    }
    if(v3->y < v2->y)
    {
        Vec2 temp = *v2;
        *v2 = *v3;
        *v3 = temp;
    }

}


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

float C_GetSlopeBetween2Vec(Vec2 v1, Vec2 v2, bool reverse_slope)
{
    if(reverse_slope)
    {
        return (float)(v1.y - v2.y) / (float)(v1.x - v2.x);
    }
    else
    {

        return (float)(v1.x - v2.x) / (float)(v1.y - v2.y);
    }
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

float C_DistanceSquaredBetween2Points(int xa, int ya, int xb, int yb)
{
    float distanceX =  xa - xb;
    float distanceY = ya - yb;
    float distance = distanceX * distanceX +
                                distanceY * distanceY;

    return distance;
}

float C_AngleAndDistanceBetween2EntitiesSquared(Entity* a, Entity* b, float *angle, float *distance)
{
    float aMiddleX, aMiddleY, bMiddleX, bMiddleY;

    Entity_GetMiddleCoordinates(a, &aMiddleX, &aMiddleY);
    Entity_GetMiddleCoordinates(b, &bMiddleX, &bMiddleY);

    *angle = C_AngleBetween2Points(aMiddleX, aMiddleY, bMiddleX, bMiddleY);
    *distance = C_DistanceBetween2Points(aMiddleX, aMiddleY, bMiddleX, bMiddleY);
}

float C_GetSlopeBetween2Points(float xa, float ya, float xb, float yb, bool reverse_slope)
{
    if(reverse_slope)
    {
        return (float)(ya - yb) / (float)(xa - xb);
    }
    else
    {

        return (float)(xa - xb) / (float)(ya - yb);
    }
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

    //if collision left
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


#include "common.h"
#include "entity.h"
#include "math.h"


    SDL_Color font_color_g[NBCOLORS];

Texture_Type     all_textures_g[NB_OF_CAT][100];

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
    /*Texture of objects*/
    all_textures_g[Cat_Bonus][Bonus_GrenadeLauncher] = Tex_Bonus_GrenadeLauncher;
    all_textures_g[Cat_Bonus][Bonus_Rifle] = Tex_Bonus_Rifle;
    all_textures_g[Cat_Bonus][Bonus_Shotgun] = Tex_Bonus_Shotgun;
    all_textures_g[Cat_Bonus][Bonus_Handgun] = Tex_Bonus_Handgun;
    all_textures_g[Cat_Bonus][Bonus_TheBigGun] = Tex_Bonus_TheBigGun;

    all_textures_g[Cat_Wall][Wall_Normal] = Tex_Wall_Normal;
    all_textures_g[Cat_Wall][Wall_Cool] = Tex_Wall_Cool;
    all_textures_g[Cat_Wall][Wall_Glass] = Tex_Wall_Glass;

    all_textures_g[Cat_Ground][Ground_Dirt] = Tex_Ground_Dirt;
    all_textures_g[Cat_Ground][Ground_Grass] = Tex_Ground_Grass;

    all_textures_g[Cat_Event][Event_End_Level] = Tex_Event_MapEnd;
    all_textures_g[Cat_Event][Event_Player_Start] = Tex_Event_PlayerSpawn;
    all_textures_g[Cat_Event][Event_Teleport_To_Other_Map] = Tex_Event_TeleportOtherMap;

    all_textures_g[Cat_Door][Door_Normal] = Tex_Door_Normal;
    all_textures_g[Cat_Door][Door_Dead] = Tex_Door_Dead;


    all_textures_g[Cat_Zombie][Normal_Zombie] = Tex_NormalZombie;
    all_textures_g[Cat_Zombie][Heavy_Zombie] = Tex_HeavyZombie;
    all_textures_g[Cat_Zombie][Fast_Zombie] = Tex_FastZombie;
    all_textures_g[Cat_Zombie][Huge_Zombie] = Tex_HugeZombie;
    all_textures_g[Cat_Zombie][Trooper_Zombie] = Tex_TrooperZombie;

    SDL_Color black = {0,0,0,255};
    SDL_Color white = {255,255,255,255};

    font_color_g[Black] = black;
    font_color_g[White] = white;

}

void C_Sort3Vectors(Vec2* v1, Vec2* v2, Vec2* v3)
{
    //sort vectors so v1 has the lowest y
       // printf("1 = %f %f, %f %f, %f %f\n", v1->x, v1->y, v2->x, v2->y, v3->x, v3->y);
    if(v3->y < v2->y)
    {
        Vec2 temp = *v2;
        *v2 = *v3;
        *v3 = temp;
    }
        //printf("2 = %f %f, %f %f, %f %f\n", v1->x, v1->y, v2->x, v2->y, v3->x, v3->y);
    if(v2->y < v1->y)
    {
        Vec2 temp = *v1;
        *v1 = *v2;
        *v2 = temp;
    }
       // printf("3 = %f %f, %f %f, %f %f\n", v1->x, v1->y, v2->x, v2->y, v3->x, v3->y);
    if(v3->y < v2->y)
    {
        Vec2 temp = *v2;
        *v2 = *v3;
        *v3 = temp;
    }
    //printf("4 = %f %f, %f %f, %f %f\n", v1->x, v1->y, v2->x, v2->y, v3->x, v3->y);

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

#include "common.h"
#include "entity.h"
#include "math.h"

Main_Category  button_category_g[NB_OF_LEVEL_EDITOR_BUTTONS];
    int button_object_type_g[NB_OF_LEVEL_EDITOR_BUTTONS];
Texture_Type ground_textures_g[NB_OF_GROUND_TYPES];
Texture_Type wall_textures_g[NB_OF_WALL_TYPES];
Texture_Type door_textures_g[NB_OF_DOOR_TYPES];
    SDL_Color font_color_g[NBCOLORS];





        /*ZOMBIES DEFINITION*/
    //TODO : put in a moddable external file


    /*
                Normal, Heavy,  Fast,   Huge,
                Trooper,
                NB_ZOMBIE_TYPES, Not_a_zombie
    */



    float   calm_speed_g[NB_ZOMBIE_TYPES] =
            {
                0.05,   0.05,    0.05,   0.05,
                0.05,
                0,      0
            };

    float   angry_speed_g[NB_ZOMBIE_TYPES] =
            {
                0.1,   0.1,   0.25,   0.1,
                0.1,
                0,      0
            };

    Texture_Type  zombie_textures_g[NB_ZOMBIE_TYPES] =
            {
                Tex_NormalZombie, Tex_HeavyZombie, Tex_FastZombie, Tex_HugeZombie,
                Tex_TrooperZombie,

            };

    int zombie_width_g[NB_ZOMBIE_TYPES] =
            {
                20,  40, 20, 45,
                100,
                0, 0
            };

    int zombie_height_g[NB_ZOMBIE_TYPES] =
            {
                20, 40, 20, 60,
                100,
                0, 0
            };

    int zombie_hp_g[NB_ZOMBIE_TYPES] =
            {
                2, 20, 2, 20,
                40,
                0, 0
            };

    int zombie_damage_g[NB_ZOMBIE_TYPES] =
            {
                3, 10, 1, 10,
                10,
                0, 0
            };

    int zombie_weapon_g[NB_ZOMBIE_TYPES] =
            {
                No_Weapon, Fireball_w, No_Weapon, TripleFireball_w,
                TripleFireball_w,
                0, 0
            };
Game_InitData()
{




    /*Texture of objects*/
    //Texture_Type ground_textures_g[NB_OF_GROUND_TYPES];
    ground_textures_g[Ground_Dirt] = Tex_Ground_Dirt;
    ground_textures_g[Ground_Grass] = Tex_Ground_Grass;

    //Texture_Type wall_textures_g[NB_OF_WALL_TYPES];
    wall_textures_g[Wall_Normal] = Tex_Wall_Normal;

    //Texture_Type door_textures_g[NB_OF_DOOR_TYPES];
    door_textures_g[Door_Normal] = Tex_Door_Normal;
    door_textures_g[Door_Dead] = Tex_Door_Dead;



    /*The object type corresponding to each button*/

    button_object_type_g[NormalZombie_button] = Normal_Zombie;
    button_object_type_g[HeavyZombie_button] = Heavy_Zombie;
    button_object_type_g[FastZombie_button] = Fast_Zombie;
    button_object_type_g[HugeZombie_button] = Huge_Zombie;
    button_object_type_g[TrooperZombie_button] = Trooper_Zombie;

    button_object_type_g[GrassGround_button] = Ground_Grass;
    button_object_type_g[DirtGround_button] = Ground_Dirt;

    button_object_type_g[Button_Wall_Normal] = Wall_Normal;

    button_object_type_g[Button_Door_Normal] = Door_Normal;



    SDL_Color black = {0,0,0,255};
    SDL_Color white = {255,255,255,255};

    font_color_g[Black] = black;
    font_color_g[White] = white;

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

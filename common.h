#ifndef COMMON_H
#define COMMON_H

#define TOTAL_MAIN_MENU_BUTTONS 3
#define TILE_SIZE 32
#include <SDL.h>


typedef enum {
    Play_button,
    SaveLevel_button,
    LoadLevel_button,
    Options_button,
    Quit_button,
    Back_button,
    FILE_NAME_BUTTON
} Menu_Button_Name;



typedef enum {
    Main_Menu_menu,
    SaveLevel_menu,
    LoadLevel_menu,
    Options_menu,
    NBMENUS
} Menu_Name;

//typedef enum { FPS_options_button, Controls_options_button, Back_button } OptionsMenu_Button;

typedef enum { Jfalse, Jtrue } Jbool;

typedef enum { Left, Top, Bottom, Right, None } Direction;

typedef enum { Survival_mode } Game_Mode;

//main category of objets
typedef enum {
    Cat_Wall,
    Cat_Door,
    Cat_Ground,
    Cat_Zombie,
    Cat_Player,
    Cat_Weapon,
    Cat_Bonus,
    Cat_Texture,
    Cat_Bullet,
    Cat_Grenade,
    NB_OF_CAT,
    Nothing
} Main_Category;



//---sub categories : wall, ground, bonus, buttons, zombies, textures, weapons
//Ground_Type
typedef enum {
    Ground_Grass,
    Ground_Dirt,
    NB_OF_GROUND_TYPES
} Ground_Type;

//Wall_Type
typedef enum {
    Wall_Normal,
    NB_OF_WALL_TYPES
} Wall_Type
;

//Door_Type
typedef enum {
    Door_Normal,
    Door_Dead,
    NB_OF_DOOR_TYPES
} Door_Type;

//Bonus_Type
typedef enum {
    Bonus_Rifle,
    Bonus_Shotgun,
    Bonus_GrenadeLauncher,
    NB_OF_BONUS_TYPES
} Bonus_type;

//LevelEditor_Button
typedef enum{
    Button_Wall_Normal,
    Button_Door_Normal,
    GrassGround_button,
    DirtGround_button,
    NormalZombie_button,
    FastZombie_button,
    HeavyZombie_button,
    TrooperZombie_button,
    NB_OF_LEVEL_EDITOR_BUTTONS,
    HugeZombie_button
} LevelEditor_Button;

//Zombie_Type
typedef enum {
    Normal_Zombie,
    Heavy_Zombie,
    Fast_Zombie,
    Huge_Zombie,
    Trooper_Zombie,
    NB_ZOMBIE_TYPES,
    Not_a_zombie
} Zombie_Type;


//Texture_Type
typedef enum {
    Tex_Player,
    Tex_Bullet,
    Tex_Fireball,

    Tex_Wall_Normal,

    Tex_Door_Normal,
    Tex_Door_Dead,

    Tex_NormalZombie,
    Tex_FastZombie,
    Tex_HeavyZombie,
    Tex_HugeZombie,
    Tex_TrooperZombie,

    Tex_Ground_Grass,
    Tex_Ground_Dirt,

    Tex_Bonus_Ammo,
    Tex_Bonus_GrenadeLauncher,
    Tex_Bonus_Rifle,
    Tex_Bonus_Shotgun,

    Explosion1_tex,

    Tex_Cursor_Aiming,
    Cursor_resize_up_down_tex,
    Cursor_resize_left_right_tex,

    NB_OF_TEXTURES,
    No_texture
} Texture_Type;


//Weapon_Type
typedef enum {
    No_Weapon,
    Handgun_w,
    AutomaticRifle_w,
    Shotgun_w,
    TripleFireball_w,
    GrenadeLauncher_w,
    Fireball_w,
    NB_WEAPON_TYPES
} Weapon_Type;


typedef enum {
    Small,
    Medium,
    Large,
    Menu_font
} Font_Size;

typedef enum{
    Black,
    Grey,
    Dark_Grey,
    White,
    Blue,
    Red,
    Green,
    NBCOLORS
} Font_Color;

extern Jbool debug_mode;
extern int delta_g;

float C_AngleBetween2Points(int xa, int ya, int xb, int yb);

typedef struct Entity Entity;
float C_AngleBetween2Entities(Entity* a, Entity* b);
float C_DistanceBetween2Points(int xa, int ya, int xb, int yb);
float C_GenerateRandomAngle();
float C_GenerateAngleInOppositeDirection(Direction direction);
float C_DifferenceBetweenAngles(float angle1, float angle2);
float C_ConvertAngle2PiCirlce(float angle);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

/*
    Main category of each button in the level editor, the categories are
    Cat_Wall, Cat_Door, Cat_Zombie etc.
*/
//extern Main_Category    button_category_g[NB_OF_LEVEL_EDITOR_BUTTONS];

/*
    The type inside of the button category
    For example if the button is a Ground Dirt button, its object type is int = 1,
    because Ground Dirt is 2nd in the Ground enum
*/
extern int              buttons_wall_types_g[NB_OF_WALL_TYPES];


/*Textures corresponding to each object in each category*/
extern Texture_Type     wall_textures_g[NB_OF_WALL_TYPES];
extern Texture_Type     door_textures_g[NB_OF_DOOR_TYPES];
extern Texture_Type     ground_textures_g[NB_OF_GROUND_TYPES];

extern SDL_Color        font_color_g[NBCOLORS];


/*zombies definition*/
extern Texture_Type     zombie_textures_g[NB_ZOMBIE_TYPES];
extern float            calm_speed_g[NB_ZOMBIE_TYPES];
extern float            angry_speed_g[NB_ZOMBIE_TYPES];
extern int              zombie_width_g[NB_ZOMBIE_TYPES];
extern int              zombie_height_g[NB_ZOMBIE_TYPES];
extern int              zombie_hp_g[NB_ZOMBIE_TYPES];
extern int              zombie_damage_g[NB_ZOMBIE_TYPES];
extern int              zombie_weapon_g[NB_ZOMBIE_TYPES];



#define NB_OF_WEAPONS NB_WEAPON_TYPES

#define BASE_PLAYER_SPEED 0.3

#define PI 3.14159265359
#define HALF_PI 1.5707963267
#endif

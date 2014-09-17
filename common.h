#ifndef COMMON_H
#define COMMON_H

#define TOTAL_MAIN_MENU_BUTTONS 3
#define TILE_SIZE 32


typedef enum { Play_button, SaveLevel_button, Options_button, Quit_button, Back_button } Menu_Button_Name;

typedef enum { Survival_mode } Game_Mode;

typedef enum {
    Main_Menu_menu,
    SaveLevel_menu,
    Options_menu,
    NBMENUS
} Menu_Name;

//typedef enum { FPS_options_button, Controls_options_button, Back_button } OptionsMenu_Button;

typedef enum { Jfalse, Jtrue } Jbool;

typedef enum { Left, Top, Bottom, Right, None } Direction;


//main category of objets
typedef enum {
    Cat_Wall,
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
} Wall_Type;

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

extern Jbool debug_mode;

float C_AngleBetween2Points(int xa, int ya, int xb, int yb);

typedef struct Entity Entity;
float C_AngleBetween2Entities(Entity* a, Entity* b);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

Main_Category   button_category_g[NB_OF_LEVEL_EDITOR_BUTTONS];
Entity*         zombie_templates_g[NB_ZOMBIE_TYPES];
int             button_object_type_g[NB_OF_LEVEL_EDITOR_BUTTONS];
Texture_Type    wall_textures_g[NB_OF_WALL_TYPES];
Texture_Type    ground_textures_g[NB_OF_GROUND_TYPES];




#define NB_OF_WEAPONS NB_WEAPON_TYPES

#define BASE_PLAYER_SPEED 0.3
#endif

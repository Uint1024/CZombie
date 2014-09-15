#ifndef COMMON_H
#define COMMON_H

#define TOTAL_MAIN_MENU_BUTTONS 3
#define TILE_SIZE 32
#define NB_OF_WEAPONS 5

typedef enum { Play_button, Options_button, Quit_button, Back_button } Menu_Button_Name;

typedef enum { Survival_mode } Game_Mode;

typedef enum { Main_Menu_menu, Options_menu } Menu_Name;

//typedef enum { FPS_options_button, Controls_options_button, Back_button } OptionsMenu_Button;

typedef enum { Jfalse, Jtrue } Jbool;

typedef enum { Left, Top, Bottom, Right, None } Direction;

//unused I think
typedef enum { Nothing, Ball, Paddle, Square, Bullet, Wall, Zombie, Handgun, AutomaticRifle, Health, Ammo, Player, Grenade  } Type;

typedef enum { Ammo_bonus, Rifle_bonus, Shotgun_bonus, GrenadeLauncher_bonus } Bonus_type;

typedef enum { Normal_Zombie, Heavy_Zombie, Fast_Zombie, Huge_Zombie, NB_ZOMBIE_TYPES } Zombie_Type;

typedef enum {  No_texture, Player_tex, Bullet_tex, Wall_tex,
                Zombie_tex, FastZombie_tex, HeavyZombie_tex, HugeZombie_tex,
                Health_tex,
                Ammo_Bonus_tex, GrenadeLauncher_Bonus_tex, Rifle_Bonus_tex,
                Shotgun_Bonus_tex,
                Explosion1_tex, Cursor_aiming_tex } Texture_Type;

typedef enum { Key_W, Key_A, Key_S, Key_D } Keyboard_Key;

typedef enum { No_Weapon, Handgun_w, AutomaticRifle_w, Shotgun_w, GrenadeLauncher_w } Weapon_Type;

typedef enum { Small, Medium, Large, Menu_font } Font_Size;

extern Jbool debug_mode;

float C_AngleBetween2Points(int xa, int ya, int xb, int yb);


typedef struct Vec2
{
    float x;
    float y;
} Vec2;
#endif

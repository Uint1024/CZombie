#ifndef COMMON_H
#define COMMON_H

#define TOTAL_MAIN_MENU_BUTTONS 3
#define TILE_SIZE 32


typedef enum { Play_button, Options_button, Quit_button, Back_button } Menu_Button_Name;

typedef enum { Survival_mode } Game_Mode;

typedef enum { Main_Menu_menu, Options_menu } Menu_Name;

//typedef enum { FPS_options_button, Controls_options_button, Back_button } OptionsMenu_Button;

typedef enum { Jfalse, Jtrue } Jbool;

typedef enum { Left, Top, Bottom, Right, None } Direction;

//unused I think
typedef enum { Nothing, Ball, Paddle, Square, Bullet, Wall, Zombie, Handgun, AutomaticRifle, Health, Ammo, Player, Grenade, Ground  } Type;

typedef enum { Grass_ground, Dirt_ground, NB_OF_GROUND_TYPES } Ground_Type;

typedef enum { Rifle_bonus, Shotgun_bonus, GrenadeLauncher_bonus, NB_OF_BONUS_TYPES } Bonus_type;

typedef enum{   GrassGround_button, DirtGround_button,
                NormalZombie_button, FastZombie_button, HeavyZombie_button, NB_OF_LEVEL_EDITOR_BUTTONS
            } LevelEditor_Button;

typedef enum {  Normal_Zombie, Heavy_Zombie, Fast_Zombie, Huge_Zombie, Trooper_Zombie,
                NB_ZOMBIE_TYPES, Not_a_zombie } Zombie_Type;

typedef enum {  Player_tex, Bullet_tex, Wall_tex, Fireball_tex,
                Zombie_tex, FastZombie_tex, HeavyZombie_tex, HugeZombie_tex,
                TrooperZombie_tex, GrassGround_tex, DirtGround_tex,
                Ammo_Bonus_tex, GrenadeLauncher_Bonus_tex, Rifle_Bonus_tex,
                Shotgun_Bonus_tex,
                Explosion1_tex, Cursor_aiming_tex, Cursor_resize_up_down_tex, Cursor_resize_left_right_tex,
                NB_OF_TEXTURES, No_texture } Texture_Type;

typedef enum { Key_W, Key_A, Key_S, Key_D } Keyboard_Key;

typedef enum { No_Weapon, Handgun_w, AutomaticRifle_w, Shotgun_w, TripleFireball_w, GrenadeLauncher_w, Fireball_w, NB_WEAPON_TYPES } Weapon_Type;


typedef enum { Small, Medium, Large, Menu_font } Font_Size;

extern Jbool debug_mode;

float C_AngleBetween2Points(int xa, int ya, int xb, int yb);

typedef struct Entity Entity;
float C_AngleBetween2Entities(Entity* a, Entity* b);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

#define NB_OF_WEAPONS NB_WEAPON_TYPES
#endif

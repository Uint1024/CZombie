#ifndef COMMON_H
#define COMMON_H

#define TOTAL_MAIN_MENU_BUTTONS 3
#define TILE_SIZE 32
#define NB_OF_WEAPONS 5

typedef enum { Play, Options, Quit} Menu_Button;
typedef enum { Jfalse, Jtrue } Jbool;

typedef enum { Left, Top, Bottom, Right, None } Direction;

typedef enum { Nothing, Ball, Paddle, Square, Bullet, Wall, Zombie, Handgun, AutomaticRifle, Health, Ammo, Player, Grenade  } Type;

typedef enum {  No_texture, Player_tex, Bullet_tex, Wall_tex,
                Zombie_tex, Handgun_tex,
                AutomaticRifle_tex, Health_tex, Ammo_Bonus_tex, Explosion1_tex } Texture_Type;

typedef enum { Key_W, Key_A, Key_S, Key_D } Keyboard_Key;

typedef enum { No_Weapon, Handgun_w, AutomaticRifle_w, Shotgun_w, GrenadeLauncher_w } Weapon_Type;

typedef enum { Small, Medium, Large } Font_Size;

extern Jbool debug_mode;
#endif

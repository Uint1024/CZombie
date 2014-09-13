#ifndef COMMON_H
#define COMMON_H

#define TILE_SIZE 32

typedef enum { Jfalse, Jtrue } Jbool;
typedef enum { Left, Top, Bottom, Right, None } Direction;
typedef enum { Ball, Paddle, Square, Bullet, Wall, Zombie,
               Handgun, AutomaticRifle, Health, Ammo  } Type;
typedef enum { Key_W, Key_A, Key_S, Key_D } Keyboard_Key;
typedef enum { Handgun_w, AutomaticRifle_w } Weapon_Type;
typedef enum { Small, Medium, Large } Font_Size;
extern Jbool debug_mode;
#endif

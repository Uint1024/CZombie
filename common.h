#ifndef COMMON_H
#define COMMON_H


#define TILE_SIZE 32
#include <SDL.h>


typedef enum {
    Play_button,
    LevelEditor_button,
    NewMap_button,
    LoadMap_button,
    SaveGame_button,
    LoadGame_button,
    SaveMap_button,
    Options_button,
    Quit_button,
    Back_button,
    Exit_Level_Editor_button,
    TOTAL_MAIN_MENU_BUTTONS,
    FILE_NAME_BUTTON
} Menu_Button_Name;

typedef enum {
    GameState_Main_Menu,
    GameState_Editing_Map,
    GameState_Map_Editor_Testing_Level,
    GameState_Playing
} Game_State;

typedef enum {
    MainMenu_menu,
    SaveMap_menu,
    LoadMap_menu,
    Options_menu,
    LevelEditor_menu,
    LevelEditorEditing_menu,
    NBMENUS
} Menu_Name;

//typedef enum { FPS_options_button, Controls_options_button, Back_button } OptionsMenu_Button;

typedef enum { false, true } bool;

typedef enum { Left, Top, Bottom, Right, None } Direction;

typedef enum { Survival_mode } Game_Mode;


typedef enum {
    Event_Player_Start,
    Event_Teleport_To_Other_Map,
    Event_End_Level,
    NB_EVENT_TYPES
} MapEvent_Type;

//main category of objets
typedef enum {
    Cat_Wall,
    Cat_Door,
    Cat_Ground,
    Cat_Zombie,
    Cat_Event,//map events
    Cat_Player,
    Cat_Weapon,
    Cat_Bonus,
    Cat_Texture,
    Cat_Bullet,
    Cat_Grenade,
    Cat_Decal,
    Cat_Explosion,
    Cat_Cursor,
    Cat_Prop,
    NB_OF_CAT,
    Nothing
} Main_Category;

typedef enum {
    Cursor_Aim,
    Cursor_Resize_Left_Right,
    Cursor_Resize_Up_Down
} Cursor_Type;

typedef enum {
    Explosion_Normal
} Explosion_Type;

typedef enum {
    Grenade_Normal
} Grenade_Type;

typedef enum {
    Player_Normal
} Player_Type;


typedef enum {
    Ground_Grass,
    Ground_Dirt,
    Ground_Red,
    Ground_WhiteDark,
    Ground_MaroonLight,
    Ground_Black,
    Ground_Yellow,
    Ground_Grey,
    Ground_Green_Blue_Carpet,
    Ground_Wood,
    Ground_Bathroom,
    NB_OF_GROUND_TYPES
} Ground_Type;

typedef enum {
    Prop_Desk,
    Prop_Bed,
    Prop_Chair,
    Prop_Car_Cool,
    Prop_Bookcase,
    Prop_BlackChest,
    Prop_FileCabinet,
    Prop_Refrigerator,
    Prop_Couch_Blue,
    Prop_Kitchen_Stove,
    Prop_Kitchen_Table,
    Prop_Plant,
    Prop_LivingRoom_TV,
    Prop_Office_Desk_Computer,
    Prop_Bathroom_Sink,
    Prop_Bathtub,
    Prop_WC,
    Prop_Cupboard,
    Prop_Dining_Room_Table_Large,
    NB_OF_PROP_TYPES
} Prop_Type;

//Wall_Type
typedef enum {
    Wall_Normal,
    Wall_Cool,
    Wall_Glass,
    Wall_Reinforced,
    Wall_Good_Corner,
    Wall_Good_Center,

    Wall_Grey_Corner,
    Wall_Grey_No_Border,
    Wall_Grey_1_Border,
    Wall_Grey_2_Borders,

    NB_OF_WALL_TYPES
} Wall_Type
;

//Door_Type
typedef enum {
    Door_Normal,
    Door_Dead,
    Door_Reinforced,
    NB_OF_DOOR_TYPES
} Door_Type;

//Bonus_Type
typedef enum {
    Bonus_Rifle,
    Bonus_Shotgun,
    Bonus_GrenadeLauncher,
    Bonus_Handgun,
    Bonus_TheBigGun,//this should always be the last weapon bonus!!!!
    Bonus_Time_Stop,
    NB_OF_BONUS_TYPES
} Bonus_type;

typedef enum {
    Bullet_Normal,
    Bullet_Fireball
} Bullet_Type;

//Zombie_Type
typedef enum {
    Zombie_Normal,
    Zombie_Heavy,
    Zombie_Fast,
    Zombie_Huge,
    Zombie_Trooper,
    Zombie_Slow,
    Zombie_Raptor,
    Zombie_Destroyer,
    NB_ZOMBIE_TYPES,
    Not_a_zombie,

    Kamikaze_Zombie,

} Zombie_Type;

typedef enum
{
    Decal_Corpse_Normal,
    Decal_Corpse_Heavy,
    Decal_Corpse_Fast,
    Decal_Corpse_Huge,
    Decal_Corpse_Trooper,
    Decal_Corpse_Slow,
    Decal_Corpse_Raptor,
    Decal_Corpse_Destroyer,
    Decal_Rug_Ancient,
    NB_OF_DECAL_TYPES
}   Decal_Type;


//Weapon_Type
typedef enum {
    No_Weapon,
    Weapon_Handgun,
    Weapon_AutomaticRifle,
    Weapon_Shotgun,
    Weapon_TripleFireball,
    Weapon_FireballMachineGun,
    Weapon_GrenadeLauncher,
    Weapon_Fireball,
    Weapon_TheBigGun,
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

extern bool debug_mode;
extern int delta_g;
extern int screen_width_g;
extern int screen_height_g;
float C_AngleBetween2Points(int xa, int ya, int xb, int yb);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;


typedef struct Entity Entity;
float C_AngleBetween2Entities(Entity* a, Entity* b);
float C_DistanceBetween2Points(int xa, int ya, int xb, int yb);
float C_GenerateRandomAngle();
float C_GenerateAngleInOppositeDirection(Direction direction);
float C_DifferenceBetweenAngles(float angle1, float angle2);
float C_ConvertAngle2PiCirlce(float angle);
float C_GetSlopeBetween2Points(float xa, float ya, float xb, float yb, bool reverse_slope);
float C_DistanceSquaredBetween2Points(int xa, int ya, int xb, int yb);
void C_Sort3Vectors(Vec2* v1, Vec2* v2, Vec2* v3);
float C_GetSlopeBetween2Vec(Vec2 v1, Vec2 v2, bool reverse_slope);
float C_AngleAndDistanceBetween2EntitiesSquared(Entity* a, Entity* b, float *angle, float *distance);
void Game_InitData();





//100 is magic number, whatever, it just has to hold enough for each category
//extern Texture_Type     all_textures_g[NB_OF_CAT][100];

extern SDL_Color        font_color_g[NBCOLORS];

extern int              bonus_width_g[NB_OF_BONUS_TYPES];
extern int              bonus_height_g[NB_OF_BONUS_TYPES];
extern Game_State       game_state_g;
extern bool            display_menu_g;
extern bool             unlimited_ammo_g;
extern bool             reloading_g;
extern bool             draw_grid_g;
extern bool using_controller_g;
extern bool bullet_time_g;
extern Uint32 bullet_time_timer;

#define NB_OF_WEAPONS NB_WEAPON_TYPES

#define BASE_PLAYER_SPEED 0.2
#define LEVEL_EDITOR_SPEED 2

#define MAP_SIZE 100
#define PI 3.141592
#define HALF_PI 1.570796

enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_START,
    BUTTON_SELECT,
    BUTTON_LEFT_AXIS,
    BUTTON_RIGHT_AXIS,
    BUTTON_LEFT_SHOULDER,
    BUTTON_RIGHT_SHOULDER,
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y
} Xbox_Buttons;

#endif

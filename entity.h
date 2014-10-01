#ifndef ENTITY_H
#define ENTITY_H
#include "common.h"
#include "camera.h"
#include "boundingBox.h"
#include "linkedList.h"
#include "weapons_component.h"
#include "vector.h"
#include "explosive.h"


typedef struct Weapon Weapon;
typedef struct ZombieC ZombieC;
typedef struct PlayerC PlayerC;
typedef struct MovementC MovementC;
typedef struct WeaponsC WeaponsC;

typedef struct Entity
{

    //variables common to every entity
	Main_Category           t; //Cat_Wall, Cat_Door, Cat_Zombie etc...
	int                     sub_category; //normal or hard wall, fast or heavy zombie...
	float                   x;
	float                   y;
//    Texture_Type            texture;
    bool                    visible;//for hidden secrets :D
    bool                    solid;//for hidden secrets :D
    bool                    transparent;//for glass
	Box                     box;
    int                     penetration_chance; //between 0 and 1000
    unsigned char           nb_penetrations;
    int                     hp;
	bool                    alive;
    int                     alive_timer;//used by explosions, bullets
    bool                    in_dark;
	bool                    is_ennemy;//zombies, bullets, explosions
    int                     damage;//zombies, bullets, explosions
    float                   angle;

    Tile_Type              tile_type;
    int width;
    int height;

    Entity*                 last_zombie_hit;//used to calculate bullets penetration
    //components
    MovementC*              movementC;//movement component
    PlayerC*                playerC;//player component
    ZombieC*                zombieC;//etc.
    Explosive*              explosiveC;
    WeaponsC*               weaponsC;

    //direction of the last collision with a wall or mob
    //used by zombies to calculate trajectories
    Direction               collision_direction;
} Entity;



void moveEntity(Entity* ent, float x, float y);

//void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();
//Entity Entity_SpawnOnStack();
void Entity_CollisionWithExplosions(Entity* ent, Vector* explosions, World* world);
void Entity_LoseHealth(Entity* ent, int damage);
bool Entity_CollisionWithStuff(Entity* ent, World* world);
void Entity_CalculateVelocityFromAngle(Entity* ent);

bool Entity_CollisionWithWalls(Entity* ent, World* world);
bool Entity_CollisionWithMonsters(Entity* ent, Vector* monsters_vector);
bool Entity_CollisionWithProps(Entity* ent, Vector* props_vector);

bool Entity_CheckNear(Entity* ent1, Entity* ent2);
bool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance);

void Entity_CalculateVelocity(Entity* ent);
void Entity_GetMiddleCoordinates(Entity* ent, float* middleX, float* middleY);
float Entity_DistanceBetweenTwoEntities(Entity* ent1, Entity* ent2);
bool Entity_CheckCanSeeEntity(Entity* ent1, Entity* ent2, World* world);

float Entity_GetMiddleY(Entity* ent);
float Entity_GetMiddleX(Entity* ent);


void Entity_Destroy(Entity* ent);
void Entity_MoveToPosition(Entity* ent, float x, float y);
void Entity_CalculateVisibility(Entity* ent, World* world);
Entity* Entity_Create(Main_Category cat, int type, float x, float y, float angle, World* world);

bool Entity_CheckVeryClose(Entity* ent1, Entity* ent2);
#endif

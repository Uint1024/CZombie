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


typedef struct Entity
{

	Main_Category           t;
	float                   x;
	float                   y;
    Texture_Type            texture;


	float                   muzzleX;
	float                   muzzleY;


	float                   dx;
	float                   dy;
	float                   speed;
	float                   angle;
    Jbool                   visible;

	Weapon_Type             bullet_type;


    int                     explosion_timer;
	//should be a pointer? Not every entity has a hitbox (maybe)
	Box                     box;

	Jbool                   alive;


	//bullet c
	int                     time_traveled;
	Jbool                   is_ennemy_bullet;
    int                     damage;

	int                     hp;



	int                     last_creation;
	Entity*                 camera;

    //player component...
	int                     invulnerability_timer;
	int                     blinking_timer;
	int                     blinking_frame;
    float                   stamina;
    float                   max_stamina;
    Jbool                   running;


    ZombieC*                zombieC;
    Explosive*              explosive_component;
    WeaponsComponent*       weapons_component;




    Jbool                   solid;
    Direction               collision_direction;
    int                     door_opening_timer;
} Entity;



void moveEntity(Entity* ent, float x, float y);

//void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();
//Entity Entity_SpawnOnStack();
void Entity_CollisionWithExplosions(Entity* ent, Vector* explosions);
void Entity_LoseHealth(Entity* ent, int damage);
Jbool Entity_CollisionWithStuff(Entity* ent, World* world);
void Entity_CalculateVelocityFromAngle(Entity* ent);

Jbool Entity_CollisionWithWalls(Entity* ent, Entity** map, int map_size);
Jbool Entity_CollisionWithMonsters(Entity* ent, Vector* monsters_vector);

Jbool Entity_CheckNear(Entity* ent1, Entity* ent2);
Jbool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance);

void Entity_CalculateVelocity(Entity* ent);
void Entity_GetMiddleCoordinates(Entity* ent, float* middleX, float* middleY);
float Entity_DistanceBetweenTwoEntities(Entity* ent1, Entity* ent2);
Jbool Entity_CheckCanSeeEntity(Entity* ent1, Entity* ent2, World* world);

float Entity_GetMiddleY(Entity* ent);
float Entity_GetMiddleX(Entity* ent);

void Entity_Destroy(Entity* ent);
#endif

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
typedef struct Entity
{
    Texture_Type            texture;
	Main_Category                    t;
	Vec2                    pos;
	float                   x;
	float                   y;
	float                   dx;
	float                   dy;
	float                   muzzleX;
	float                   muzzleY;
	float                   speed;


	Weapon_Type             bullet_type;
	Zombie_Type             zombie_type;

    int                     explosion_timer;
	//should be a pointer? Not every entity has a hitbox (maybe)
	Box                     box;
    int                     damage;
	Jbool                   alive;
	int                     time_traveled;
	Jbool                   is_ennemy_bullet;

	int                     hp;

    //used if this entity is an explosive
    Explosive*              explosive_component;

    //used if it can use weapons
    WeaponsComponent*       weapons_component;

	int                     last_creation;
	Entity*                 camera;

    //used for bonus... should be moved elsewhere...
    Weapon_Type             corresponding_weapon;
	int                     invulnerability_timer;
	int                     blinking_timer;
	int                     blinking_frame;
    float                   stamina;
    float                     max_stamina;
    Jbool                   running;

    int                   vision_distance;
    float                   vision_width;
    Vec2                    vision_points[10];
    Jbool                   aggressive;
    int                     rand_move_timer;
    int                     rand_move_every;
    Jbool                   idling;

    float                   angle;
    float                   movement_angle;

    Direction               collision_direction;
} Entity;



void moveEntity(Entity* ent, float x, float y);

//void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();
Entity Entity_SpawnOnStack();
void Entity_CollisionWithExplosions(Entity* ent, Vector* explosions);
void Entity_LoseHealth(Entity* ent, int damage);
Jbool Entity_CollisionWithStuff(Entity* ent, World* world);
void Entity_CalculateVelocityFromAngle(Entity* ent, int delta);

Jbool Entity_CollisionWithWalls(Entity* ent, Entity** map, int map_size);
Jbool Entity_CollisionWithMonsters(Entity* ent, Vector* monsters_vector);

Jbool Entity_CheckNear(Entity* ent1, Entity* ent2);
Jbool Entity_CheckDistance(Entity* ent1, Entity* ent2, int distance);

void Entity_CalculateVelocity(Entity* ent);
void Entity_GetMiddleCoordinates(Entity* ent, float* middleX, float* middleY);
float Entity_DistanceBetweenTwoEntities(Entity* ent1, Entity* ent2);
Jbool Entity_CheckCanSeeEntity(Entity* ent1, Entity* ent2, World* world);
#endif

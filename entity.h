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
    Texture_Type texture;
	Type t;
	float x;
	float y;
	float dx;
	float dy;
	float muzzleX;
	float muzzleY;
	float speed;
	float angle;

    int explosion_timer;
	//should be a pointer? Not every entity has a hitbox (maybe)
	Box box;
    int damage;
	Jbool alive;

	int hp;

    //used if this entity is an explosive
    Explosive* explosive_component;

    //used if it can use weapons
    WeaponsComponent* weapons_component;

	int last_creation;
	Entity* camera;


	int invulnerability_timer;
	int blinking_timer;
	int blinking_frame;

} Entity;



void moveEntity(Entity* ent, float x, float y);
void CollisionWithMonsters(Entity* ent, Vector* monsters_vector);
void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();
void Entity_CollisionWithExplosions(Entity* ent, Vector* explosions);
void Entity_LoseHealth(Entity* ent, int damage);
void Entity_CollisionWithStuff(Entity* ent, World* world);
#endif

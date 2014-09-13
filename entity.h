#ifndef ENTITY_H
#define ENTITY_H
#include "common.h"
#include "camera.h"
#include "boundingBox.h"
#include "linkedList.h"
#include "weapons_component.h"

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

	//should be a pointer? Not every entity has a hitbox (maybe)
	Box box;

	Jbool alive;
	int hp;



    WeaponsComponent* weapons_component;

	int last_creation;
	Entity* camera;

} Entity;



void moveEntity(Entity* ent, float x, float y);
void CollisionWithMonsters(Entity* ent, List* mob_list);
void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();

#endif

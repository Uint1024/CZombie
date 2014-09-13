#ifndef ENTITY_H
#define ENTITY_H
#include "common.h"
#include "camera.h"
#include "boundingBox.h"
#include "linkedList.h"
typedef struct Entity
{
	Type t;
	float x;
	float y;
	float dx;
	float dy;
	float muzzleX;
	float muzzleY;
	float speed;
	float angle;
	Box box;
	int player_id;
	Jbool alive;
	int hp;
	int bullets[5];
    Entity* weapon;
    int magazine_max_bullets;
    int magazine_bullets;
	int last_creation;
	Weapon_Type current_weapon;
	int last_shoot;
	int last_reload;
	Jbool reloading;
	int tempFixForCamera;
	Entity* camera;
	Texture_Type texture;
} Entity;



void moveEntity(Entity* ent, float x, float y);
void CollisionWithMonsters(Entity* ent, List* mob_list);
void CalculateVelocity(Entity* p, Entity* map, int map_size);
Entity* Entity_Spawn();

#endif

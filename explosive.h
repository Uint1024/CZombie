#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H
#include "common.h"
typedef struct Vector Vector;

typedef struct Entity Entity;

typedef struct Explosive
{
    //thrown explosives move toward their destination
    float destinationX;
    float destinationY;

    int damage;
    int explosion_radius;

    Jbool activated;
    int explosion_timer;
    int time_of_explosion;
} Explosive;

Explosive* ExplosiveComponent_Create(float x, float y, float angle, float speed,
                            float destinationX, float destinationY);
Entity* Grenade_Create(float x, float y, float angle, float speed,
                        float destinationX, float destinationY);
                        void Grenade_Move(Entity* g, Entity* map, int map_size, Vector* monsters_vector, int delta, Entity* camera);
#endif // EXPLOSIVE_H
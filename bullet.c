#include <math.h>
#include <stdlib.h>
#include "linkedList.h"
#include "bullet.h"
#include "entity.h"
#include "world.h"

Entity* Bullet_Create(Weapon_Type type, float x, float y, float angle, float speed, Jbool is_ennemy_bullet)
{
	Entity* bullet = Entity_Spawn();

	bullet->t = Cat_Bullet;
	bullet->bullet_type = type;
	bullet->x = x;
	bullet->y = y;
	bullet->angle = angle;
	bullet->speed = speed;
	bullet->is_ennemy_bullet = is_ennemy_bullet;
	bullet->time_traveled = 0;
	bullet->damage = 1;

	switch(type)
	{
    case Handgun_w:
        bullet->box.height = 10;
        bullet->box.width = 10;
        bullet->texture = Tex_Bullet;
        break;
    case AutomaticRifle_w:
        bullet->box.height = 10;
        bullet->box.width = 10;
        bullet->texture = Tex_Bullet;
        break;
    case Shotgun_w:
        bullet->box.height = 10;
        bullet->box.width = 10;
        bullet->texture = Tex_Bullet;
        break;
    case Fireball_w:
        bullet->box.height = 15;
        bullet->box.width = 15;
        bullet->texture = Tex_Fireball;
        break;
    case TripleFireball_w:
        bullet->box.height = 15;
        bullet->box.width = 15;
        bullet->texture = Tex_Fireball;
        break;
    case No_Weapon:
        printf("Error, trying to create a bullet without weapon type");
        break;
    case GrenadeLauncher_w:
        printf("Error, trying to create a grenade launcher bullet");
        break;
	}

    BoundingBox_Create(bullet, bullet->box.width, bullet->box.height);
	return bullet;
}

void Bullet_Update(Entity* bullet, int delta, World* world)
{

    bullet->time_traveled += bullet->speed * delta;
    if(bullet->time_traveled > 1000)
    {
        bullet->alive = Jfalse;
    }


    if(bullet->alive)
    {
        bullet->dx = cos(bullet->angle) * bullet->speed * delta;
        bullet->dy = sin(bullet->angle) * bullet->speed  * delta;
        moveEntity(bullet, bullet->dx, bullet->dy);

       if(!bullet->is_ennemy_bullet)
        {
            for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
            {
                Entity* mob = (struct Entity*)Vector_Get(&world->monsters_vector, i);
                if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box))
                {
                    Entity_LoseHealth(mob, bullet->damage);
                    bullet->alive = Jfalse;
                }
            }
        }
        else
        {
            //wow I really need to improve this mess
            //put the collision detection on the zombie's side
            //and pass the attack direction to the player
            Box* temp = BoundingBox_CreateTemp(bullet);
            Direction bullet_coming_from = BoundingBox_CheckCollision(&bullet->box, temp, &world->player.box);
            if(bullet_coming_from != None && world->player.invulnerability_timer <= 0)
            {
                Entity* collision_direction[5] = {NULL};
                collision_direction[bullet_coming_from] = bullet;
                Player_TakeDamage(&world->player, collision_direction);

                bullet->alive = Jfalse;
            }

            free(temp);
        }

        for(int i = 0 ; i < world->map_size ; i++)
        {
            if(world->map[i] != NULL)
            {
                if(Entity_CheckNear(bullet, world->map[i]))
                {
                    if(BoundingBox_CheckSimpleCollision(&bullet->box, &world->map[i]->box))
                    {
                        bullet->alive = Jfalse;
                    }
                }

            }

        }

    }
}

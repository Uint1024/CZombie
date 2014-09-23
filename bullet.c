#include <math.h>
#include <stdlib.h>
#include "linkedList.h"
#include "bullet.h"
#include "entity.h"
#include "world.h"
#include "stdio.h"
#include "player.h"
#include "zombie.h"
#include "movement_component.h"

Entity* Bullet_Create(Weapon_Type type, float x, float y, float angle, float speed, bool is_ennemy_bullet)
{
	Entity* bullet = Entity_Spawn();

	bullet->t = Cat_Bullet;
	bullet->sub_category = type;
	bullet->x = x;
	bullet->y = y;
	bullet->movementC = MovementC_Create();
	bullet->movementC->angle = angle;
	bullet->movementC->speed = speed;
	bullet->is_ennemy = is_ennemy_bullet;
	bullet->alive_timer = 2000; //die in 2 seconds
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
     case NB_WEAPON_TYPES:
        printf("Error, trying to create undefined weapon");
        break;
	}

    BoundingBox_Create(bullet, bullet->box.width, bullet->box.height);
	return bullet;
}

void Bullet_Update(Entity* bullet, World* world)
{


    bullet->alive_timer -= delta_g;
    if(bullet->alive_timer <= 0)
    {
        bullet->alive = false;
    }


    if(bullet->alive)
    {
        bullet->movementC->dx = cos(bullet->movementC->angle) * bullet->movementC->speed * delta_g;
        bullet->movementC->dy = sin(bullet->movementC->angle) * bullet->movementC->speed  * delta_g;

        moveEntity(bullet, bullet->movementC->dx, bullet->movementC->dy);


       if(!bullet->is_ennemy)
        {
            for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
            {
                Entity* mob = (struct Entity*)Vector_Get(&world->monsters_vector, i);
                if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box))
                {
                    Zombie_GetAttacked(mob, bullet->damage, world);
                    bullet->alive = false;
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
            if(bullet_coming_from != None && world->player.playerC->invulnerability_timer <= 0)
            {
                Entity* collision_direction[5] = {NULL};
                collision_direction[bullet_coming_from] = bullet;
                Player_TakeDamage(&world->player, collision_direction);

                bullet->alive = false;


            }

            free(temp);
        }



        for(int i = 0 ; i < world->map_size && bullet->alive; i++)
        {
            if(world->map[i] != NULL)
            {
                if(Entity_CheckNear(bullet, world->map[i]) && world->map[i]->solid)
                {

                    if(BoundingBox_CheckSimpleCollision(&bullet->box, &world->map[i]->box))
                    {

                        bullet->alive = false;

                    }
                }

            }

        }

    }


}

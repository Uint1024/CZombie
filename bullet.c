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
#include "weapon.h"
#include "wall.h"


Entity* Bullet_Create(float x, float y,
                      float angle,
                      Weapon* parent)
{
	Entity* bullet = Entity_Spawn();

	bullet->t = Cat_Bullet;
	bullet->sub_category = parent->type;
	bullet->x = x;
	bullet->y = y;
	bullet->movementC = MovementC_Create();
	bullet->angle = angle;
	bullet->movementC->speed = parent->bullet_speed;
    bullet->movementC->dx = cos(angle) * bullet->movementC->speed;
    bullet->movementC->dy = sin(angle) * bullet->movementC->speed;
	bullet->is_ennemy = parent->is_monster;
	bullet->alive_timer = (1/bullet->movementC->speed) * 1500;
	bullet->damage = parent->bullet_damage;


    bullet->box.height = parent->bullet_height;
    bullet->box.width = parent->bullet_width;
    bullet->penetration_chance = parent->bullet_penetration_chance;
    bullet->last_zombie_hit = NULL;
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
        moveEntity(bullet, bullet->movementC->dx * delta_g, bullet->movementC->dy * delta_g);


       if(!bullet->is_ennemy)
        {
            for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
            {
                Entity* mob = (struct Entity*)Vector_Get(&world->monsters_vector, i);
                if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box) &&
                    mob != bullet->last_zombie_hit)
                {
                    bullet->last_zombie_hit = mob;
                    Zombie_GetAttacked(mob, bullet->damage, world);
                    int random = rand() % 1000;


                    //modulo the address by 1000 to get a kinda random number
                    if(bullet->nb_penetrations < 4 &&
                        random < bullet->penetration_chance)
                    {
                        bullet->nb_penetrations++;
                    }
                    else
                    {
                        bullet->alive = false;
                    }

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



        for(int i = 0 ; i < Vector_Count(&world->non_null_walls) && bullet->alive; i++)
        {
            Entity* wall = (Entity*)Vector_Get(&world->non_null_walls, i);

            if(
               Entity_CheckNear(bullet, wall) &&
               wall->solid &&
               BoundingBox_CheckSimpleCollision(&bullet->box, &wall->box))
            {
                Structure_GetAttacked(wall, bullet);

                bullet->alive = false;
            }

        }

    }


}

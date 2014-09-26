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


Entity* Bullet_Create(Weapon_Type type, float x, float y,
                      float angle, float speed, bool is_ennemy_bullet,
                      Weapon* parent)
{
	Entity* bullet = Entity_Spawn();

	bullet->t = Cat_Bullet;
	bullet->sub_category = type;
	bullet->x = x;
	bullet->y = y;
	bullet->movementC = MovementC_Create();
	bullet->movementC->angle = angle;
	bullet->movementC->speed = speed;
    bullet->movementC->dx = cos(angle) * speed;
    bullet->movementC->dy = sin(angle) * speed;
	bullet->is_ennemy = is_ennemy_bullet;
	bullet->alive_timer = 4000; //die in 4 seconds
	bullet->damage = 1;


    bullet->box.height = parent->bullet_height;
    bullet->box.width = parent->bullet_width;

	switch(type)
	{
    case Weapon_Handgun:
        bullet->penetration_chance = 400;
        bullet->damage = 2;
        break;
    case Weapon_AutomaticRifle:
        bullet->penetration_chance = 500;
        bullet->damage = 2;
        break;
    case Weapon_Shotgun:
        bullet->penetration_chance = 600;
        bullet->damage = 1;
        break;
    case Weapon_Fireball:
        bullet->penetration_chance = 0;
        bullet->damage = 1;
        break;
    case Weapon_TripleFireball:
        bullet->penetration_chance = 0;
        bullet->damage = 1;
        break;
    case Weapon_TheBigGun:
        bullet->penetration_chance = 700;
        bullet->damage = 5;
        break;
    case No_Weapon:
        printf("Error, trying to create a bullet without weapon type");
        break;
    case Weapon_GrenadeLauncher:
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


        moveEntity(bullet, bullet->movementC->dx * delta_g, bullet->movementC->dy * delta_g);


       if(!bullet->is_ennemy)
        {
            for(int i = 0 ; i < Vector_Count(&world->monsters_vector) ; i++)
            {
                Entity* mob = (struct Entity*)Vector_Get(&world->monsters_vector, i);
                if (BoundingBox_CheckSimpleCollision(&bullet->box, &mob->box))
                {


                    Zombie_GetAttacked(mob, bullet->damage, world);
                    int random = rand() % 1000;


                    //modulo the address by 1000 to get a kinda random number
                    if(bullet->nb_penetrations < 3 &&
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

            if(wall != bullet->hit_wall &&
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

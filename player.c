#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"

void Player_Move(Entity* p, Entity* map, int map_size, Entity* camera, List* bonus_list, int delta, List* mob_list)
{
	camera->dx = 0;
	camera->dy = 0;
    CalculateVelocity(p, map, map_size);
    Player_CheckBonusCollision(p, bonus_list);
    p->dx = floor(p->dx);
    p->dy = floor(p->dy);
    moveEntity(p, p->dx, p->dy);

    MoveCamera(p->camera, p->dx , p->dy );
    //printf("%f\n", p->camera->x);
    /*if(p->tempFixForCamera == 0)
    {
        MoveCamera(camera, -p->dx -1, -p->dy -1);
        p->tempFixForCamera = 1;
    }
    else if(p->tempFixForCamera == 1)
    {
        MoveCamera(camera, -p->dx + 1, -p->dy + 1);
        p->tempFixForCamera = 0;
    }*/
	//int random = rand() % 2;
	//if (random == 0) {MoveCamera(camera, 0.01, 0.01);}
	//if (random == 1) {MoveCamera(camera, -0.01, -0.01);}
}


Entity* Player_Create(float x, float y, int w, int h)
{
	Entity* ent = (Entity*)malloc(sizeof(Entity));
	ent->t = Square;
	ent->x = x;
	ent->y = y;
	ent->dx = 0;
	ent->dy = 0;
	BoundingBox_Create(ent, w, h);
	ent->speed = 0.5;
	ent->last_creation = 0;
	for(int i = 0 ; i < 5 ; i++)
        ent->bullets[i] = 250;

    ent->magazine_max_bullets = 50;
    ent->magazine_bullets = 50;
    ent->current_weapon = AutomaticRifle_w;
    ent->last_shoot = 0;
    ent->last_reload = 0;
    ent->reloading = Jfalse;
    ent->tempFixForCamera = 0;

    ent->camera = CreateCamera();

	return ent;
}

void Player_CheckBonusCollision(Entity* player, List* bonus_list)
{
	//Box* temp = BoundingBox_CreateTemp(player);
    Jbool collision = Jfalse;
    ListNode *_nodeB = NULL;
    for(_nodeB = bonus_list->first; _nodeB != NULL; _nodeB = _nodeB->next)
    {
        collision = Jfalse;
        Entity* bonus = (struct Entity*)_nodeB->value;
        collision = BoundingBox_CheckSimpleCollision(&player->box, &bonus->box);
        if(collision)
        {
            if(bonus->t == Ammo)
            {
                player->bullets[player->current_weapon] += bonus->bullets[AutomaticRifle];
                bonus->alive = Jfalse;
            }
        }
    }
}

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

}


Entity* Player_Create(float x, float y, int w, int h)
{
    Entity* p = Entity_Spawn();
	//Entity* ent = (Entity*)malloc(sizeof(Entity));
	p->t = Player;
	p->texture = Player_tex;
	p->x = x;
	p->y = y;
	BoundingBox_Create(p, w, h);
	p->speed = 0.5;
	for(int i = 0 ; i < 5 ; i++)
        p->bullets[i] = 250;

    p->magazine_max_bullets = 50;
    p->magazine_bullets = 50;
    p->current_weapon = AutomaticRifle_w;

    p->camera = CreateCamera();

	return p;
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

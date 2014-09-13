#include "math.h"
#include "stdio.h"
#include "player.h"
#include "entity.h"
#include "camera.h"
#include "linkedList.h"
#include "weapon.h"
#include "weapons_component.h"

void Player_Move(Entity* p, Entity* map, int map_size, Entity* camera, List* bonus_list, int delta, List* mob_list)
{
    if(p->weapons_component->reloading)
    {
        WeaponsComponent_Reload(p->weapons_component);
    }

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

	p->t = Player;
	p->texture = Player_tex;
	p->x = x;
	p->y = y;
	BoundingBox_Create(p, w, h);
	p->speed = 0.6;

    p->camera = CreateCamera();

    p->weapons_component = WeaponsComponent_Create();
    WeaponsComponent_AddWeaponToInventory(p->weapons_component, Weapon_Create(AutomaticRifle_w));
    WeaponsComponent_ChangeWeapon(p->weapons_component, AutomaticRifle_w);

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
                WeaponsComponent_AddAmmo(player->weapons_component, AutomaticRifle_w, 50);
                bonus->alive = Jfalse;
            }
        }
    }
}


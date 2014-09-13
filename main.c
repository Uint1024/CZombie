#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
//#include <GL/glew.h>
#include "string.h"

#include "entity.h"
#include "controls.h"
#include "linkedList.h"
#include "bullet.h"
#include "common.h"
#include "wall.h"
#include "camera.h"
#include "player.h"
#include "zombie.h"

Jbool debug_mode = Jfalse;

Entity* createSquare(float x, float y, int w, int h)
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
	return ent;
}

void Render(SDL_Renderer* renderer, Entity* ent, SDL_Texture* textures[15], TTF_Font** font, Entity* camera);
void RenderPlayer(SDL_Renderer* renderer, Entity* ent, SDL_Texture* texture);
void Move_With_Camera(Entity* ent, Entity* camera);
void RenderText(SDL_Renderer* renderer, char* text, float x, float y, TTF_Font* font);
void RenderWorld(SDL_Renderer* renderer, Entity* map,
                 int map_size, List* monsters, List* bullets, Entity* player,
                 SDL_Texture** textures, List* bonus_list, TTF_Font* font);


void Render(SDL_Renderer* renderer, Entity* ent, SDL_Texture* textures[15],
            TTF_Font** font, Entity* camera)
{
	const SDL_Rect rect = { ent->box.left - camera->x, ent->box.top - camera->y, ent->box.width, ent->box.height };

	if (ent->t == Zombie || ent->t == Ammo || ent->t == Bullet)
	{
		SDL_RenderCopyEx(renderer, textures[ent->texture], NULL, &rect, ent->angle * 57.32f, NULL, SDL_FLIP_NONE);
		//SDL_RenderCopy(renderer, textures[1], NULL, &rect);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);

		SDL_RenderFillRect(renderer, &rect);
	}
	if(debug_mode)
    {
        SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
        SDL_RenderDrawRect(renderer, &rect);

        char* position_str[20];
        snprintf(position_str, sizeof(position_str), "%d:%d", (int)ent->x, (int)ent->y);

        RenderText(renderer, position_str, ent->x - camera->x, ent->y - camera->y, font[Small]);
    }
}

void RenderPlayer(SDL_Renderer* renderer, Entity* ent, SDL_Texture* texture)
{
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	const SDL_Rect rect = { ent->box.left - ent->camera->x, ent->box.top - ent->camera->y, ent->box.width, ent->box.height };
	//SDL_RenderFillRect(renderer, &rect);

	SDL_RenderCopyEx(renderer, texture, NULL, &rect, ent->angle * 57.32f, NULL, SDL_FLIP_NONE);

SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderDrawPoint(renderer, ent->muzzleX, ent->muzzleY);
}


/*void Move_With_Camera(Entity* ent, Entity* camera)
{
	moveEntity(ent, camera->dx, camera->dy);

}*/

void RenderText(SDL_Renderer* renderer, char* text, float x, float y, TTF_Font* font)
{


    SDL_Color text_color = { 0, 255, 0, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, text_color);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textH = 0;
    int textW = 0;

    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);

    SDL_Rect textRect = { x, y, textW, textH };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void RenderWorld(SDL_Renderer* renderer, Entity* map,
                 int map_size, List* monsters, List* bullets, Entity* player,
                 SDL_Texture** textures, List* bonus_list, TTF_Font* font)
{
    RenderPlayer(renderer, player, textures[Player_tex]);

    for (int i = 0; i < map_size; i++)
    {
        if (map[i].t == Wall)
        {
            Render(renderer, &map[i], textures, font, player->camera);
        }
    }

    ListNode *_nodeM = NULL;
    for(_nodeM = monsters->first; _nodeM != NULL; _nodeM = _nodeM->next)
    {
        Entity* mob = (struct Entity*)_nodeM->value;
        if (mob->t == Zombie)
        {
            Render(renderer, mob, textures, font, player->camera);
        }
    }

    ListNode *_nodeB = NULL;
    for(_nodeB = bullets->first; _nodeB != NULL; _nodeB = _nodeB->next)
    {
        Entity* bullet = (struct Entity*)_nodeB->value;

        Render(renderer, bullet, textures, font, player->camera);
    }

    _nodeB = NULL;
    for(_nodeB = bonus_list->first; _nodeB != NULL; _nodeB = _nodeB->next)
    {
        Entity* bonus = (struct Entity*)_nodeB->value;

        Render(renderer, bonus, textures, font, player->camera);
    }
}

void Update(Entity* map, int map_size, List* monsters,
            List* bullets, Entity* player, Entity* camera, int delta,
            List* bonus_list)
{
    Player_Move(player, map, map_size, &player->camera, bonus_list, delta, monsters);

    ListNode *_node3 = bullets->first;
    while (_node3 != NULL)
    {
        Entity* bullet = (struct Entity*)_node3->value;
        void* ptr_to_next = _node3->next;

        if (bullet->t == Bullet)
        {
            Bullet_Move(bullet, map, map_size, monsters, delta, player->camera);

            if (bullet->alive == Jfalse)
            {
                List_remove(bullets, _node3);
            }
        }
        _node3 = ptr_to_next;
        ptr_to_next = NULL;
    }


    ListNode *_nodeMonster = monsters->first;
    while (_nodeMonster != NULL)
    {
        Entity* mob = (struct Entity*)_nodeMonster->value;
        void* ptr_to_next = _nodeMonster->next;
        if (mob->t == Zombie)
        {
            UpdateZombie(mob, player, map, map_size, monsters, delta);

            if (mob->alive == Jfalse)
            {
                Zombie_Die(mob, bonus_list);
                List_remove(monsters, _nodeMonster);
            }
        }
        _nodeMonster = ptr_to_next;
        ptr_to_next = NULL;
    }

     ListNode *_nodeB = bonus_list->first;
    while(_nodeB != NULL)
    {
        void* ptr_to_next = _nodeB->next;
        Entity* bonus = (struct Entity*)_nodeB->value;

        if(!bonus->alive)
        {
            List_remove(bonus_list, _nodeB);
        }

        _nodeB = ptr_to_next;
    }



}

int main(int argc, char* args[])
{
    srand(time(NULL));
	int screen_width = 1080;
	int screen_height = 768;
	SDL_Window* window = SDL_CreateWindow("	C Game",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen_width,
		screen_height,
		SDL_WINDOW_SHOWN |
		SDL_WINDOW_OPENGL);
	SDL_GL_SwapWindow(window);
	SDL_GL_CreateContext(window);
	SDL_Renderer* renderer = SDL_CreateRenderer(window,
		-1,
		SDL_RENDERER_ACCELERATED);

	//glewInit();

	Jbool running = Jtrue;

	Entity* player = Player_Create(screen_width / 2 - 10, screen_height / 2 - 10, 20, 20);

	Controls* controls = CreateControls();
	List* bullets = List_create();

	int map_width = 50;
	int map_height = 50;
	int map_size = map_width * map_height;

	Entity* map;
	map = calloc(map_size, sizeof(Entity));

    List* bonus_list = List_create();
	List* monsters = List_create();


	int time_now = SDL_GetTicks();
	int time_last_frame = 0;
	int delta = 1;
	float fps = 0;

	SDL_Texture* textures[15];

	textures[Player_tex] = IMG_LoadTexture(renderer, "player.png");
	textures[Zombie_tex] = IMG_LoadTexture(renderer, "zombie.png");
	textures[Ammo_Bonus_tex] = IMG_LoadTexture(renderer, "bullet_bonus.png");
	textures[Bullet_tex] = IMG_LoadTexture(renderer, "bullet.png");

	TTF_Init();

	TTF_Font* font[5];
	font[Small] = TTF_OpenFont("cour.ttf", 7);
	font[Medium] = TTF_OpenFont("cour.ttf", 16);

	char fps_str[6];
    char delta_str[4];
    int chrono_frame = 0;
    int time_last_frame_real = 0;
    int time_this_frame_real = 0;
    char reloading_str[11] = "Reloading!";


	while (running)
	{
	    time_this_frame_real = SDL_GetTicks();


        chrono_frame += time_this_frame_real - time_last_frame_real;



        //1000/7 = 140 frames per second
        if(chrono_frame > 7)
        {
            time_now = SDL_GetTicks();
            delta = time_now - time_last_frame;
            if(delta > 0)
            {
                fps = 1000 / chrono_frame;
            }

            SDL_SetRenderDrawColor(renderer, 0xE5, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer);

            chrono_frame = 0;
            running = PoolInputs(controls, player->camera);

            ProcessInputs(controls, player, bullets, map, map_width, map_height, monsters, delta);


            if (fps > 0)
            {
                snprintf(delta_str, 4, "%d", delta);
                snprintf(fps_str, 6, "%f", fps);
                char full_txt_fps[80];
                snprintf(full_txt_fps, sizeof full_txt_fps, "%s%s%s%s", fps_str, " FPS - ", delta_str, " ms since last frame.");
                RenderText(renderer, full_txt_fps, 700, 50, font[Medium]);
            }

            if(debug_mode)
            {
                char* nb_of_monsters[50];
                snprintf(nb_of_monsters, sizeof(nb_of_monsters), "%d%s", monsters->count, " monsters on screen.");
                RenderText(renderer, nb_of_monsters, 700, 100, font[Medium]);

                char* nb_of_bullets_on_screen[50];
                snprintf(nb_of_bullets_on_screen, sizeof(nb_of_bullets_on_screen), "%d%s", bullets->count, " bullets on screen.");
                RenderText(renderer, nb_of_bullets_on_screen, 700, 120, font[Medium]);


            }

            char* nb_of_bullets_on_player[70];
            snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d%s%d%s%d",
                     player->magazine_bullets, " / ",
                     player->magazine_max_bullets, " / ",
                     player->bullets[player->current_weapon]
                     );
            RenderText(renderer, nb_of_bullets_on_player, 700, 150, font[Medium]);

            if(player->reloading)
            {
                RenderText(renderer, reloading_str, player->x - player->camera->x - 20, player->y - 20 - player->camera->y, font[Medium]);
            }

            Update(map, map_size, monsters, bullets, player, &player->camera, delta, bonus_list);
            RenderWorld(renderer, map, map_size, monsters, bullets, player, textures, bonus_list, font);


            SDL_RenderPresent(renderer);
            time_last_frame = time_now;

        }

        time_last_frame_real = time_this_frame_real;
	}

	//SDL_FreeSurface(textSurface);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);



	SDL_Quit();
	return 0;
}

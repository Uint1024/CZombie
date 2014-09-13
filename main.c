#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
#include "graphics.h"
#include "weapon.h"
#include "weapons_component.h"
#include "bonus.h"
#include "vector.h"

Jbool debug_mode = Jfalse;


void Update(Entity* map, int map_size,
            Entity* player, int delta,
            Vector* bullets_vector,
            Vector* bonus_vector, Vector* monsters_vector)
{
    Player_Move(player, map, map_size, delta, monsters_vector, bonus_vector);

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {
            Entity* bullet = (Entity*)Vector_Get(bullets_vector, i);
            Bullet_Move(bullet, map, map_size, monsters_vector, delta, player->camera);

            if (bullet->alive == Jfalse)
            {
                Vector_Delete(bullets_vector, i);
            }
        }
    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
            Bonus_Update(bonus, player);
            if (bonus->alive == Jfalse)
            {
                Vector_Delete(bonus_vector, i);
            }
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
            if(mob->t == Zombie)
            {
                UpdateZombie(mob, player, map, map_size, monsters_vector, delta);

                if (mob->alive == Jfalse)
                {
                    Zombie_Die(mob, bonus_vector);
                    Vector_Delete(monsters_vector, i);

                }
            }
        }
    }
}

int main(int argc, char* args[])
{
    srand(time(NULL));
	int screen_width = 1080;
	int screen_height = 768;

    Graphics* graphics = Graphics_Create(screen_width, screen_height);
	Jbool running = Jtrue;

	Entity* player = Player_Create(screen_width / 2 - 10, screen_height / 2 - 10, 20, 20);

	Controls* controls = CreateControls();

    Vector bullets_vector = Vector_Create();
    Vector bonus_vector = Vector_Create();
    Vector monsters_vector = Vector_Create();

	int map_width = 50;
	int map_height = 50;
	int map_size = map_width * map_height;

	Entity* map;
	map = calloc(map_size, sizeof(Entity));

	int time_now = SDL_GetTicks();
	int time_last_frame = 0;
	int delta = 1;
	float fps = 0;

	char fps_str[6];
    char delta_str[4];
    int chrono_update = 0;
    int chrono_render = 0;
    int time_last_frame_real = 0;
    int time_this_frame_real = 0;
    char reloading_str[11] = "Reloading!";


	while (running)
	{
	    time_this_frame_real = SDL_GetTicks();


        chrono_update += time_this_frame_real - time_last_frame_real;



        //1000/7 = 140 frames per second
        if(chrono_update > 14)
        {
            time_now = SDL_GetTicks();
            delta = time_now - time_last_frame;
            if(delta > 0)
            {
                fps = 1000 / chrono_update;
            }

            SDL_SetRenderDrawColor(graphics->renderer, 0xE5, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(graphics->renderer);

            chrono_update = 0;
            running = PoolInputs(controls, player->camera);

            ProcessInputs(controls, player, map, map_width, map_height, delta, &bullets_vector, &monsters_vector);


            if (fps > 0)
            {
                snprintf(delta_str, 4, "%d", delta);
                snprintf(fps_str, 6, "%f", fps);
                char full_txt_fps[80];
                snprintf(full_txt_fps, sizeof full_txt_fps, "%s%s%s%s", fps_str, " FPS - ", delta_str, " ms since last frame.");
                Graphics_RenderText(graphics, full_txt_fps, Medium, 700, 50);
            }
/*
            if(debug_mode)
            {
                char nb_of_monsters[50];
                snprintf(nb_of_monsters, sizeof(nb_of_monsters), "%d%s", monsters->count, " monsters on screen.");
                Graphics_RenderText(graphics, nb_of_monsters, Medium, 700, 100);

                char nb_of_bullets_on_screen[50];
                snprintf(nb_of_bullets_on_screen, sizeof(nb_of_bullets_on_screen), "%d%s", bullets->count, " bullets on screen.");
                Graphics_RenderText(graphics, nb_of_bullets_on_screen, Medium, 700, 120);


            }
*/
            Graphics_RenderText(graphics, player->weapons_component->current_weapon->name, Medium, 700, 130);

            char nb_of_bullets_on_player[70];
            snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d%s%d%s%d",
                     player->weapons_component->current_weapon->magazine_bullets, " / ",
                     player->weapons_component->current_weapon->magazine_max_bullets, " / ",
                     player->weapons_component->bullets[player->weapons_component->current_weapon->type]
                     );
            Graphics_RenderText(graphics, nb_of_bullets_on_player, Medium, 700, 150);

            if(player->weapons_component->reloading)
            {
                Graphics_RenderText(graphics, reloading_str, Medium, player->x - player->camera->x - 20, player->y - 20 - player->camera->y);
            }

            Update(map, map_size, player, delta, &bullets_vector, &bonus_vector, &monsters_vector);
            Graphics_RenderWorld(graphics, map, map_size, player, &bullets_vector, &bonus_vector, &monsters_vector);
            Graphics_Flip(graphics);
            time_last_frame = time_now;

        }

        time_last_frame_real = time_this_frame_real;
	}

	SDL_DestroyRenderer(graphics->renderer);
	SDL_DestroyWindow(graphics->window);



	SDL_Quit();
	return 0;
}

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
#include "explosion.h"
#include "menu_manager.h"
#include "world.h"

Jbool debug_mode = Jfalse;


void Update(int delta, World* world)
{
    //because I don't want to type world-> 50 times
    Vector* bullets_vector = &world->bullets_vector;
    Vector* bonus_vector = &world->bonus_vector;
    Vector* monsters_vector = &world->monsters_vector;
    Vector* explosions_vector = &world->explosions_vector;

    Player_Update(delta, world);

    for(int i = 0 ; i < Vector_Count(bullets_vector) ; i++)
    {
        if(Vector_Get(bullets_vector, i) != NULL)
        {
            Entity* projectile = (Entity*)Vector_Get(bullets_vector, i);

            if(projectile->t == Bullet)
            {
                 Bullet_Update(projectile, delta, world);
            }
            else if(projectile->t == Grenade)
            {
                Grenade_Update(projectile, delta, world);

                if(!projectile->alive)
                {
                    Vector_Push(explosions_vector, Explosion_Create(projectile->x - 64, projectile->y - 64) );
                }
            }

            if (projectile->alive == Jfalse)
            {
                Vector_Delete(bullets_vector, i);
            }
        }
        else
        {
            printf("Error during update of bullets vector : bullet = NULL");
        }
    }

    for(int i = 0 ; i < Vector_Count(bonus_vector) ; i++)
    {
        if(Vector_Get(bonus_vector, i) != NULL)
        {
            Entity* bonus = (Entity*)Vector_Get(bonus_vector, i);
            Bonus_Update(bonus, &world->player);
            if (bonus->alive == Jfalse)
            {
                Vector_Delete(bonus_vector, i);
            }
        }
        else
        {
            printf("Error during update of bonus vector : bonus = NULL");
        }
    }

    for(int i = 0 ; i < Vector_Count(monsters_vector) ; i++)
    {
        if(Vector_Get(monsters_vector, i) != NULL)
        {
            Entity* mob = (struct Entity*)Vector_Get(monsters_vector, i);
            if(mob->t == Zombie)
            {
                Zombie_Update(mob, delta, world);

                if (mob->alive == Jfalse)
                {
                    Zombie_Die(mob, bonus_vector);
                    Vector_Delete(monsters_vector, i);

                }
            }
        }
        else
        {
            printf("Error during update of monsters vector : monster = NULL");
        }
    }

    for(int i = 0 ; i < Vector_Count(explosions_vector) ; i++)
    {
        Entity* exp = (Entity*)Vector_Get(explosions_vector, i);
        Explosion_Update(exp, delta);
        if (!exp->alive)
        {
            Vector_Delete(explosions_vector, i);
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

	//Entity* player = Player_Create(screen_width / 2 - 10, screen_height / 2 - 10, 20, 20);

	Controls* controls = CreateControls();

    Vector bullets_vector = Vector_Create();
    Vector bonus_vector = Vector_Create();
    Vector monsters_vector = Vector_Create();
    Vector explosions_vector = Vector_Create();

	int map_width = 50;
	int map_height = 50;
	int map_size = map_width * map_height;

	Entity* map;
	map = calloc(map_size, sizeof(Entity));

    World world = World_Initialize(50, 50, screen_width, screen_height);
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
    char reloading_str[25] = "Reloading!";

    Jbool game_started = Jtrue;

    MenuManager menu_manager = MenuManager_Create(graphics);
    int ms_delay_between_frame = 1000 / 60;

	while (running)
	{
	    time_this_frame_real = SDL_GetTicks();


        chrono_update += time_this_frame_real - time_last_frame_real;

        //1000/7 = 140 frames per second
        if(chrono_update > ms_delay_between_frame)
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
            running = PoolInputs(controls, world.player.camera);

            ProcessInputs(controls, delta, &game_started, &world);


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

            if(game_started)
            {
                Graphics_RenderText(graphics, world.player.weapons_component->current_weapon->name, Medium, 700, 130);

                char nb_of_bullets_on_player[70];
                snprintf(nb_of_bullets_on_player, sizeof(nb_of_bullets_on_player), "%d%s%d%s%d",
                         world.player.weapons_component->current_weapon->magazine_bullets, " / ",
                         world.player.weapons_component->current_weapon->magazine_max_bullets, " / ",
                         world.player.weapons_component->bullets[world.player.weapons_component->current_weapon->type]
                         );
                Graphics_RenderText(graphics, nb_of_bullets_on_player, Medium, 700, 150);

                if(world.player.weapons_component->reloading)
                {
                    reloading_str[25] = "";
                    snprintf(reloading_str, sizeof(reloading_str), "Reloading (%d)",
                             world.player.weapons_component->reload_timer);
                    Graphics_RenderText(graphics, reloading_str,
                                        Medium,
                                        world.player.x - world.player.camera->x - 20,
                                        world.player.y - 20 - world.player.camera->y);
                }

                Update(delta, &world);
                Graphics_RenderWorld(graphics, &world);
            }
            else //show menu
            {
                Graphics_RenderMenu(graphics, menu_manager.active_menu);
                MenuManager_Update(&menu_manager, controls, &game_started, &running, delta);
            }

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

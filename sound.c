#include "sound.h"
#include "SDL_mixer.h"
#include "stdio.h"
#include "stdbool.h"

static Mix_Chunk* sound_wallDestroyed1;
static Mix_Chunk* sound_explosion1;
static Mix_Chunk* sound_shot1;
static Mix_Chunk* sound_shot2;
static Mix_Chunk* sound_shot_shotgun1;
static Mix_Chunk* sound_shot_handgun1;
static Mix_Chunk* sound_takeItem;
static Mix_Chunk* sound_pickupCoin;
static Mix_Chunk* sound_openDoor;
static Mix_Chunk* sound_shoot_grenadeLauncher;
static int shot_sound_delay = 150;
static int shot_sound_timer = 0;//time of last shot sound

void Sound_Init()
{
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024);

    sound_wallDestroyed1 = Mix_LoadWAV("sounds/Explosion5.wav");
    sound_explosion1 = Mix_LoadWAV("sounds/Explosion16.wav");
    sound_shot1 = Mix_LoadWAV("sounds/Hit_Hurt12.wav");
    sound_shot2 = Mix_LoadWAV("sounds/Randomize409.wav");
    sound_takeItem = Mix_LoadWAV("sounds/Randomize399.wav");
    sound_shot_shotgun1 = Mix_LoadWAV("sounds/shotgun.wav");
    sound_shoot_grenadeLauncher = Mix_LoadWAV("sounds/shoot_handgun4.wav");
    sound_shot_handgun1 = Mix_LoadWAV("sounds/shoot_handgun.wav");
    sound_pickupCoin = Mix_LoadWAV("sounds/coin_pickup.wav");
    sound_openDoor = Mix_LoadWAV("sounds/open_door.wav");
    Mix_AllocateChannels(100);

}

void Sound_PlayOpenDoor()
{

    Mix_PlayChannel(-1, sound_openDoor, 0 );
}

void Sound_PlayWallDestroyed()
{
    Mix_PlayChannel(-1, sound_wallDestroyed1, 0 );
}

void Sound_PlayExplosion()
{
    Mix_PlayChannel(-1, sound_explosion1, 0 );
}


void Sound_PlayPickUp()
{
    Mix_PlayChannel(-1, sound_pickupCoin, 0 );
}

void Sound_PlayShot(Weapon_Type type)
{
    switch(type)
    {
    case Weapon_Handgun:
            Mix_PlayChannel(-1, sound_shot_handgun1, 0 );
            break;
    case Weapon_AutomaticRifle:
            Mix_PlayChannel(-1, sound_shot1, 0 );
            break;
    case Weapon_Shotgun:
            Mix_PlayChannel(-1, sound_shot_shotgun1, 0 );
            break;
    case Weapon_TheBigGun:
            Mix_PlayChannel(-1, sound_shot2, 0 );
            break;
    case Weapon_GrenadeLauncher:
            Mix_PlayChannel(-1, sound_shoot_grenadeLauncher, 0 );
            break;
    }

}

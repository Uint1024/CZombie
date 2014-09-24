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
    sound_shot_handgun1 = Mix_LoadWAV("sounds/handgun.wav");
    Mix_VolumeChunk(sound_shot1, 40);
    Mix_VolumeChunk(sound_shot2, 40);
     Mix_VolumeChunk(sound_shot_handgun1, 40);
    Mix_VolumeChunk(sound_shot_shotgun1, 40);
    Mix_VolumeChunk(sound_takeItem, 15);
    //Mix_AllocateChannels(100);
    Mix_PlayChannel(47, sound_shot1, 0 );


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
    Mix_PlayChannel(-1, sound_takeItem, 0 );
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
    }

}

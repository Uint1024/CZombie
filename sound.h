#ifndef SOUND_H
#define SOUND_H
#include "common.h"

void Sound_Init();
void Sound_PlayWallDestroyed();
void Sound_PlayExplosion();
void Sound_PlayPickUp();
void Sound_PlayShot(Weapon_Type type);
void Sound_PlayOpenDoor();
#endif // SOUND_H

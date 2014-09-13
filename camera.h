#ifndef CAMERA_H
#define CAMERA_H
typedef struct Entity Entity;

Entity* CreateCamera();
void MoveCamera(Entity* c, float dx, float dy);
#endif


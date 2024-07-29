#ifndef BURRO_MESH_H
#define BURRO_MESH_H

#include "painlessMesh.h"
#include "burro_leds.h"

#define MESH_PREFIX "S3cur3_W1f1_M35h"
#define MESH_PASSWORD ""
#define MESH_PORT 1337

extern Scheduler userScheduler;
extern painlessMesh mesh;
extern int points;

void initMesh();
void sendMessage();
void printNodeList();
uint32_t updateMesh();
void setPoints();

#endif // BURRO_MESH_H

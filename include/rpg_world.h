#ifndef __RPG_WORLD_H__
#define __RPG_WORLD_H__

#include "game.h"

typedef struct
{
	Entity *ground;
	Entity *water;
	Entity *buildings;
	Entity *trees;


}World;

void world_init(Vector3D position);

void water_init(Vector3D position);

void buildings_init(Uint32 building_count, Vector3D position);

#endif
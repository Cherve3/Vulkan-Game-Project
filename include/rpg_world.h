#ifndef __RPG_WORLD_H__
#define __RPG_WORLD_H__

#include "gf3d_entity.h"

typedef struct
{
	Entity *terrain;
	Entity *water;
	Entity **buildings;
	Entity **trees;

}World;

void rpg_world_init();

#endif
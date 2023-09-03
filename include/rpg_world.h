#ifndef __RPG_WORLD_H__
#define __RPG_WORLD_H__

#include "game.h"

typedef struct
{
	Entity *terrain;
	Entity *water;
	Entity **buildings;
	Entity **trees;

}World;

void rpg_world_init();

#endif
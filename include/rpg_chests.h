#ifndef __CHESTS_H__
#define __CHESTS_H__

#include "gfc_types.h"

#include "gf3d_entity.h"

#include "rpg_collision.h"
#include "rpg_items.h"

typedef struct
{
	Uint8			_inuse;
	Item			*loot;
	Entity			*ent;
	CircleCollider	interactBounds;

}Chest;

void rpg_chests_free(Chest *chest);

void rpg_chests_close();

Chest *rpg_chest_new();

void chest_init();

void rpg_chest_fill(Item *item, int lootSize);

void rpg_chest_open();

#endif

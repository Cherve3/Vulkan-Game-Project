#ifndef __CHESTS_H__
#define __CHESTS_H__

#include "gfc_types.h"

#include "rpg_items.h"

typedef struct
{

	Item *loot;
}Chest;

void chest_init();

void chest_destroy();

#endif

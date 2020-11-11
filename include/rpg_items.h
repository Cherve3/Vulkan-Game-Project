#ifndef __RPG_ITEMS_H__
#define __RPG_ITEMS_H__

#include "gfc_vector.h"

#include "gfc_types.h"

typedef enum
{
	consumable,
	material,
	weapon,
	armor,
}ItemType;

typedef struct
{
	ItemType type;
	char *name;
	char *description;

	Uint8 armor;
	Uint8 damage;

	Uint8 quantity;
	float weight;

}Item;

Item rpg_item_new();

Item *rpg_item_spawn(Vector3D position);

#endif

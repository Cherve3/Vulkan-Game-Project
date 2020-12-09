#ifndef __RPG_ITEMS_H__
#define __RPG_ITEMS_H__

#include "gfc_vector.h"
#include "gfc_types.h"

#include "gf3d_entity.h"

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
	Uint8	 _inuse;
	char *name;
	char *description;

	Entity *ent;

	Uint8 armor;
	Uint8 damage;

	Uint8 cost;
	Uint8 quantity;
	float weight;

}Item;

Item rpg_item_new_random(int random);

Item rpg_item_new_consumable(char* name, Uint8 quantity);

Item rpg_item_new_material(char* name, Uint8 quantity);

Item rpg_item_new_weapon(char* name, Uint8 quantity);

Item rpg_item_new_armor(char* name, Uint8 quantity);

Item *rpg_item_new(ItemType type, char* name, Vector3D position);

#endif

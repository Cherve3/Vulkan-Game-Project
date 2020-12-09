
#include "simple_logger.h"
#include "simple_json.h"

#include "rpg_items.h"

typedef struct
{
	Item *item_list;
	Uint32 item_count;

}ItemManager;

static ItemManager items = { 0 };
static SJson *item_info;

void rpg_item_close()
{
	int i;
	if (items.item_list != NULL)
	{
		for (i = 0; i < items.item_count; i++)
		{
			gf3d_entity_free(&items.item_list[i]);
		}
		free(items.item_list);
	}
	memset(&items, 0, sizeof(ItemManager));
	slog("Item System Closed");
}

void rpg_item_entity_init(Uint32 maxItems)
{
	if (items.item_list != NULL)
	{
		slog("WARNING: Item system already initialized");
		return;
	}
	items.item_list = gfc_allocate_array(sizeof(Item), maxItems);
	if (!items.item_list)
	{
		slog("failed to allocate item list");
		return;
	}
	items.item_count = maxItems;
	atexit(rpg_item_close);
	slog("Item System Initialized");
}

//TODO: Finish item setup 
Item rpg_item_new_random(int random)
{
	switch ( random )
	{
		case consumable:

			return rpg_item_new_consumable("Health Potion", 1);
			break;
		case material:

			return rpg_item_new_material("Wood Log", 1);
			break;
		case weapon:

			return rpg_item_new_weapon("Wooden Sword", 1);
			break;
		case armor:

			return rpg_item_new_armor("Leather Boots", 1);
			break;
	}


}

Item rpg_item_new_consumable(char* name, Uint8 quantity)
{
	Item item;
	SJson *item_info = sj_load("json/items.json");

	if (!item_info)
	{
		slog("Failed to load json data %s", sj_get_error());
		return;
	}

	item_info = sj_object_get_value(item_info, name);

	sj_get_integer_value(sj_object_get_value(item_info, "damage"), &item.damage);
	sj_get_float_value(sj_object_get_value(item_info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(item_info, "cost"), &item.cost);

	item.name = sj_get_string_value(sj_object_get_value(item_info, "name"));
	item.description = sj_get_string_value(sj_object_get_value(item_info, "description"));

	sj_free(item_info);
	item_info = NULL;

	item.quantity = quantity;
	item.type = consumable;
	item.armor = 0;
	slog("Consumable: %s Quantity: %i", item.name, item.quantity);

	return item;
}

Item rpg_item_new_material(char* name, Uint8 quantity)
{
	Item item;
	item_info = sj_load("json/items.json");

	if (!item_info)
	{
		slog("Failed to load json data %s", sj_get_error());
		return;
	}
	item_info = sj_object_get_value(item_info, name);

	sj_get_float_value(sj_object_get_value(item_info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(item_info, "cost"), &item.cost);

	item.name = sj_get_string_value(sj_object_get_value(item_info, "name"));
	item.description = sj_get_string_value(sj_object_get_value(item_info, "description"));

	sj_free(item_info);

	item.quantity = quantity;
	item.type = material;
	item.armor = 0;
	item.damage = 0;
	slog("Material: %s Quantity: %i", name, quantity);
	slog("NAME: %s	DESCRIPTION: %s", item.name, item.description);

	return item;
}

Item rpg_item_new_weapon(char* name, Uint8 quantity)
{
	Item item;
	item.quantity = quantity;
	item.type = weapon;
	item.armor = 0;
	slog("Weapon: %s Quantity: %i", name, quantity);
	if (strcmp(name, "Wooden Sword") == 0)
	{
		item.damage = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A mid-sized sword made of a solid wood. It is not very sharp but I don't want to get hit by it.";
		item.cost = 100;
		return item;
	}
	if (strcmp(name, "Iron Sword") == 0)
	{
		item.damage = 15;
		item.name = name;
		item.weight = 10.0;
		item.description = "A mid-sized cast iron sword. The quality is questionable but it will work.";
		item.cost = 1000;
		return item;
	}
	if (strcmp(name, "Dagger") == 0)
	{
		item.damage = 10;
		item.name = name;
		item.weight = 3.0;
		item.description = "A small thin metal weapon. Stick 'em with the pointy end.";
		item.cost = 600;
		return item;
	}
	if (strcmp(name, "Bow") == 0)
	{
		item.damage = 5;
		item.name = name;
		item.weight = 3.0;
		item.description = "A mid-sized sword made of a solid wood. It is not very sharp but I don't want to get hit by it.";
		item.cost = 250;
		return item;
	}
	if (strcmp(name, "Hammer") == 0)
	{
		item.damage = 20;
		item.name = name;
		item.weight = 20.0;
		item.description = "A thick blunt metal object with a tiny handle. That's what she said...";
		item.cost = 1000;
		return item;
	}
}

Item rpg_item_new_armor(char* name, Uint8 quantity)
{
	Item item;
	item.quantity = quantity;
	item.type = armor;
	item.damage = 0;
	slog("Weapon: %s Quantity: %i", name, quantity);
	if (strcmp(name, "Leather Cap") == 0)
	{
		item.armor = 1;
		item.name = name;
		item.weight = 1.0;
		item.description = "A worn leather cap. It is your cap right?";
		item.cost = 50;
		return item;
	}
	if (strcmp(name, "Leather Cuirass") == 0)
	{
		item.armor = 15;
		item.name = name;
		item.weight = 15.0;
		item.description = "A cuirass crafted from leather. It is better than nothing.";
		item.cost = 100;
		return item;
	}
	if (strcmp(name, "Leather Greaves") == 0)
	{
		item.armor = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A small leather covering for your legs.";
		item.cost = 50;
		return item;
	}
	if (strcmp(name, "Leather Boots") == 0)
	{
		item.armor = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A fairly comfy pair of leather boots. Keep your toes out of the dirt.";
		item.cost = 50;
		return item;
	}
}

void rpg_item_collect()
{
	slog("Collecting item");
}

void rpg_item_create(Item item, ItemType type, char *name)
{

	switch (type)
	{
	case consumable:
		item = rpg_item_new_consumable(name, 1);
		return item;
		break;
	case material:
		item = rpg_item_new_material(name, 1);

		return item;
		break;
	case weapon:
		item = rpg_item_new_weapon(name, 1);

		return item;
		break;
	case armor:
		item = rpg_item_new_armor(name, 1);

		return item;
		break;
	default:
		slog("invalid item type");
		break;
	}
}

Item *rpg_item_new(ItemType type, char* name, Vector3D position)
{
	int i;
	for (i = 0; i < items.item_count; i++)
	{
		if (!items.item_list[i]._inuse)
		{
			rpg_item_create(items.item_list[i],type,name);
			items.item_list[i]._inuse = 1;

			items.item_list[i].ent = gf3d_entity_new();
			items.item_list[i].ent->model = gf3d_model_load("log");
			items.item_list[i].ent->position = position;
			gfc_matrix_scale(items.item_list[i].ent->modelMatrix, vector3d(2,2,2), items.item_list[i].ent->modelMatrix);
			gfc_matrix_translate(items.item_list[i].ent->modelMatrix,position);
			items.item_list[i].ent->type = ITEM;
			items.item_list[i].ent->name = name;
			items.item_list[i].ent->boxCollider.width = 6;
			items.item_list[i].ent->boxCollider.height = 4;
			items.item_list[i].ent->boxCollider.depth = 6;
			items.item_list[i].ent->interact = rpg_item_collect;
			items.item_list[i].ent->boxCollider.x = items.item_list[i].ent->position.x;
			items.item_list[i].ent->boxCollider.y = items.item_list[i].ent->position.y;
			items.item_list[i].ent->boxCollider.z = items.item_list[i].ent->position.z;

			return &items.item_list[i];
		}
	}
	slog("Failed to provide new item, no unused slots");
	return NULL;
}

#include "simple_logger.h"
#include "simple_json.h"

#include "rpg_items.h"
#include "rpg_player.h"
#include "rpg_levelup.h"

typedef struct
{
	Item *item_list;
	Uint32 item_count;

}ItemManager;

static ItemManager items = { 0 };

static SJson *item_info			= NULL;
static SJson *consumable_info	= NULL;
static SJson *material_info		= NULL;
static SJson *weapons			= NULL;
static SJson *armors			= NULL;
static SJson *weapon_info		= NULL;
static SJson *armor_info		= NULL;

void print_item_manager();

void rpg_item_close()
{
	int i;

	sj_free(item_info);
	sj_free(consumable_info);
	sj_free(material_info);
	sj_free(weapons);
	sj_free(armors);
	sj_free(weapon_info);
	sj_free(armor_info);
	if (items.item_list != NULL)
	{
		for (i = 0; i < items.item_count; i++)
		{
			gf3d_entity_free(&items.item_list[i].ent);
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
	if (!items.item_list){slog("failed to allocate item list");return;}

	item_info		= sj_load("json/items.json");
	consumable_info = sj_object_get_value(item_info, "Consumable");
	material_info	= sj_object_get_value(item_info, "Material");
	armors			= sj_load("json/armor.json");
	armor_info = sj_object_get_value(armors, "Armor");
	weapons			= sj_load("json/weapons.json");
	weapon_info = sj_object_get_value(weapons, "Weapons");

	if (!item_info){ slog("failed to load item info"); return; }
	if (!consumable_info){ slog("failed to load consumable info"); return; }
	if (!material_info){ slog("failed to load material info"); return; }
	if (!armor_info){ slog("failed to load armor info"); return; }
	if(!weapon_info){ slog("failed to load weapon info"); return; }

	items.item_count = maxItems;

	atexit(rpg_item_close);
	slog("Item System Initialized");
}

//TODO: randomize each type of item
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
	SJson *info = sj_object_get_value(consumable_info, name);

	if (!info)
	{
		slog("Consumable %s",name);
		slog("Failed to load json data %s", sj_get_error());
		return;
	}

	sj_get_integer_value(sj_object_get_value(info, "damage"), &item.damage);
	sj_get_float_value(sj_object_get_value(info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item.cost);

	item.name			= sj_get_string_value(sj_object_get_value(info, "name"));
	item.description	= sj_get_string_value(sj_object_get_value(info, "description"));

	item.quantity	= quantity;
	item.type		= consumable;
	item.armor		= 0;

	slog("Consumable: %s Quantity: %i", item.name, item.quantity);

	return item;
}

Item rpg_item_new_material(char* name, Uint8 quantity)
{
	Item item;
	SJson *info = sj_object_get_value(material_info, name);

	if (!info)
	{
		slog("Material %s", name);
		slog("Failed to load json data %s", sj_get_error());
		return;
	}

	sj_get_float_value(sj_object_get_value(info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item.cost);

	item.name			= sj_get_string_value(sj_object_get_value(info, "name"));
	item.description	= sj_get_string_value(sj_object_get_value(info, "description"));

	item.quantity	= quantity;
	item.type		= material;
	item.armor		= 0;
	item.damage		= 0;

	slog("Material: %s Quantity: %i", name, quantity);

	return item;
}

Item rpg_item_new_weapon(char* name, Uint8 quantity)
{
	Item item;
	SJson *info = sj_object_get_value(weapon_info, name);

	if (!info)
	{
		slog("Weapon %s",name);
		slog("Failed to load weapon json data %s", sj_get_error());
		return;
	}

	sj_get_integer_value(sj_object_get_value(info, "damage"), &item.damage);
	sj_get_float_value(sj_object_get_value(info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item.cost);

	item.name = sj_get_string_value(sj_object_get_value(info, "name"));
	item.description = sj_get_string_value(sj_object_get_value(info, "description"));

	item.quantity = quantity;
	item.type = weapon;
	item.armor = 0;
	slog("Weapon: %s Quantity: %i", name, quantity);

	return item;
}

Item rpg_item_new_armor(char* name, Uint8 quantity)
{
	Item item;
	SJson *info = sj_object_get_value(armor_info, name);

	if (!info)
	{
		slog("Armor %s",name);
		slog("Failed to load armor json data %s", sj_get_error());
		return;
	}

	sj_get_integer_value(sj_object_get_value(info, "armor"), &item.damage);
	sj_get_float_value(sj_object_get_value(info, "weight"), &item.weight);
	sj_get_integer_value(sj_object_get_value(info, "cost"), &item.cost);

	item.name = sj_get_string_value(sj_object_get_value(info, "name"));
	item.description = sj_get_string_value(sj_object_get_value(info, "description"));

	item.quantity = quantity;
	item.type = armor;
	item.damage = 0;

	slog("Weapon: %s Quantity: %i", name, quantity);

	return item;

}

void rpg_item_free(Item *item)
{
	int i;
	if (!item) return;
	memset(item, 0, sizeof(Item));
}

void rpg_item_collect(Entity *self)
{
	Item *item = (Item*)(self->data);
	slog("Item data: %s", item->name);
	slog("Collecting item");
	int i = 0, j;
	slog("Item Name: %s", self->name);
	for (i = 0; i < get_player()->inventory.bagSize; i++)
	{
		if (get_player()->inventory.bag[i]._inuse)
		{
			slog("strcmp %i", strcmp(get_player()->inventory.bag[i].name, self->name));
			slog("Player inventory: %s Item: %s", get_player()->inventory.bag[i].name, self->name);
			if (strcmp(get_player()->inventory.bag[i].name,self->name) == 0)
			{
				slog("updating quantity");
				stat_counter("itemscollected");
				get_player()->stats.carry_weight += item->weight;

				get_player()->inventory.bag[i].quantity++;
				get_player()->inventory.bag[i].weight +=  item->weight;
				item->_inuse = 0;
				self->_inuse = 0;
//				gf3d_entity_free(&self);
				rpg_item_free(item);
				return;
			}
		}
	}
	
	for (i = 0; i < get_player()->inventory.bagSize; i++)
	{
		if (!get_player()->inventory.bag[i]._inuse)
		{
			slog("adding item to inventory");
			stat_counter("itemscollected");
			get_player()->stats.carry_weight += item->weight;

			get_player()->inventory.bag[i].armor = item->armor;
			get_player()->inventory.bag[i].cost = item->cost;
			get_player()->inventory.bag[i].damage = item->damage;
			get_player()->inventory.bag[i].description = item->description;
			get_player()->inventory.bag[i].name = item->name;
			get_player()->inventory.bag[i].quantity = item->quantity;
			get_player()->inventory.bag[i].type = item->type;
			get_player()->inventory.bag[i].weight = item->weight;
			get_player()->inventory.bag[i]._inuse = 1;
			item->_inuse = 0;
			self->_inuse = 0;

			rpg_item_free(item);
			return;
		}
	}
	if (!self->_inuse)
		slog("Item could not be added to inventory");
}

Item rpg_item_create(Item item, ItemType type, char *name)
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
			items.item_list[i] = rpg_item_create(items.item_list[i], type, name);
			items.item_list[i]._inuse = 1;

			items.item_list[i].ent = gf3d_entity_new();
			items.item_list[i].ent->model = gf3d_model_load(name);
			items.item_list[i].ent->position = position;
			gfc_matrix_scale(items.item_list[i].ent->modelMatrix, vector3d(2,2,2), items.item_list[i].ent->modelMatrix);
			gfc_matrix_translate(items.item_list[i].ent->modelMatrix,position);
			items.item_list[i].ent->type = ITEM;
			items.item_list[i].ent->name = name;
			items.item_list[i].ent->boxCollider.width = 2;
			items.item_list[i].ent->boxCollider.height = 2;
			items.item_list[i].ent->boxCollider.depth = 2;
			items.item_list[i].ent->interact = rpg_item_collect;
			items.item_list[i].ent->boxCollider.x = items.item_list[i].ent->position.x;
			items.item_list[i].ent->boxCollider.y = items.item_list[i].ent->position.y;
			items.item_list[i].ent->boxCollider.z = items.item_list[i].ent->position.z;
			
			items.item_list[i].ent->data = (void*)&items.item_list[i];

			//print_item_manager();

			return &items.item_list[i];
		}
	}
	
	slog("Failed to provide new item, no unused slots");
	return NULL;
}

void print_item_manager()
{
	int i;
	for (i = 0; i < items.item_count; i++)
	{
		slog("Item name: %s", items.item_list[i].name);
	}
}
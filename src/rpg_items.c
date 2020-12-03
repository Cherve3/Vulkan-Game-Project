
#include "simple_logger.h"

#include "rpg_items.h"

//TODO: Finish item setup 
Item rpg_item_new_random(int random)
{
	switch ( random )
	{
		case consumable:

			return rpg_item_new_consumable("Health Potion", 1);
			break;
		case material:

			return rpg_item_new_material("Wooden Log", 1);
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
	item.quantity = quantity;
	item.type = consumable;
	slog("Consumable: %s Quantity: %i", name, quantity);
	if (strcmp(name, "Health Potion"))
	{
		item.damage = -10;
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass filled with a red glowing liquid. It will fortify your health.";
		item.armor = 0;
		item.cost = 20;
		return item;
	}
	if (strcmp(name, "Mana Potion"))
	{
		item.damage = -10;
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass filled with a blue glowing liquid. It will fortify your mana.";
		item.armor = 0;
		item.cost = 20;
		return item;
	}
	if (strcmp(name, "Stamina Potion"))
	{
		item.damage = -10;
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass filled with a green glowing liquid. It will fortify your stamina.";
		item.armor = 0;
		item.cost = 20;
		return item;
	}
	if (strcmp(name, "Arrow"))
	{
		item.damage = 10;
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass filled with a green glowing liquid. It will fortify your stamina.";
		item.armor = 0;
		item.cost = 5;
		return item;
	}
}

Item rpg_item_new_material(char* name, Uint8 quantity)
{
	Item item;
	item.quantity = quantity;
	item.type = material;
	item.armor = 0;
	item.damage = 0;
	slog("Material: %s Quantity: %i", name, quantity);
	if (strcmp(name, "Wood Log"))
	{
		item.name = name;
		item.quantity = 1;
		item.weight = 3.0;
		item.description = "A sturdy looking wood log. It can be used for crafting.";
		item.cost = 30;
		return item;
	}
	if (strcmp(name, "Herb"))
	{
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass bottle filled with a blue glowing liquid. It will fortify your mana.";
		item.cost = 20;
		return item;
	}
	if (strcmp(name, "Water"))
	{
		item.name = name;
		item.weight = 0.1;
		item.description = "A glass bottle filled with water. Where did you get that?.";
		item.cost = 20;
		return item;
	}
}

Item rpg_item_new_weapon(char* name, Uint8 quantity)
{
	Item item;
	item.quantity = quantity;
	item.type = weapon;
	item.armor = 0;
	slog("Weapon: %s Quantity: %i", name, quantity);
	if (strcmp(name, "Wooden Sword"))
	{
		item.damage = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A mid-sized sword made of a solid wood. It is not very sharp but I don't want to get hit by it.";
		item.cost = 100;
		return item;
	}
	if (strcmp(name, "Iron Sword"))
	{
		item.damage = 15;
		item.name = name;
		item.weight = 10.0;
		item.description = "A mid-sized cast iron sword. The quality is questionable but it will work.";
		item.cost = 1000;
		return item;
	}
	if (strcmp(name, "Dagger"))
	{
		item.damage = 10;
		item.name = name;
		item.weight = 3.0;
		item.description = "A small thin metal weapon. Stick 'em with the pointy end.";
		item.cost = 600;
		return item;
	}
	if (strcmp(name, "Bow"))
	{
		item.damage = 5;
		item.name = name;
		item.weight = 3.0;
		item.description = "A mid-sized sword made of a solid wood. It is not very sharp but I don't want to get hit by it.";
		item.cost = 250;
		return item;
	}
	if (strcmp(name, "Hammer"))
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
	if (strcmp(name, "Leather Cap"))
	{
		item.armor = 1;
		item.name = name;
		item.weight = 1.0;
		item.description = "A worn leather cap. It is your cap right?";
		item.cost = 50;
		return item;
	}
	if (strcmp(name, "Leather Cuirass"))
	{
		item.armor = 15;
		item.name = name;
		item.weight = 15.0;
		item.description = "A cuirass crafted from leather. It is better than nothing.";
		item.cost = 100;
		return item;
	}
	if (strcmp(name, "Leather Greaves"))
	{
		item.armor = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A small leather covering for your legs.";
		item.cost = 50;
		return item;
	}
	if (strcmp(name, "Leather Boots"))
	{
		item.armor = 5;
		item.name = name;
		item.weight = 5.0;
		item.description = "A fairly comfy pair of leather boots. Keep your toes out of the dirt.";
		item.cost = 50;
		return item;
	}
}

ItemEntity *rpg_item_spawn(ItemType type, char* name, Vector3D position)
{
	ItemEntity *item;

	item->ent = gf3d_entity_new();
	item->ent->model = gf3d_model_load(name);
	item->ent->position = position;

	switch (type)
	{
	case consumable:
		item->item = rpg_item_new_consumable(name, 1);
		return item;
		break;
	case material:
		item->item = rpg_item_new_material(name, 1);

		return item;
		break;
	case weapon:
		item->item = rpg_item_new_weapon(name, 1);

		return item;
		break;
	case armor:
		item->item = rpg_item_new_armor(name, 1);

		return item;
		break;
	}
}
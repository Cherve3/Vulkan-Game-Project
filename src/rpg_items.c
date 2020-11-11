
#include "simple_logger.h"

#include "rpg_items.h"


Item rpg_item_new()
{
	Item item;
	srand(time(NULL));
	float random = rand() % 3;
	item.type = (int)random;
	slog("Random: %f \nItemType: %i", random,item.type);
	switch (item.type)
	{
		case consumable:

			slog("Consumable: %i ", consumable);
			item.damage = -10;
			item.name = "Health Potion";
			item.quantity = 1;
			item.weight = 0.1;
			item.description = "A glass filled with a red glowing liquid. It will fortify your health.";
			item.armor = 0;
			return item;
			break;
		case material:

			slog("Material: %i", material);
			item.damage = 0;
			item.name = "Wood";
			item.quantity = 1;
			item.weight = 3;
			item.description = "A sturdy looking wood log. It can be used for crafting";
			item.armor = 0;
			return item;
			break;
		case weapon:

			slog("Weapon: %i", weapon);
			item.damage = 5;
			item.name = "Wooden Sword";
			item.quantity = 1;
			item.weight = 0.1;
			item.description = "A glass filled with a red glowing liquid. It will fortify your health.";
			item.armor = 0;
			return item;
			break;
		case armor:

			slog("Armor: %i", armor);
			item.damage = 0;
			item.name = "Leather Boots";
			item.quantity = 1;
			item.weight = 1;
			item.description = "A pair of slightly worn leather boots. If your feet could talk they would not thank you.";
			item.armor = 5;
			return item;
			break;
	}


}
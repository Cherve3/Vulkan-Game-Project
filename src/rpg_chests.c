
#include "simple_logger.h"

#include "rpg_chests.h"
#include "rpg_items.h"
#include "rpg_player.h"
#include "rpg_levelup.h"

typedef struct
{
	Chest *chest_list;
	Uint8 chest_count;
}ChestManager;

ChestManager rpg_chests = { 0 };

time_t t;

void rpg_chest_loot_free(Item *item)
{
	if (!item) return;
	memset(item, 0, sizeof(Item));
}

void rpg_chests_free(Chest *chest)
{
	int i;
	if (!chest) return;
	for (i = 0; i < chest->lootSize; i++)
	{
		rpg_chest_loot_free(&chest->loot[i]);
	}
	chest->lootSize = 0;
	
	memset(chest, 0, sizeof(Chest));
}

void rpg_chest_despawn(Chest *chest)
{
	int i;
	if (!chest) return;
	for (i = 0; i < chest->lootSize; i++)
	{
		chest->loot[i]._inuse = 0;
	}
	chest->_inuse = 0;
	memset(chest, 0, sizeof(Chest));

}

void rpg_chests_close()
{
	int i;
	if (rpg_chests.chest_list != NULL)
	{
		for (i = 0; i < rpg_chests.chest_count; i++)
		{
			rpg_chests_free(&rpg_chests.chest_list[i]);
		}
		free(rpg_chests.chest_list);
	}
	memset(&rpg_chests, 0, sizeof(ChestManager));
	slog("Chest System Closed");
}

Chest *rpg_chest_new()
{
	int i;
	char* id, name;
	for (i = 0; i < rpg_chests.chest_count; i++)
	{
		if (!rpg_chests.chest_list[i]._inuse)
		{
			rpg_chests.chest_list[i]._inuse = 1;

			rpg_chests.chest_list[i].ent = gf3d_entity_new();
			rpg_chests.chest_list[i].ent->model = gf3d_model_load("chest");
			rpg_chests.chest_list[i].ent->position = vector3d_create(-15, 2.4, 0);
			rpg_chests.chest_list[i].ent->type = INTERACT;
			rpg_chests.chest_list[i].ent->name = "Chest";
			rpg_chests.chest_list[i].ent->boxCollider.width  = 4;
			rpg_chests.chest_list[i].ent->boxCollider.height = 3;
			rpg_chests.chest_list[i].ent->boxCollider.depth  = 2;
			rpg_chests.chest_list[i].ent->interact = rpg_chest_open;
			rpg_chests.chest_list[i].ent->boxCollider.x = rpg_chests.chest_list[i].ent->position.x;
			rpg_chests.chest_list[i].ent->boxCollider.y = rpg_chests.chest_list[i].ent->position.y;
			rpg_chests.chest_list[i].ent->boxCollider.z = rpg_chests.chest_list[i].ent->position.z;
	
			matrix4d_translate(rpg_chests.chest_list[i].ent->position, rpg_chests.chest_list[i].ent->modelMatrix);

			float lootSize = rand() % 10;
			slog("LootSize: %f", lootSize);
			rpg_chests.chest_list[i].lootSize = lootSize;
			
			rpg_chests.chest_list[i].loot = (Item *)gfc_allocate_array(sizeof(Item), (int)lootSize);
			
			rpg_chest_fill(rpg_chests.chest_list[i].loot, (int)lootSize);

			return &rpg_chests.chest_list[i];
		}
	}
	slog("Failed to provide new chest, no unused slots");
	return NULL;
}


void rpg_chests_init(Uint8 maxChests)
{
	if (rpg_chests.chest_list != NULL)
	{
		slog("WARNING: Chest system already initialized");
		return;
	}
	rpg_chests.chest_list = gfc_allocate_array(sizeof(Chest), maxChests);
	if (!rpg_chests.chest_list)
	{
		slog("failed to allocate chest list");
		return;
	}
	rpg_chests.chest_count = maxChests;
	srand((unsigned)time(&t));
	atexit(rpg_chests_close);
	slog("Chest System Initialized");
}

void rpg_chest_fill(Item *item, int lootSize)
{
	if (!item){
		slog("Chest item is NULL");
		return;
	}

	slog("LootSize: %i", lootSize);
	int i;
	for (i = 0; i < lootSize; i++)
	{
		
		item[i] = rpg_item_new_random((int)rand() % 4);
		item[i]._inuse = 1;
	}

}

void rpg_chest_open()
{
	slog("Opening Chest");
	int i = 0, j;

	for (j = 0; j < rpg_chests.chest_list[0].lootSize; j++)
	{
		do{
			slog("NAME: %s	DESCRIPTION: %s", rpg_chests.chest_list[0].loot[j].name, rpg_chests.chest_list[0].loot[j].description);
			if (get_player()->inventory.bag[i].name == rpg_chests.chest_list[0].loot[j].name)
			{
				slog("updating quantity");
				stat_counter("itemscollected");
				get_player()->stats.carry_weight += rpg_chests.chest_list[0].loot[j].weight;

				get_player()->inventory.bag[i].quantity++;
				get_player()->inventory.bag[i].weight += rpg_chests.chest_list[0].loot[j].weight;
				rpg_chests.chest_list[0].loot[j]._inuse = 0;
			}
			else if (!get_player()->inventory.bag[i]._inuse)
			{
				slog("adding item to inventory");
				stat_counter("itemscollected");
				get_player()->stats.carry_weight += rpg_chests.chest_list[0].loot[j].weight;

				get_player()->inventory.bag[i] = rpg_chests.chest_list[0].loot[j];
				rpg_chests.chest_list[0].loot[j]._inuse = 0;
				
			}
			i++;
			if (i == get_player()->inventory.bagSize + 1)
			{
				slog("There is no space in player inventory");
				break;
			}
		} while (rpg_chests.chest_list[0].loot[j]._inuse);
		i = 0;
	}
	// Destroy Chest
	rpg_chests.chest_list[0].ent->_inuse = 0;
	gf3d_entity_free(&rpg_chests.chest_list[0].ent);
	rpg_chest_despawn(&rpg_chests.chest_list[0]);
	rpg_chests_free(&rpg_chests.chest_list[0]);


}

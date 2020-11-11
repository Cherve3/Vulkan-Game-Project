
#include "simple_logger.h"

#include "rpg_chests.h"
#include "rpg_items.h"

typedef struct
{
	Chest *chest_list;
	Uint8 chest_count;
}ChestManager;

ChestManager rpg_chests = { 0 };

void rpg_chest_loot_free(Item *item)
{
	if (!item) return;
	memset(item, 0, sizeof(Item));
}

void rpg_chests_free(Chest *chest)
{
	if (!chest) return;
	rpg_chest_loot_free(chest->loot);
	gf3d_entity_free(chest->ent);
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
	for (i = 0; i < rpg_chests.chest_count; i++)
	{
		if (!rpg_chests.chest_list[i]._inuse)
		{
			rpg_chests.chest_list[i]._inuse = 1;

			rpg_chests.chest_list[i].ent = gf3d_entity_new();
			rpg_chests.chest_list[i].ent->model = gf3d_model_load("chest");
			rpg_chests.chest_list[i].ent->position = vector3d(-15, 0, 0);
			rpg_chests.chest_list[i].ent->name = "Chest";
			rpg_chests.chest_list[i].ent->boxCollider.depth  = 1;
			rpg_chests.chest_list[i].ent->boxCollider.height = 1;
			rpg_chests.chest_list[i].ent->boxCollider.width  = 1;
			rpg_chests.chest_list[i].ent->boxCollider.x = rpg_chests.chest_list[i].ent->position.x;
			rpg_chests.chest_list[i].ent->boxCollider.y = rpg_chests.chest_list[i].ent->position.y;
			rpg_chests.chest_list[i].ent->boxCollider.z = rpg_chests.chest_list[i].ent->position.z;
			gfc_matrix_new_translation(rpg_chests.chest_list[i].ent->modelMatrix, rpg_chests.chest_list[i].ent->position);

			rpg_chests.chest_list[i].interactBounds.radius = 2;

			srand(time(NULL));
			float lootSize = rand() % 10;
			slog("LootSize: %f", lootSize);
			rpg_chests.chest_list[i].loot = (Item *)gfc_allocate_array(sizeof(Item), (int)lootSize);

			rpg_chest_fill(rpg_chests.chest_list[i].loot, (int)lootSize);
			//rpg_chests.chest_list[i].loot[i] = rpg_item_new();

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
	atexit(rpg_chests_close);
	slog("Chest System Initialized");
}

void rpg_chest_fill(Item *item, int lootSize)
{
	if (!item){
		slog("Item is NULL");
		return;
	}

	slog("LootSize: %i", lootSize);
	int i;
	for (i = 0; i < lootSize; i++)
	{
		item[i] = rpg_item_new();
	}

}

void rpg_chest_open()
{

}

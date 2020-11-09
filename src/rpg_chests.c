
#include "simple_logger.h"

#include "rpg_chests.h"

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

			//rpg_chests.chest_list[i].ent = gf3d_entity_new();
			//rpg_chests.chest_list[i].interactBounds.radius = 2;
			//rpg_chests.chest_list[i].ent->position = vector3d(0, 0, 0);
			//gfc_matrix_new_translation(rpg_chests.chest_list[i].ent->modelMatrix, rpg_chests.chest_list[i].ent->position);
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

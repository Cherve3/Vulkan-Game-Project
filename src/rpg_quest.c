#include "simple_json.h"
#include "simple_logger.h"

#include "rpg_quest.h"
#include "rpg_items.h"
#include "rpg_goblin.h"

typedef struct
{
	Quest* quest_list;
	Item** item_spawn_list;
	NPC* goblin_spawn_list;

	int quest_count;
	int current_quest;
}QuestManager;

static QuestManager quests = { 0 };

static SJson* quest_info;

char file_path[60];
char* qpath = "D:/Git/Projects/Vulkan-Game-Project/";

void rpg_quest_close()
{
	int i;
	if (quests.quest_list != NULL)
	{
		//for (i = 0; i < quests.quest_count; i++)
		//{
		//	gf3d_entity_free(&quests.quest_list[i]);
		//}
		free(quests.quest_list);
	}
	memset(&quests, 0, sizeof(QuestManager));
	slog("Quest System Closed");
}

void rpg_quest_init()
{
	quest_info = NULL;
	snprintf(file_path, sizeof(file_path), "%s%s", qpath, "json/quest.json");
	quest_info = sj_load(file_path);
	if (!quest_info)
		slog("Quest info json is null");

	if (quests.quest_list != NULL)
	{
		slog("WARNING: quest system already initialized");
		return;
	}
	quests.current_quest = 0;
	quests.quest_count= 5;

	quests.quest_list = gfc_allocate_array(sizeof(Quest), 5);
	if (!quests.quest_list)
	{
		slog("failed to allocate entity list");
		return;
	}
	atexit(rpg_quest_close);
	slog("Quest System Initialized");
}

void rpg_main_quests_init()
{
	SJson* quest_1 = sj_object_get_value(quest_info, "1");
	SJson* quest_2 = sj_object_get_value(quest_info, "2");
	SJson* quest_3 = sj_object_get_value(quest_info, "3");
	SJson* quest_4 = sj_object_get_value(quest_info, "4");
	SJson* quest_5 = sj_object_get_value(quest_info, "5");

	// Quest 1
	quests.quest_list[0].title			= sj_get_string_value(sj_object_get_value(quest_1, "title"));
	quests.quest_list[0].description	= sj_get_string_value(sj_object_get_value(quest_1, "description"));
	quests.quest_list[0].objective		= sj_get_string_value(sj_object_get_value(quest_1, "objective"));
	quests.quest_list[0].reward			= sj_get_string_value(sj_object_get_value(quest_1, "reward"));

	sj_get_integer_value(sj_object_get_value(quest_1, "bits"), &quests.quest_list[0].bits);
	sj_get_integer_value(sj_object_get_value(quest_1, "exp"), &quests.quest_list[0].exp);

	// Quest 2
	quests.quest_list[1].title			= sj_get_string_value(sj_object_get_value(quest_2, "title"));
	quests.quest_list[1].description	= sj_get_string_value(sj_object_get_value(quest_2, "description"));
	quests.quest_list[1].objective		= sj_get_string_value(sj_object_get_value(quest_2, "objective"));
	quests.quest_list[1].reward			= sj_get_string_value(sj_object_get_value(quest_2, "reward"));

	sj_get_integer_value(sj_object_get_value(quest_2, "bits"), &quests.quest_list[1].bits);
	sj_get_integer_value(sj_object_get_value(quest_2, "exp"), &quests.quest_list[1].exp);

	// Quest 3
	quests.quest_list[2].title			= sj_get_string_value(sj_object_get_value(quest_3, "title"));
	quests.quest_list[2].description	= sj_get_string_value(sj_object_get_value(quest_3, "description"));
	quests.quest_list[2].objective		= sj_get_string_value(sj_object_get_value(quest_3, "objective"));
	quests.quest_list[2].reward			= sj_get_string_value(sj_object_get_value(quest_3, "reward"));

	sj_get_integer_value(sj_object_get_value(quest_3, "bits"), &quests.quest_list[2].bits);
	sj_get_integer_value(sj_object_get_value(quest_3, "exp"), &quests.quest_list[2].exp);

	// Quest 4
	quests.quest_list[3].title			= sj_get_string_value(sj_object_get_value(quest_4, "title"));
	quests.quest_list[3].description	= sj_get_string_value(sj_object_get_value(quest_4, "description"));
	quests.quest_list[3].objective		= sj_get_string_value(sj_object_get_value(quest_4, "objective"));
	quests.quest_list[3].reward			= sj_get_string_value(sj_object_get_value(quest_4, "reward"));

	sj_get_integer_value(sj_object_get_value(quest_4, "bits"), &quests.quest_list[3].bits);
	sj_get_integer_value(sj_object_get_value(quest_4, "exp"), &quests.quest_list[3].exp);

	// Quest 5
	quests.quest_list[4].title			= sj_get_string_value(sj_object_get_value(quest_5, "title"));
	quests.quest_list[4].description	= sj_get_string_value(sj_object_get_value(quest_5, "description"));
	quests.quest_list[4].objective		= sj_get_string_value(sj_object_get_value(quest_5, "objective"));
	quests.quest_list[4].reward			= sj_get_string_value(sj_object_get_value(quest_5, "reward"));

	sj_get_integer_value(sj_object_get_value(quest_5, "bits"), &quests.quest_list[4].bits);
	sj_get_integer_value(sj_object_get_value(quest_5, "exp"), &quests.quest_list[4].exp);
	slog("Main Quest Initialized");
}

Quest rpg_start_quest(int quest_number)
{
	if (quest_number == 1)
	{
		slog("Quest 1 started");
		quests.current_quest = 1;
		quests.item_spawn_list = gfc_allocate_array(sizeof(Item), 5);
		quests.item_spawn_list[0] = rpg_item_new(material, "Wood Log", vector3d_create(-5, 2, 20));
		quests.item_spawn_list[1] = rpg_item_new(material, "Wood Log", vector3d_create(-5, 2, 22.5));
		quests.item_spawn_list[2] = rpg_item_new(material, "Wood Log", vector3d_create(-5, 2, 24.5));
		quests.item_spawn_list[3] = rpg_item_new(material, "Wood Log", vector3d_create(-5, 4, 21.5));
		quests.item_spawn_list[4] = rpg_item_new(material, "Wood Log", vector3d_create(-5, 4, 23.5));

		return quests.quest_list[0];
	}

	if (quest_number == 2)
	{
		slog("Quest 2 started");
		quests.current_quest = 2;
		rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -205));
		rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -210));
		rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -215));
		rpg_goblin_spawn(GoblinGrunt, vector3d_create(-210, 5, -225));
		rpg_goblin_spawn(GoblinGrunt, vector3d_create(-210, 5, -205));

		return quests.quest_list[1];
	}

	if (quest_number == 3)
	{
		slog("Quest 3 started");
		quests.current_quest = 3;
		return quests.quest_list[2];
	}

	if (quest_number == 4)
	{
		slog("Quest 4 started");
		quests.current_quest = 4;
		return quests.quest_list[3];
	}

	if (quest_number == 5)
	{
		slog("Quest 5 started");
		quests.current_quest = 5;
		return quests.quest_list[4];
	}
}

int rpg_get_current_quest()
{
	return quests.current_quest;
}



/*eol@eof*/

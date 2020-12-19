#include "simple_json.h"
#include "gfc_types.h"
#include "simple_logger.h"

#include "rpg_levelup.h"
#include "rpg_player.h"

static SJson *levelup_info;
static SJson *levelup;
static SJson *stat_counters;

LevelUp level = { 0 };
StatCount stat_count = { 0 };

void rpg_levelup_init()
{
	levelup_info = sj_load("json/levelup.json");

	if (!levelup_info)
		slog("Failed to load levelup json data %s", sj_get_error());

	levelup = sj_object_get_value(levelup_info, "Levelup");
	stat_counters = sj_object_get_value(levelup_info, "StatCounters");

}

void update_stat(char *name)
{
	if (!name) return;
	if (strcmp(name, "strength") == 0)
	{
		if (stat_count.strength == 10)
		{
			stat_count.strength = 0;
			get_player()->stats.strength++;
			return;
		}
	}
	if (strcmp(name, "health") == 0)
	{
		if (stat_count.health == 10)
		{
			stat_count.health = 0;
			get_player()->stats.health++;
			return;
		}
	}
	if (strcmp(name, "perception") == 0)
	{
		if (stat_count.perception == 10)
		{
			stat_count.perception = 0;
			get_player()->stats.perception++;
			return;
		}
	}
	if (strcmp(name, "speed") == 0)
	{
		if (stat_count.speed == 10)
		{
			stat_count.speed = 0;
			get_player()->stats.speed++;
			return;
		}
	}
	if (strcmp(name, "willpower") == 0)
	{
		if (stat_count.willpower == 10)
		{
			stat_count.willpower = 0;
			get_player()->stats.willpower++;
			return;
		}
	}
	if (strcmp(name, "dexterity") == 0)
	{
		if (stat_count.dexterity == 10)
		{
			stat_count.dexterity = 0;
			get_player()->stats.dexterity++;
			return;
		}
	}
	if (strcmp(name, "intelligence") == 0)
	{
		if (stat_count.intelligence == 10)
		{
			stat_count.intelligence = 0;
			get_player()->stats.intelligence++;
			return;
		}
	}
	if (strcmp(name, "luck") == 0)
	{
		if (stat_count.luck == 10)
		{
			stat_count.luck = 0;
			get_player()->stats.luck++;
			return;
		}
	}
}


void level_up()
{
	int level2, level3, level4, level5;

	sj_get_integer_value("level2", &level2);
	sj_get_integer_value("level3", &level3);
	sj_get_integer_value("level4", &level4);
	sj_get_integer_value("level5", &level5);

	if (get_player()->stats.exp >= level5)
	{
		slog("Player leveled up");
		get_player()->stats.level++;
	}
	if (get_player()->stats.exp >= level4)
	{
		slog("Player leveled up");
		get_player()->stats.level++;
	}
	if (get_player()->stats.exp >= level3)
	{
		slog("Player leveled up");
		get_player()->stats.level++;
	}
	if (get_player()->stats.exp >= level2)
	{
		slog("Player leveled up");
		get_player()->stats.level++;
	}
	slog("player level: %i", get_player()->stats.level);
}

void update_stat_counter(char *name)
{
	if (strcmp(name, "strength") == 0)
	{
		slog("Players strength increased");
		stat_count.attacks = 0;
		stat_count.strength++;
		stat_count.total++;
	}
	if (strcmp(name, "health") == 0)
	{
		slog("Players health increased");
		stat_count.damage_taken = 0;
		stat_count.health++;
		stat_count.total++;
	}
	if (strcmp(name, "perception") == 0)
	{
		slog("Players perception increased");
		stat_count.npc_convo = 0;
		stat_count.perception++;
		stat_count.total++;
	}
	if (strcmp(name, "speed") == 0)
	{
		slog("Players speed increased");
		stat_count.run_distance = 0;
		stat_count.speed++;
		stat_count.total++;
	}
	if (strcmp(name, "willpower") == 0)
	{
		slog("Players willpower increased");
		stat_count.magic_cast = 0;
		stat_count.willpower++;
		stat_count.total++;
	}
	if (strcmp(name, "dexterity") == 0)
	{
		slog("Players dexterity increased");
		stat_count.jump_amount = 0;
		stat_count.dexterity++;
		stat_count.total++;
	}
	if (strcmp(name, "intelligence") == 0)
	{
		slog("Players intelligence increased");
		stat_count.items_collected = 0;
		stat_count.intelligence++;
		stat_count.total++;
	}
	if (strcmp(name, "luck") == 0)
	{
		slog("Players luck increased");
		stat_count.random = 0;
		stat_count.luck++;
		stat_count.total++;
	}
	if (stat_count.total == 10)
	{
		stat_count.total = 0;
		slog("Player leveled up");
		get_player()->stats.level++;
	}
}

void stat_counter(char *name)
{
	if (!name)return;
	if (strcmp(name, "attacks") == 0)
	{
		stat_count.attacks++;
		slog("attack +1");
		if (stat_count.attacks == 10)
		{
			get_player()->stats.strength++;
			update_stat_counter("strength");
		}
		return;
	}
	if (strcmp(name, "damagetaken") == 0)
	{
		stat_count.damage_taken++;
		slog("damage taken +1");
		if (stat_count.damage_taken == 10)
		{
			get_player()->stats.health++;
			update_stat_counter("health");
		}
		return;
	}
	if (strcmp(name, "npcconvo") == 0)
	{
		stat_count.npc_convo++;
		slog("npc convo +1");
		if (stat_count.npc_convo == 10)
		{
			get_player()->stats.perception++;
			update_stat_counter("perception");
		}
		return;
	}
	if (strcmp(name, "rundistance") == 0)
	{
		stat_count.run_distance++;
		slog("distance run +1");
		if (stat_count.run_distance == 10)
		{
			get_player()->stats.speed++;
			update_stat_counter("speed");
		}
		return;
	}
	if (strcmp(name, "magiccast") == 0)
	{
		stat_count.magic_cast++;
		slog("magic cast +1");
		if (stat_count.magic_cast == 10)
		{
			get_player()->stats.willpower++;
			update_stat_counter("willpower");
		}
		return;
	}
	if (strcmp(name, "jumpamount") == 0)
	{
		stat_count.jump_amount++;
		slog("jump amount +1");
		if (stat_count.jump_amount == 10)
		{
			get_player()->stats.dexterity++;
			update_stat_counter("dexterity");
		}
		return;
	}
	if (strcmp(name, "itemscollected") == 0)
	{
		slog("Items collected +1");
		stat_count.items_collected++;
		if (stat_count.items_collected == 10)
		{
			get_player()->stats.intelligence++;
			update_stat_counter("intelligence");
		}
		return;
	}
	if (strcmp(name, "random") == 0)
	{
		slog("random +1");
		stat_count.random++;
		if (stat_count.random == 10)
		{
			get_player()->stats.luck++;
			update_stat_counter("luck");
		}
		return;
	}
}

/*eol@eof*/

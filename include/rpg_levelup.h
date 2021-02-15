#ifndef __RPG_LEVELUP_H__
#define __RPG_LEVLEUP_H__

typedef struct
{
	int level2;
	int level3;
	int level4;
	int level5;
}LevelUp;

typedef struct
{
	int total;

	int strength;
	int attacks;

	int health;
	int damage_taken;

	int perception;
	int npc_convo;

	int speed;
	int run_distance;

	int willpower;
	int magic_cast;

	int dexterity;
	int jump_amount;

	int intelligence;
	int items_collected;
	int chests_opened;

	int luck;
	int random;
}StatCount;

void rpg_levelup_init();

void level_up();

void stat_counter(char *name);

#endif

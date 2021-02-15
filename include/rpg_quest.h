#ifndef __RPG_QUEST_H__
#define __RPG_QUEST_H__

#include "gfc_types.h"

typedef struct
{
	char* title;
	char* description;
	char* objective;
	char* reward;

	Uint8 bits;
	Uint8 exp;

	Bool complete;
}Quest;

void rpg_quest_init();

void rpg_main_quests_init();

Quest rpg_start_quest(int quest_number);

int rpg_get_current_quest();

#endif

#ifndef __RPG_SPELLBOOK_H__
#define __RPG_SPELLBOOK_H__

#include "gfc_types.h"

typedef enum
{
	healing,
	defensive,
	alteration,
	destructive,
	arcane,
}SpellType;

typedef struct
{
	SpellType type;
	Uint8 cost;
	char *name;
	char *description;
	
}Spell;

void rpg_spellbook_free(Spell *spellbook);

#endif

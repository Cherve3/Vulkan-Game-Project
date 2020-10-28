#include "simple_logger.h"

#include "rpg_spellbook.h"

void rpg_spellbook_free(Spell *spellbook){
	if (!spellbook) return;
	memset(spellbook, 0, sizeof(Spell));
}
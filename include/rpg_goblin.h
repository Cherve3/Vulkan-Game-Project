#ifndef __RPG_GOBLIN_H__
#define __RPG_GOBLIN_H__

#include "gf3d_entity.h"

#include "rpg_npc.h"
#include "rpg_items.h"
#include "rpg_spellbook.h"

/**
*	@purpose this is the definition file for an NPC character.
*/

typedef enum {
	GoblinGrunt,
	GoblinHeavy,
	GoblinArcher
}GoblinType;

/**
*	@brief initialize a goblin grunt character
*/
void rpg_goblin_init(int type, Vector3D position);

/**
*	@brief runs gf3d_entity_new function to add an NPC to entity system
*	@return NULL on error or a pointer to an entity otherwise
*/
Entity *rpg_goblin_new();

/**
*	@brief called once per frame to update NPC state
*	@param self the NPC that is being updated
*/
void rpg_goblin_think(Entity *self);

#endif

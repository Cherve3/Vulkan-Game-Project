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
	GoblinArcher,
	GoblinKing
}GoblinType;

/**
 *	@brief initialize goblin setup
 */
void rpg_goblin_init();

/*
 *  @brief Spawn a new Goblin character
 *	@param type an enum of the goblin type
 *	@param position the initial spawn position of the goblin
 */
void rpg_goblin_spawn(GoblinType type, Vector3D position);

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

void rpg_goblin_search(Entity *player);

void print_goblin_stats(NPC goblin);

NPC get_goblin(int index);

#endif

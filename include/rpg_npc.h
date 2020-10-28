#ifndef __RPG_NPC_H__
#define __RPG_NPC_H__

#include "gf3d_entity.h"

#include "rpg_items.h"
#include "rpg_spellbook.h"

/**
 *	@purpose this is the definition file for an NPC character.
 */


typedef struct{
	char* name;

	Uint8 level;
	Uint32 exp;

	Uint8 life;
	Uint8 life_max;				//100
	Uint8 life_regen;			//10
	Uint8 magic;
	Uint8 magic_max;			//100
	Uint8 magic_regen;			//10
	Uint8 stamina;
	Uint8 stamina_max;			//100
	Uint8 stamina_regen;		//10

	float carry_weight;

	Uint8 strength;
	Uint8 strength_max;
	Uint8 health;
	Uint8 health_max;
	Uint8 perception;
	Uint8 perception_max;
	Uint8 speed;
	Uint8 speed_max;
	Uint8 willpower;
	Uint8 willpower_max;
	Uint8 dexterity;
	Uint8 dexterity_max;
	Uint8 intelligence;
	Uint8 intelligence_max;
	Uint8 luck;
	Uint8 luck_max;

}npcStats;

typedef struct{
	Spell *spellbook;
	Item *bag;
}npcInventory;

/**
*	@brief initialize an NPC character
*/
void rpg_npc_init();

/**
*	@brief runs gf3d_entity_new function to add an NPC to entity system
*	@return NULL on error or a pointer to an entity otherwise
*/
Entity *rpg_npc_new();

/**
 *	@brief called once per frame to update NPC state
 *	@param self the NPC that is being updated
 */
void rpg_npc_think(Entity *self);

#endif

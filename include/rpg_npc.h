#ifndef __RPG_NPC_H__
#define __RPG_NPC_H__

#include "gf3d_entity.h"

#include "rpg_items.h"
#include "rpg_spellbook.h"

/**
 *	@purpose this is the definition file for an NPC character.
 */
typedef enum {
	ItemShop,
	WeaponShop,
	ArmorShop,
	SpellShop,
	Generic,
	Questgiver
}NPCType;

typedef struct{
	char* name;

	Uint8 level;
	Uint32 exp;
	Uint32 bits;			//currency

	Uint8 life;
	Uint8 life_max;
	Uint8 life_regen;
	Uint8 mana;
	Uint8 mana_max;
	Uint8 mana_regen;
	Uint8 stamina;
	Uint8 stamina_max;
	Uint8 stamina_regen;

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


typedef struct NPC_S
{
	NPCType type;
	Entity *ent;
	npcStats stats;
	npcInventory inventory;
}NPC;
/**
*	@brief initialize an NPC setup
*/
void rpg_npc_init();

/*
 *	@brief Spawn a new NPC character
 *	@param type an enum of the goblin type
 *	@param position the initial spawn position of the goblin
 */
void rpg_npc_spawn(NPCType type, Vector3 position);

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

Bool rpg_get_itemshop_toggle();

Bool rpg_get_weaponshop_toggle();

Bool rpg_get_armorshop_toggle();

Bool rpg_get_spellshop_toggle();

Bool rpg_get_generic_toggle();

Bool rpg_get_quest_toggle();

NPC *rpg_get_npc();

void print_npc_stats(NPC npc);

#endif

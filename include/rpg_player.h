#ifndef __RPG_PLAYER_H__
#define __RPG_PLAYER_H__

#include "gf3d_entity.h"

#include "game.h"
#include "rpg_items.h"
#include "rpg_spellbook.h"

/**
*	@purpose this is the definition file for a player character.
*/
typedef struct
{
	Uint8 walking;
	Uint8 crouched;
	Uint8 inAir;
	Uint8 attacking;
	Uint8 onGround;
	Uint8 idle;
	
}PlayerState;

typedef struct PStats_S
{
	char* name;
	
	Uint8 level;
	Uint32 exp;

	Uint8 life;
	Uint8 life_max;				//100
	Uint8 life_regen;			//10
	Uint8 mana;
	Uint8 mana_max;			//100
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

	Bool toggleStats;

}pStats;

typedef struct PInventory_S{
	Spell *spellbook;
	Item *bag;
	Uint8 bagSize;
	Uint8 spellbookSize;
}pInventory;

typedef struct Player_S
{
	Entity *ent;
	pStats stats;
	pInventory inventory;
	PlayerState state;
	CircleCollider interactBound;
}Player;
/**
 *	@brief initialize a player character
 */
void rpg_player_init();

/**
 *	@brief runs gf3d_entity_new function to add player to entity system
 *	@return NULL on error or a pointer to an entity otherwise
 */
Entity *rpg_player_new();

/**
*	@brief called once per frame to update player state
*	@param self the player that is being updated
*/
void rpg_player_think(Entity *self);

void rpg_player_free(Player *player);

void rpg_player_touch(Entity *self);

void rpg_player_inventory_free(pInventory *inventory);

void rpg_player_bag_free(Item *bag);

Entity *get_player_entity();

void rpg_player_move(Entity *self);

Player *get_player();

pStats get_player_stats();

pInventory get_player_inventory();

void player_interaction();

#endif

#ifndef __RPG_PLAYER_H__
#define __RPG_PLAYER_H__

#include "gf3d_entity.h"

#include "game.h"
#include "rpg_items.h"
#include "rpg_spellbook.h"

/**
*	@purpose this is the definition file for a player character.
*/
typedef struct PlayerState_S
{
	Uint8 walking;
	Uint8 running;
	Uint8 crouched;
	Uint8 inAir;
	Uint8 attacking;
	Uint8 onGround;
	Uint8 idle;
	
}PlayerState;

typedef struct PlayerStats_S
{
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
	
	float carry_weight;		//Weight in kilograms
	float carry_weight_max;	//max weight in Kilograms

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
	Bool toggleMap;
	Bool toggleShop;

}PlayerStats;

typedef struct PlayerInventory_S{
	Spell	*spellbook;
	Item	*bag;
	Uint8	 bagSize;
	Uint8	 spellbookSize;
}PlayerInventory;

typedef struct Player_S
{
	Entity			*ent;
	PlayerStats		 stats;
	Vector3D		 acceleration;
	float			 speed;
	float			 max_speed;
	PlayerInventory	 inventory;
	PlayerState		 state;
	CircleCollider	 interactBound;
}Player;

/**
 *	@brief initialize a player character
 */
void rpg_player_init();

/**
*	@brief called once per frame to update player state
*	@param self the player that is being updated
*/
void rpg_player_think(Entity *self);

void rpg_player_free(Player *player);

void rpg_player_touch(Entity *self);

void rpg_player_inventory_free(PlayerInventory *inventory);

void rpg_player_bag_free(Item *bag);

Entity *get_player_entity();

void rpg_player_move(Entity *self);

Player *get_player();

PlayerStats get_player_stats();

PlayerInventory get_player_inventory();

void player_interaction();

void print_player_name();

void print_stats();

#endif

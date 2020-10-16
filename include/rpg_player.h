#ifndef __RPG_PLAYER_H__
#define __RPG_PLAYER_H__

#include "gf3d_entity.h"

/**
*	@purpose this is the definition file for a player character.
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

}Stats;

typedef struct{
	//Spellbook
	//Bag slots
	Uint8 spell;
}Inventory;

/**
 *	@brief initialize a player character
 */
void rpg_player_init();

Entity *rpg_player_new();

void rpg_player_think(Entity *self);

#endif

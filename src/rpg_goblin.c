#include "simple_logger.h"

#include "rpg_goblin.h"
#include "rpg_npc.h"

static NPC *goblin = NULL;
static int count = 0;

void rpg_goblin_move(Entity *self);

void rpg_goblin_init(int type, Vector3D position){
	
	if (!goblin)
		goblin = (NPC *)gfc_allocate_array(sizeof(NPC), 10);

	slog("Goblin Count: %i", count);

	goblin[count].ent						= rpg_goblin_new();

	goblin[count].ent->name					= "Goblin ";
	goblin[count].ent->type					= MONSTER;

	goblin[count].ent->position				= position;
	goblin[count].ent->velocity				= vector3d(0, 0, 0);
	goblin[count].ent->rotation				= vector3d(0, 0, 0);

	goblin[count].ent->boxCollider.width	= 2.0;
	goblin[count].ent->boxCollider.height	= 5.0;
	goblin[count].ent->boxCollider.depth	= 2.0;
	goblin[count].ent->boxCollider.x		= goblin[count].ent->position.x;
	goblin[count].ent->boxCollider.y		= goblin[count].ent->position.y;
	goblin[count].ent->boxCollider.z		= goblin[count].ent->position.z;

	gfc_matrix_new_translation(goblin[count].ent->modelMatrix, goblin[count].ent->position);

	switch (type)
	{
	case GoblinGrunt:

		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name				= "Goblin Grunt";
		goblin[count].stats.level				= 1;
		goblin[count].stats.exp					= 0;
		goblin[count].stats.bits				= 50;		//currency

		goblin[count].stats.life				= 25;
		goblin[count].stats.life_max			= 25;
		goblin[count].stats.life_regen			= 5;
		goblin[count].stats.mana				= 25;
		goblin[count].stats.mana_max			= 25;
		goblin[count].stats.mana_regen			= 5;
		goblin[count].stats.stamina				= 50;
		goblin[count].stats.stamina_max			= 50;
		goblin[count].stats.stamina_regen		= 10;

		goblin[count].stats.carry_weight		= 45.00;	//kilograms

		goblin[count].stats.strength			= 1;
		goblin[count].stats.strength_max		= 100;
		goblin[count].stats.health				= 1;
		goblin[count].stats.health_max			= 100;
		goblin[count].stats.perception			= 1;
		goblin[count].stats.perception_max		= 100;
		goblin[count].stats.speed				= 2;
		goblin[count].stats.speed_max			= 100;
		goblin[count].stats.willpower			= 1;
		goblin[count].stats.willpower_max		= 100;
		goblin[count].stats.dexterity			= 1;
		goblin[count].stats.dexterity_max		= 100;
		goblin[count].stats.intelligence		= 1;
		goblin[count].stats.intelligence_max	= 100;
		goblin[count].stats.luck				= 1;
		goblin[count].stats.luck_max			= 100;

		count++;
		break;
	case GoblinHeavy:							//Goblin Heavy

		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;
		gfc_matrix_scale(goblin[count].ent->modelMatrix,vector3d(1.2,1.2,1.2));

		goblin[count].stats.name				= "Goblin Heavy";
		goblin[count].stats.level				= 1;
		goblin[count].stats.exp					= 0;
		goblin[count].stats.bits				= 100;		//currency

		goblin[count].stats.life				= 100;
		goblin[count].stats.life_max			= 100;
		goblin[count].stats.life_regen			= 10;
		goblin[count].stats.mana				= 25;
		goblin[count].stats.mana_max			= 25;
		goblin[count].stats.mana_regen			= 5;
		goblin[count].stats.stamina				= 50;
		goblin[count].stats.stamina_max			= 50;
		goblin[count].stats.stamina_regen		= 5;

		goblin[count].stats.carry_weight		= 65.00;	//kilograms

		goblin[count].stats.strength			= 2;
		goblin[count].stats.strength_max		= 100;
		goblin[count].stats.health				= 2;
		goblin[count].stats.health_max			= 100;
		goblin[count].stats.perception			= 1;
		goblin[count].stats.perception_max		= 100;
		goblin[count].stats.speed				= 1;
		goblin[count].stats.speed_max			= 100;
		goblin[count].stats.willpower			= 1;
		goblin[count].stats.willpower_max		= 100;
		goblin[count].stats.dexterity			= 1;
		goblin[count].stats.dexterity_max		= 100;
		goblin[count].stats.intelligence		= 1;
		goblin[count].stats.intelligence_max	= 100;
		goblin[count].stats.luck				= 1;
		goblin[count].stats.luck_max			= 100;

		count++;
		break;
	case GoblinArcher:							//Goblin Archer

		goblin[count].ent->model = gf3d_model_load("goblinarcher");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name				= "Goblin Archer";
		goblin[count].stats.level				= 1;
		goblin[count].stats.exp					= 0;
		goblin[count].stats.bits				= 50;		//currency

		goblin[count].stats.life				= 50;
		goblin[count].stats.life_max			= 50;
		goblin[count].stats.life_regen			= 5;
		goblin[count].stats.mana				= 25;
		goblin[count].stats.mana_max			= 25;
		goblin[count].stats.mana_regen			= 5;
		goblin[count].stats.stamina				= 100;
		goblin[count].stats.stamina_max			= 100;
		goblin[count].stats.stamina_regen		= 10;

		goblin[count].stats.carry_weight		= 65.00;	//kilograms

		goblin[count].stats.strength			= 1;
		goblin[count].stats.strength_max		= 100;
		goblin[count].stats.health				= 1;
		goblin[count].stats.health_max			= 100;
		goblin[count].stats.perception			= 2;
		goblin[count].stats.perception_max		= 100;
		goblin[count].stats.speed				= 2;
		goblin[count].stats.speed_max			= 100;
		goblin[count].stats.willpower			= 1;
		goblin[count].stats.willpower_max		= 100;
		goblin[count].stats.dexterity			= 2;
		goblin[count].stats.dexterity_max		= 100;
		goblin[count].stats.intelligence		= 1;
		goblin[count].stats.intelligence_max	= 100;
		goblin[count].stats.luck				= 1;
		goblin[count].stats.luck_max			= 100;

		count++;
		break;

	case GoblinKing:							//Goblin King

		goblin[count].ent->model = gf3d_model_load("goblinking");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name				= "Goblin King";
		goblin[count].stats.level				= 5;
		goblin[count].stats.exp					= 0;
		goblin[count].stats.bits				= 2000;		//currency

		goblin[count].stats.life				= 200;
		goblin[count].stats.life_max			= 200;
		goblin[count].stats.life_regen			= 15;
		goblin[count].stats.mana				= 100;
		goblin[count].stats.mana_max			= 100;
		goblin[count].stats.mana_regen			= 15;
		goblin[count].stats.stamina				= 200;
		goblin[count].stats.stamina_max			= 200;
		goblin[count].stats.stamina_regen		= 15;

		goblin[count].stats.carry_weight		= 100.00;	//kilograms

		goblin[count].stats.strength			= 10;
		goblin[count].stats.strength_max		= 100;
		goblin[count].stats.health				= 5;
		goblin[count].stats.health_max			= 100;
		goblin[count].stats.perception			= 2;
		goblin[count].stats.perception_max		= 100;
		goblin[count].stats.speed				= 5;
		goblin[count].stats.speed_max			= 100;
		goblin[count].stats.willpower			= 10;
		goblin[count].stats.willpower_max		= 100;
		goblin[count].stats.dexterity			= 2;
		goblin[count].stats.dexterity_max		= 100;
		goblin[count].stats.intelligence		= 1;
		goblin[count].stats.intelligence_max	= 100;
		goblin[count].stats.luck				= 5;
		goblin[count].stats.luck_max			= 100;

		goblin[count].ent->boxCollider.width	= 8.0;
		goblin[count].ent->boxCollider.height	= 30.0;
		goblin[count].ent->boxCollider.depth	= 8.0;

		count++;
		break;
	}
}

Entity *rpg_goblin_new(){

	return gf3d_entity_new();
}

void rpg_goblin_think(Entity *self){

	rpg_goblin_move(self);

}

void rpg_goblin_move(Entity *self){

	self->velocity = vector3d(gfc_crandom()*0.8, 0, gfc_crandom()*0.8);
	vector3d_add(self->position,self->position, self->velocity);
	gfc_matrix_new_translation(self->modelMatrix,self->position);
	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

}

void rpg_goblin_search(Entity *player)
{
	//TODO : have goblins search for the player to attack.
}
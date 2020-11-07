
#include "simple_logger.h"

#include "rpg_npc.h"

static Entity *npc = NULL;

void rpg_npc_move(Entity *self);

void rpg_npc_init(){
	npc = gf3d_entity_new();

	npc->position = vector3d(0, 0, 0);
	npc->velocity = vector3d(0, 0, 0);
	npc->rotation = vector3d(0,0,0);

	npc->boxCollider.depth = 1.0;
	npc->boxCollider.height = 1.0;
	npc->boxCollider.width = 1.0;
	npc->boxCollider.x = npc->position.x;
	npc->boxCollider.y = npc->position.y;
	npc->boxCollider.z = npc->position.z;

	npcStats stats;
	npcInventory inventory;

	stats.name = "npc";
	stats.level = 1;
	stats.exp = 0;

	stats.life = 100;
	stats.life_max = 100;
	stats.life_regen = 100;
	stats.magic = 100;
	stats.magic_max = 100;
	stats.magic_regen = 100;
	stats.stamina = 100;
	stats.stamina_max = 100;
	stats.stamina_regen = 100;

	stats.carry_weight = 65.00;	//Value is in kilograms

	stats.strength = 1;
	stats.strength_max = 100;
	stats.health = 1;
	stats.health_max = 100;
	stats.perception = 1;
	stats.perception_max = 100;
	stats.speed = 1;
	stats.speed_max = 100;
	stats.willpower = 1;
	stats.willpower_max = 100;
	stats.dexterity = 1;
	stats.dexterity_max = 100;
	stats.intelligence = 1;
	stats.intelligence_max = 100;
	stats.luck = 1;
	stats.luck_max = 100;

}

Entity *rpg_npc_new(){

	return gf3d_entity_new();
}

void rpg_npc_think(Entity *self){

	rpg_npc_move(self);

	gf3d_entity_collision_test(self);
}

void rpg_npc_move(Entity *self){

	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

}
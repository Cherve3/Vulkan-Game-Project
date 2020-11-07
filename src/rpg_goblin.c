#include "simple_logger.h"

#include "rpg_goblin.h"
#include "rpg_npc.h"

static NPC *goblin = NULL;
static int count = 0;

void rpg_goblin_move(Entity *self);

void rpg_goblin_init(int type, Vector3D position){
	
	if (!goblin)
		goblin = (NPC *)gfc_allocate_array(sizeof(NPC), 10);
	//while(!goblin[count].ent)
	//	count++;
	slog("Count: %i", count);

	goblin[count].ent = rpg_goblin_new();

	goblin[count].ent->name = "Goblin ";
	
	goblin[count].ent->position = position;
	goblin[count].ent->velocity = vector3d(0, 0, 0);
	goblin[count].ent->rotation = vector3d(0, 0, 0);

	goblin[count].ent->boxCollider.depth = 2.0;
	goblin[count].ent->boxCollider.height = 2.0;
	goblin[count].ent->boxCollider.width = 2.0;
	goblin[count].ent->boxCollider.x = goblin[count].ent->position.x;
	goblin[count].ent->boxCollider.y = goblin[count].ent->position.y;
	goblin[count].ent->boxCollider.z = goblin[count].ent->position.z;

	gfc_matrix_new_translation(goblin[count].ent->modelMatrix, goblin[count].ent->position);

	switch (type)
	{
	case 0:

		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name = "Goblin Grunt";
		goblin[count].stats.level = 1;
		goblin[count].stats.exp = 0;
		
		goblin[count].stats.life = 25;
		goblin[count].stats.life_max = 25;
		goblin[count].stats.life_regen = 5;
		goblin[count].stats.magic = 25;
		goblin[count].stats.magic_max = 25;
		goblin[count].stats.magic_regen = 5;
		goblin[count].stats.stamina = 50;
		goblin[count].stats.stamina_max = 50;
		goblin[count].stats.stamina_regen = 10;

		goblin[count].stats.carry_weight = 45.00;	//Value is in kilograms

		goblin[count].stats.strength = 1;
		goblin[count].stats.strength_max = 100;
		goblin[count].stats.health = 1;
		goblin[count].stats.health_max = 100;
		goblin[count].stats.perception = 1;
		goblin[count].stats.perception_max = 100;
		goblin[count].stats.speed = 2;
		goblin[count].stats.speed_max = 100;
		goblin[count].stats.willpower = 1;
		goblin[count].stats.willpower_max = 100;
		goblin[count].stats.dexterity = 1;
		goblin[count].stats.dexterity_max = 100;
		goblin[count].stats.intelligence = 1;
		goblin[count].stats.intelligence_max = 100;
		goblin[count].stats.luck = 1;
		goblin[count].stats.luck_max = 100;

		count++;
		break;
	case 1:							//Goblin Heavy

		goblin[count].ent->model = gf3d_model_load("goblinheavy");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name = "Goblin Heavy";
		goblin[count].stats.level = 1;
		goblin[count].stats.exp = 0;

		goblin[count].stats.life = 100;
		goblin[count].stats.life_max = 100;
		goblin[count].stats.life_regen = 10;
		goblin[count].stats.magic = 25;
		goblin[count].stats.magic_max = 25;
		goblin[count].stats.magic_regen = 10;
		goblin[count].stats.stamina = 50;
		goblin[count].stats.stamina_max = 50;
		goblin[count].stats.stamina_regen = 10;

		goblin[count].stats.carry_weight = 65.00;	//Value is in kilograms

		goblin[count].stats.strength = 2;
		goblin[count].stats.strength_max = 100;
		goblin[count].stats.health = 2;
		goblin[count].stats.health_max = 100;
		goblin[count].stats.perception = 1;
		goblin[count].stats.perception_max = 100;
		goblin[count].stats.speed = 1;
		goblin[count].stats.speed_max = 100;
		goblin[count].stats.willpower = 1;
		goblin[count].stats.willpower_max = 100;
		goblin[count].stats.dexterity = 1;
		goblin[count].stats.dexterity_max = 100;
		goblin[count].stats.intelligence = 1;
		goblin[count].stats.intelligence_max = 100;
		goblin[count].stats.luck = 1;
		goblin[count].stats.luck_max = 100;

		count++;
		break;
	case 2:							//Goblin Archer

		goblin[count].ent->model = gf3d_model_load("goblinarcher");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name = "Goblin Archer";
		goblin[count].stats.level = 1;
		goblin[count].stats.exp = 0;

		goblin[count].stats.life = 50;
		goblin[count].stats.life_max = 50;
		goblin[count].stats.life_regen = 10;
		goblin[count].stats.magic = 25;
		goblin[count].stats.magic_max = 25;
		goblin[count].stats.magic_regen = 10;
		goblin[count].stats.stamina = 100;
		goblin[count].stats.stamina_max = 100;
		goblin[count].stats.stamina_regen = 10;

		goblin[count].stats.carry_weight = 65.00;	//Value is in kilograms

		goblin[count].stats.strength = 1;
		goblin[count].stats.strength_max = 100;
		goblin[count].stats.health = 1;
		goblin[count].stats.health_max = 100;
		goblin[count].stats.perception = 2;
		goblin[count].stats.perception_max = 100;
		goblin[count].stats.speed = 2;
		goblin[count].stats.speed_max = 100;
		goblin[count].stats.willpower = 1;
		goblin[count].stats.willpower_max = 100;
		goblin[count].stats.dexterity = 2;
		goblin[count].stats.dexterity_max = 100;
		goblin[count].stats.intelligence = 1;
		goblin[count].stats.intelligence_max = 100;
		goblin[count].stats.luck = 1;
		goblin[count].stats.luck_max = 100;

		count++;
		break;
	}
	
}

Entity *rpg_goblin_new(){

	return gf3d_entity_new();
}

void rpg_goblin_think(Entity *self){

	rpg_goblin_move(self);

	gf3d_entity_collision_test(self);
}

void rpg_goblin_move(Entity *self){

	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

}
#include "simple_logger.h"

#include "simple_json.h"

#include "rpg_goblin.h"
#include "rpg_npc.h"

static NPC *goblin = NULL;
static int count = 0;

void rpg_goblin_move(Entity *self);
void print_goblin_stats(NPC goblin);

void rpg_goblin_init(int type, Vector3D position){
	
	// Load goblin json file
	SJson *goblin_info = sj_load("json/goblin.json");
	
	if (!goblin)
		goblin = (NPC *)gfc_allocate_array(sizeof(NPC), 10);

	if (!goblin_info)
	{
		slog("Failed to load json data %s", sj_get_error());
		return;
	}
	
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

//	gfc_matrix_new_translation(goblin[count].ent->modelMatrix, goblin[count].ent->position);

	switch (type)
	{
	case GoblinGrunt:

		goblin_info = sj_object_get_value(goblin_info, "GoblinGrunt");

		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;
		count++;
		break;
	case GoblinHeavy:							//Goblin Heavy

		goblin_info = sj_object_get_value(goblin_info, "GoblinHeavy");

		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].ent->boxCollider.width = 2.0;
		goblin[count].ent->boxCollider.height = 5.0;
		goblin[count].ent->boxCollider.depth = 2.0;

		gfc_matrix_slog(goblin[count].ent->modelMatrix);
		goblin[count].ent->modelMatrix[0][0] = 2;
		goblin[count].ent->modelMatrix[1][1] = 2;
		goblin[count].ent->modelMatrix[2][2] = 2;
		gfc_matrix_slog(goblin[count].ent->modelMatrix);
		count++;
		break;

	case GoblinArcher:							//Goblin Archer

		goblin_info = sj_object_get_value(goblin_info, "GoblinArcher");

		goblin[count].ent->model = gf3d_model_load("goblinarcher");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name = sj_get_string_value(sj_object_get_value(goblin_info, "name"));
		count++;
		break;

	case GoblinKing:							//Goblin King

		goblin_info = sj_object_get_value(goblin_info, "GoblinKing");

		goblin[count].ent->name = "Goblin King";

		goblin[count].ent->model = gf3d_model_load("goblinking");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].ent->boxCollider.width	= 8.0;
		goblin[count].ent->boxCollider.height	= 30.0;
		goblin[count].ent->boxCollider.depth	= 8.0;

		count++;
		break;
	}

	goblin[count].stats.name = sj_get_string_value(sj_object_get_value(goblin_info, "name"));

	sj_get_integer_value(sj_object_get_value(goblin_info, "level"), &goblin[count].stats.level);
	sj_get_integer_value(sj_object_get_value(goblin_info, "exp"), &goblin[count].stats.exp);
	sj_get_integer_value(sj_object_get_value(goblin_info, "bits"), &goblin[count].stats.bits);

	sj_get_integer_value(sj_object_get_value(goblin_info, "life"), &goblin[count].stats.life);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_life"), &goblin[count].stats.life_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "regen_life"), &goblin[count].stats.life_regen);
	sj_get_integer_value(sj_object_get_value(goblin_info, "mana"), &goblin[count].stats.mana);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_mana"), &goblin[count].stats.mana_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "regen_mana"), &goblin[count].stats.mana_regen);
	sj_get_integer_value(sj_object_get_value(goblin_info, "stamina"), &goblin[count].stats.stamina);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_stamina"), &goblin[count].stats.stamina_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "regen_stamina"), &goblin[count].stats.stamina_regen);

	sj_get_integer_value(sj_object_get_value(goblin_info, "strength"), &goblin[count].stats.strength);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_strength"), &goblin[count].stats.strength_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "health"), &goblin[count].stats.health);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_health"), &goblin[count].stats.health_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "perception"), &goblin[count].stats.perception);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_perception"), &goblin[count].stats.perception_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "speed"), &goblin[count].stats.speed);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_speed"), &goblin[count].stats.speed_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "willpower"), &goblin[count].stats.willpower);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_willpower"), &goblin[count].stats.willpower_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "dexterity"), &goblin[count].stats.dexterity);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_dexterity"), &goblin[count].stats.dexterity_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "intelligence"), &goblin[count].stats.intelligence);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_intelligence"), &goblin[count].stats.intelligence_max);
	sj_get_integer_value(sj_object_get_value(goblin_info, "luck"), &goblin[count].stats.luck);
	sj_get_integer_value(sj_object_get_value(goblin_info, "max_luck"), &goblin[count].stats.luck_max);

	print_goblin_stats(goblin[count]);

	sj_free(goblin_info);
}

Entity *rpg_goblin_new(){

	return gf3d_entity_new();
}

void rpg_goblin_think(Entity *self){


	rpg_goblin_move(self);
	//if (strcmp(self->name, "Goblin King") == 0)
		//self->model = gf3d_model_load_animated("goblinking", 1, 5);

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

void print_goblin_stats(NPC goblin)
{
	slog("            Name: %s", goblin.stats.name);
	slog("           Level: %i", goblin.stats.level);
	slog("             Exp: %i", goblin.stats.exp);
	slog("            Bits: %i", goblin.stats.bits);

	slog("            Life: %i", goblin.stats.life);
	slog("        Life Max: %i", goblin.stats.life_max);
	slog("      Life Regen: %i", goblin.stats.life_regen);
	slog("            Mana: %i", goblin.stats.mana);
	slog("        Mana Max: %i", goblin.stats.mana_max);
	slog("      Mana Regen: %i", goblin.stats.mana_regen);
	slog("         Stamina: %i", goblin.stats.stamina);
	slog("     Stamina Max: %i", goblin.stats.stamina_max);
	slog("   Stamina Regen: %i", goblin.stats.stamina_regen);

	slog("        Strength: %i", goblin.stats.strength);
	slog("    Strength Max: %i", goblin.stats.strength_max);
	slog("          Health: %i", goblin.stats.health);
	slog("      Health Max: %i", goblin.stats.health_max);
	slog("      Perception: %i", goblin.stats.perception);
	slog("  Perception Max: %i", goblin.stats.perception_max);
	slog("           Speed: %i", goblin.stats.speed);
	slog("       Speed Max: %i", goblin.stats.speed_max);
	slog("       Willpower: %i", goblin.stats.willpower);
	slog("   Willpower Max: %i", goblin.stats.willpower_max);
	slog("       Dexterity: %i", goblin.stats.dexterity);
	slog("   Dexterity Max: %i", goblin.stats.dexterity_max);
	slog("    Intelligence: %i", goblin.stats.intelligence);
	slog("Intelligence Max: %i", goblin.stats.intelligence_max);
	slog("            Luck: %i", goblin.stats.luck);
	slog("        Luck Max: %i", goblin.stats.luck_max);
}
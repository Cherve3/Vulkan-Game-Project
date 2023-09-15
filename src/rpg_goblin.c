#include "simple_logger.h"

#include "game.h"
#include "simple_json.h"

#include "rpg_goblin.h"
#include "rpg_npc.h"
#include "rpg_projectile.h"

char file_path[60];

static NPC *goblin = NULL;
static int count = 0;

static SJson  *goblin_info = NULL;
static SJson  *grunt_info = NULL;
static SJson  *heavy_info = NULL;
static SJson  *archer_info = NULL;
static SJson  *king_info = NULL;

void rpg_goblin_move(Entity *self);
void print_goblin_stats(NPC goblin);
void set_goblin_stats(SJson *info);

void goblin_close()
{
	sj_free(grunt_info);
	sj_free(heavy_info);
	sj_free(archer_info);
	sj_free(king_info);
	sj_free(goblin_info);
	memset(goblin,0,sizeof(NPC)*10);
	slog("Goblin files closing");
}

void rpg_goblin_init(){

	goblin = (NPC *)gfc_allocate_array(sizeof(NPC), 10);

	// Load goblin json file
	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/goblin.json");
	goblin_info = sj_load(file_path);
	grunt_info = sj_object_get_value(goblin_info, "goblingrunt");
	heavy_info = sj_object_get_value(goblin_info, "goblinheavy");
	archer_info = sj_object_get_value(goblin_info, "goblinarcher");
	king_info = sj_object_get_value(goblin_info, "goblinking");

	if (!goblin_info || !grunt_info || !heavy_info || !archer_info || !king_info)
	{
		slog("Failed to load goblin json data %s", sj_get_error());
		return;
	}

	atexit(goblin_close);
}

void rpg_goblin_spawn(GoblinType type, Vector3D position)
{

	slog("Goblin Count: %i", count);

	goblin[count].ent						= rpg_goblin_new();

	char name[10];
	char int_buffer[3];
	strcpy(name, "Goblin ");
	_itoa(count, int_buffer, 10);
	strcat(name, int_buffer);
	slog("Name: %s", name);

	goblin[count].ent->name					= name;
	goblin[count].ent->type					= MONSTER;

	goblin[count].ent->position				= position;
	goblin[count].ent->velocity				= vector3d(0, 0, 0);
	goblin[count].ent->rotation				= vector3d(0, 0, 0);

	goblin[count].ent->boxCollider.width	= 2.0;
	goblin[count].ent->boxCollider.height	= 10.0;
	goblin[count].ent->boxCollider.depth	= 2.0;
	goblin[count].ent->boxCollider.x		= goblin[count].ent->position.x;
	goblin[count].ent->boxCollider.y		= goblin[count].ent->position.y;
	goblin[count].ent->boxCollider.z		= goblin[count].ent->position.z;

	switch (type)
	{
	case GoblinGrunt:

		goblin_info = sj_object_get_value(goblin_info, "goblingrunt");

		goblin[count].ent->name = "Goblin Grunt";
		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;

		set_goblin_stats(grunt_info);

		count++;
		break;
	case GoblinHeavy:

		goblin_info = sj_object_get_value(heavy_info, "goblinheavy");

		goblin[count].ent->name = "Goblin Heavy";
		goblin[count].ent->model = gf3d_model_load("goblingrunt");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].ent->boxCollider.width = 2.0;
		goblin[count].ent->boxCollider.height = 6.0;
		goblin[count].ent->boxCollider.depth = 2.0;

		goblin[count].ent->modelMatrix[0][0] = 2;
		goblin[count].ent->modelMatrix[1][1] = 2;
		goblin[count].ent->modelMatrix[2][2] = 2;

		set_goblin_stats(heavy_info);

		count++;
		break;

	case GoblinArcher:

		goblin_info = sj_object_get_value(archer_info, "goblinarcher");

		goblin[count].ent->name = "Goblin Archer";
		goblin[count].ent->model = gf3d_model_load("goblinarcher");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].stats.name = sj_get_string_value(sj_object_get_value(goblin_info, "name"));

		set_goblin_stats(archer_info);

		count++;
		break;

	case GoblinKing:

		goblin_info = sj_object_get_value(goblin_info, "goblinking");

		goblin[count].ent->name = "Goblin King";

		goblin[count].ent->model = gf3d_model_load("goblinking");
		goblin[count].ent->think = rpg_goblin_think;

		goblin[count].ent->boxCollider.width	= 8.0;
		goblin[count].ent->boxCollider.height	= 28.0;
		goblin[count].ent->boxCollider.depth	= 8.0;

		set_goblin_stats(king_info);
		goblin[count].ent->data = (void*)&goblin[count];

		count++;
		break;
	}


}

Entity *rpg_goblin_new(){

	return gf3d_entity_new();
}

void rpg_goblin_touch(Entity *self, Entity *other)
{
	Projectile *pro = NULL;
	NPC *goblin = NULL;

	if (!self || !other) return;
	goblin = self->data;
	slog("Goblin hit");
	if (other->type == PROJECTILE)
	{
		pro = (Projectile*)other->data;
		slog("Goblin health is %i", goblin->stats.health);
		goblin->stats.health -= pro->damage;
		slog("Goblin health is %i", goblin->stats.health);
	}
	if (goblin->stats.health <= 0)
		self->_inuse = 0;
	
}

void rpg_goblin_think(Entity *self){

	rpg_goblin_move(self);
	gf3d_entity_collision_test(self);
	//if (strcmp(self->name, "Goblin King") == 0)
		//self->model = gf3d_model_load_animated("goblinking", 1, 5);

}

void rpg_goblin_move(Entity *self){

	self->velocity = vector3d(gfc_crandom()*0.8, 0, gfc_crandom()*0.8);
	vector3d_add(self->position,self->position, self->velocity);
	gfc_matrix_translate(self->modelMatrix, self->position);
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

void set_goblin_stats(SJson *info)
{
	goblin[count].stats.name = sj_get_string_value(sj_object_get_value(info, "name"));

	sj_get_integer_value(sj_object_get_value(info, "level"), &goblin[count].stats.level);
	sj_get_integer_value(sj_object_get_value(info, "exp"), &goblin[count].stats.exp);
	sj_get_integer_value(sj_object_get_value(info, "bits"), &goblin[count].stats.bits);

	sj_get_integer_value(sj_object_get_value(info, "life"), &goblin[count].stats.life);
	sj_get_integer_value(sj_object_get_value(info, "max_life"), &goblin[count].stats.life_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_life"), &goblin[count].stats.life_regen);
	sj_get_integer_value(sj_object_get_value(info, "mana"), &goblin[count].stats.mana);
	sj_get_integer_value(sj_object_get_value(info, "max_mana"), &goblin[count].stats.mana_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_mana"), &goblin[count].stats.mana_regen);
	sj_get_integer_value(sj_object_get_value(info, "stamina"), &goblin[count].stats.stamina);
	sj_get_integer_value(sj_object_get_value(info, "max_stamina"), &goblin[count].stats.stamina_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_stamina"), &goblin[count].stats.stamina_regen);

	sj_get_integer_value(sj_object_get_value(info, "strength"), &goblin[count].stats.strength);
	sj_get_integer_value(sj_object_get_value(info, "max_strength"), &goblin[count].stats.strength_max);
	sj_get_integer_value(sj_object_get_value(info, "health"), &goblin[count].stats.health);
	sj_get_integer_value(sj_object_get_value(info, "max_health"), &goblin[count].stats.health_max);
	sj_get_integer_value(sj_object_get_value(info, "perception"), &goblin[count].stats.perception);
	sj_get_integer_value(sj_object_get_value(info, "max_perception"), &goblin[count].stats.perception_max);
	sj_get_integer_value(sj_object_get_value(info, "speed"), &goblin[count].stats.speed);
	sj_get_integer_value(sj_object_get_value(info, "max_speed"), &goblin[count].stats.speed_max);
	sj_get_integer_value(sj_object_get_value(info, "willpower"), &goblin[count].stats.willpower);
	sj_get_integer_value(sj_object_get_value(info, "max_willpower"), &goblin[count].stats.willpower_max);
	sj_get_integer_value(sj_object_get_value(info, "dexterity"), &goblin[count].stats.dexterity);
	sj_get_integer_value(sj_object_get_value(info, "max_dexterity"), &goblin[count].stats.dexterity_max);
	sj_get_integer_value(sj_object_get_value(info, "intelligence"), &goblin[count].stats.intelligence);
	sj_get_integer_value(sj_object_get_value(info, "max_intelligence"), &goblin[count].stats.intelligence_max);
	sj_get_integer_value(sj_object_get_value(info, "luck"), &goblin[count].stats.luck);
	sj_get_integer_value(sj_object_get_value(info, "max_luck"), &goblin[count].stats.luck_max);
}

NPC get_goblin(int index)
{
	return goblin[index];
}


/*eof@eol*/

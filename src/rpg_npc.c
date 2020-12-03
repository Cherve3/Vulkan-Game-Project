
#include "simple_logger.h"

#include "rpg_npc.h"

static NPC *npc = NULL;
static int count = 0;

void rpg_npc_move(Entity *self);

void rpg_npc_init(int type, Vector3D position){

	if (!npc)
		npc = (NPC *)gfc_allocate_array(sizeof(NPC), 10);

	slog("NPC Count: %i", count);

	npc[count].ent = rpg_npc_new();

	npc[count].ent->name = "NPC";
	npc[count].ent->type = NONPLAYER;

	npc[count].ent->position = position;
	npc[count].ent->velocity = vector3d(0, 0, 0);
	npc[count].ent->rotation = vector3d(0,0,0);

	npc[count].ent->boxCollider.width = 1.0;
	npc[count].ent->boxCollider.height = 6.0;
	npc[count].ent->boxCollider.depth = 1.0;
	npc[count].ent->boxCollider.x = npc[count].ent->position.x;
	npc[count].ent->boxCollider.y = npc[count].ent->position.y;
	npc[count].ent->boxCollider.z = npc[count].ent->position.z;

	gfc_matrix_new_translation(npc[count].ent->modelMatrix, npc[count].ent->position);

	switch (type)
	{
	case ItemShop:

		npc[count].ent->model = gf3d_model_load("itemshop");
		npc[count].ent->think = rpg_npc_think;

		npc[count].stats.name = "Item Shop";
		npc[count].stats.level = 1;
		npc[count].stats.exp = 0;

		npc[count].stats.life = 100;
		npc[count].stats.life_max = 100;
		npc[count].stats.life_regen = 100;
		npc[count].stats.mana = 100;
		npc[count].stats.mana_max = 100;
		npc[count].stats.mana_regen = 100;
		npc[count].stats.stamina = 100;
		npc[count].stats.stamina_max = 100;
		npc[count].stats.stamina_regen = 100;

		npc[count].stats.carry_weight = 65.00;	//Value is in kilograms

		npc[count].stats.strength = 1;
		npc[count].stats.strength_max = 100;
		npc[count].stats.health = 1;
		npc[count].stats.health_max = 100;
		npc[count].stats.perception = 1;
		npc[count].stats.perception_max = 100;
		npc[count].stats.speed = 1;
		npc[count].stats.speed_max = 100;
		npc[count].stats.willpower = 1;
		npc[count].stats.willpower_max = 100;
		npc[count].stats.dexterity = 1;
		npc[count].stats.dexterity_max = 100;
		npc[count].stats.intelligence = 1;
		npc[count].stats.intelligence_max = 100;
		npc[count].stats.luck = 1;
		npc[count].stats.luck_max = 100;

		count++;
		break;
	case WeaponShop:

		npc[count].ent->model = gf3d_model_load("weaponshop");
		npc[count].ent->think = rpg_npc_think;

		npc[count].stats.name = "Weapon Shop";
		npc[count].stats.level = 1;
		npc[count].stats.exp = 0;

		npc[count].stats.life = 100;
		npc[count].stats.life_max = 100;
		npc[count].stats.life_regen = 100;
		npc[count].stats.mana = 100;
		npc[count].stats.mana_max = 100;
		npc[count].stats.mana_regen = 100;
		npc[count].stats.stamina = 100;
		npc[count].stats.stamina_max = 100;
		npc[count].stats.stamina_regen = 100;

		npc[count].stats.carry_weight = 65.00;	//Value is in kilograms

		npc[count].stats.strength = 1;
		npc[count].stats.strength_max = 100;
		npc[count].stats.health = 1;
		npc[count].stats.health_max = 100;
		npc[count].stats.perception = 1;
		npc[count].stats.perception_max = 100;
		npc[count].stats.speed = 1;
		npc[count].stats.speed_max = 100;
		npc[count].stats.willpower = 1;
		npc[count].stats.willpower_max = 100;
		npc[count].stats.dexterity = 1;
		npc[count].stats.dexterity_max = 100;
		npc[count].stats.intelligence = 1;
		npc[count].stats.intelligence_max = 100;
		npc[count].stats.luck = 1;
		npc[count].stats.luck_max = 100;

		count++;
		break;
	case SpellShop:

		npc[count].ent->model = gf3d_model_load("spellshop");
		npc[count].ent->think = rpg_npc_think;

		npc[count].stats.name = "Spell Shop";
		npc[count].stats.level = 1;
		npc[count].stats.exp = 0;

		npc[count].stats.life = 100;
		npc[count].stats.life_max = 100;
		npc[count].stats.life_regen = 100;
		npc[count].stats.mana = 100;
		npc[count].stats.mana_max = 100;
		npc[count].stats.mana_regen = 100;
		npc[count].stats.stamina = 100;
		npc[count].stats.stamina_max = 100;
		npc[count].stats.stamina_regen = 100;

		npc[count].stats.carry_weight = 65.00;	//Value is in kilograms

		npc[count].stats.strength = 1;
		npc[count].stats.strength_max = 100;
		npc[count].stats.health = 1;
		npc[count].stats.health_max = 100;
		npc[count].stats.perception = 1;
		npc[count].stats.perception_max = 100;
		npc[count].stats.speed = 1;
		npc[count].stats.speed_max = 100;
		npc[count].stats.willpower = 1;
		npc[count].stats.willpower_max = 100;
		npc[count].stats.dexterity = 1;
		npc[count].stats.dexterity_max = 100;
		npc[count].stats.intelligence = 1;
		npc[count].stats.intelligence_max = 100;
		npc[count].stats.luck = 1;
		npc[count].stats.luck_max = 100;

		count++;
		break;
	case General:

		npc[count].ent->model = gf3d_model_load("npc");
		npc[count].ent->think = rpg_npc_think;

		npc[count].stats.name = "John Doe";
		npc[count].stats.level = 1;
		npc[count].stats.exp = 0;

		npc[count].stats.life = 100;
		npc[count].stats.life_max = 100;
		npc[count].stats.life_regen = 100;
		npc[count].stats.mana = 100;
		npc[count].stats.mana_max = 100;
		npc[count].stats.mana_regen = 100;
		npc[count].stats.stamina = 100;
		npc[count].stats.stamina_max = 100;
		npc[count].stats.stamina_regen = 100;

		npc[count].stats.carry_weight = 65.00;	//Value is in kilograms

		npc[count].stats.strength = 1;
		npc[count].stats.strength_max = 100;
		npc[count].stats.health = 1;
		npc[count].stats.health_max = 100;
		npc[count].stats.perception = 1;
		npc[count].stats.perception_max = 100;
		npc[count].stats.speed = 1;
		npc[count].stats.speed_max = 100;
		npc[count].stats.willpower = 1;
		npc[count].stats.willpower_max = 100;
		npc[count].stats.dexterity = 1;
		npc[count].stats.dexterity_max = 100;
		npc[count].stats.intelligence = 1;
		npc[count].stats.intelligence_max = 100;
		npc[count].stats.luck = 1;
		npc[count].stats.luck_max = 100;

		count++;
		break;
	case Questgiver:

		npc[count].ent->model = gf3d_model_load("questnpc");
		npc[count].ent->think = rpg_npc_think;

		npc[count].stats.name = "Questgiver";
		npc[count].stats.level = 1;
		npc[count].stats.exp = 0;

		npc[count].stats.life = 100;
		npc[count].stats.life_max = 100;
		npc[count].stats.life_regen = 100;
		npc[count].stats.mana = 100;
		npc[count].stats.mana_max = 100;
		npc[count].stats.mana_regen = 100;
		npc[count].stats.stamina = 100;
		npc[count].stats.stamina_max = 100;
		npc[count].stats.stamina_regen = 100;

		npc[count].stats.carry_weight = 65.00;	//Value is in kilograms

		npc[count].stats.strength = 1;
		npc[count].stats.strength_max = 100;
		npc[count].stats.health = 1;
		npc[count].stats.health_max = 100;
		npc[count].stats.perception = 1;
		npc[count].stats.perception_max = 100;
		npc[count].stats.speed = 1;
		npc[count].stats.speed_max = 100;
		npc[count].stats.willpower = 1;
		npc[count].stats.willpower_max = 100;
		npc[count].stats.dexterity = 1;
		npc[count].stats.dexterity_max = 100;
		npc[count].stats.intelligence = 1;
		npc[count].stats.intelligence_max = 100;
		npc[count].stats.luck = 1;
		npc[count].stats.luck_max = 100;

		count++;
		break;

	}

}

Entity *rpg_npc_new(){

	return gf3d_entity_new();
}

void rpg_npc_think(Entity *self){

	rpg_npc_move(self);
}

void rpg_npc_move(Entity *self){

	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

}
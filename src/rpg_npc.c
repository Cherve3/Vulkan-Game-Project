

#include "game.h"
#include "simple_logger.h"
#include "simple_json.h"

#include "rpg_npc.h"
#include "rpg_quest.h"

char file_path[60];

static NPC *npc = NULL;
static int count = 0;

static SJson *npc_info			= NULL;
static SJson *itemshop_info		= NULL;
static SJson *weaponshop_info	= NULL;
static SJson *armorshop_info	= NULL;
static SJson *spellshop_info	= NULL;
static SJson *generic_info		= NULL;
static SJson *questgiver_info	= NULL;

Bool toggleItemShop		= false;
Bool toggleWeaponShop	= false;
Bool toggleArmorShop	= false;
Bool toggleSpellShop	= false;
Bool toggleGeneric		= false;
Bool toggleQuest		= false;

void rpg_npc_move(Entity *self);
void set_npc_stats(SJson *info);
void print_npc_stats(NPC npc);
void rpg_npc_interact(Entity *self);

void npc_close()
{
	sj_free(itemshop_info);
	sj_free(weaponshop_info);
	sj_free(armorshop_info);
	sj_free(spellshop_info);
	sj_free(generic_info);
	sj_free(questgiver_info);
	sj_free(npc_info);
	memset(npc, 0, sizeof(NPC)* 10);
	slog("NPC files closing");
}

void rpg_npc_init(){

	slog("Initializing NPC's");
	npc = gfc_allocate_array(sizeof(NPC), 10);

	// Load npc json file
	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/npc.json");
	npc_info		= sj_load(file_path);
	itemshop_info	= sj_object_get_value(npc_info, "itemshop");
	weaponshop_info = sj_object_get_value(npc_info, "weaponshop");
	armorshop_info	= sj_object_get_value(npc_info, "armorshop");
	spellshop_info	= sj_object_get_value(npc_info, "spellshop");
	generic_info	= sj_object_get_value(npc_info, "generic");
	questgiver_info = sj_object_get_value(npc_info, "questgiver");

	if (!itemshop_info || !weaponshop_info || !armorshop_info || !spellshop_info || !generic_info || !questgiver_info)
	{
		slog("Failed to load npc json data %s", sj_get_error());
		return;
	}
	atexit(npc_close);
	slog("NPC's Initialized");
}

void rpg_npc_spawn(NPCType type, Vector3D position)
{

	npc[count].ent = rpg_npc_new();

	char name[7];
	char int_buffer[3];
	strcpy(name, "NPC ");
	_itoa(count, int_buffer, 10);
	strcat(name, int_buffer);
	slog("Name: %s", name);

	npc[count].ent->name = name;
	npc[count].ent->type = NONPLAYER;

	npc[count].ent->position = position;
	npc[count].ent->velocity = vector3d(0, 0, 0);
	npc[count].ent->rotation = vector3d(0,0,0);

	npc[count].ent->interact = rpg_npc_interact;

	npc[count].ent->boxCollider.width = 1.0;
	npc[count].ent->boxCollider.height = 6.0;
	npc[count].ent->boxCollider.depth = 1.0;
	npc[count].ent->boxCollider.x = npc[count].ent->position.x;
	npc[count].ent->boxCollider.y = npc[count].ent->position.y;
	npc[count].ent->boxCollider.z = npc[count].ent->position.z;

	gfc_matrix_translate(npc[count].ent->modelMatrix, npc[count].ent->position);

	switch (type)
	{
	case ItemShop:

		npc[count].ent->name = "Item Shop";
		npc[count].ent->model = gf3d_model_load("itemshop");
		npc[count].ent->think = rpg_npc_think;

		set_npc_stats(itemshop_info);
		count++;
		break;
	case WeaponShop:

		npc[count].ent->name = "Weapon Shop";
		npc[count].ent->model = gf3d_model_load("weaponshop");
		npc[count].ent->think = rpg_npc_think;

		npc[count].ent->boxCollider.width = 2.0;
		npc[count].ent->boxCollider.height = 5.0;
		npc[count].ent->boxCollider.depth = 2.0;

		set_npc_stats(weaponshop_info);
		count++;
		break;
	case ArmorShop:

		npc[count].ent->name = "Armor Shop";
		npc[count].ent->model = gf3d_model_load("armorshop");
		npc[count].ent->think = rpg_npc_think;

		npc[count].ent->boxCollider.width = 2.0;
		npc[count].ent->boxCollider.height = 5.0;
		npc[count].ent->boxCollider.depth = 2.0;

		set_npc_stats(armorshop_info);
		count++;
		break;
	case SpellShop:

		npc[count].ent->name = "Spell Shop";
		npc[count].ent->model = gf3d_model_load("spellshop");
		npc[count].ent->think = rpg_npc_think;

		set_npc_stats(spellshop_info);
		count++;
		break;
	case Generic:

		npc[count].ent->name = "Generic";
		npc[count].ent->model = gf3d_model_load("generic1");
		npc[count].ent->think = rpg_npc_think;

		set_npc_stats(generic_info);
		count++;
		break;
	case Questgiver:

		npc[count].ent->name = "Questgiver";
		npc[count].ent->model = gf3d_model_load("questgiver");
		npc[count].ent->think = rpg_npc_think;

		set_npc_stats(questgiver_info);
		
		count++;
		break;
	}
	//npc[count].ent->data = (void*)npc[count];
//	print_npc_stats(npc[count]);
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

void rpg_npc_interact(Entity *self)
{

	slog("Interacting with %s", self->name);
	if (strcmp(self->name, "Item Shop") == 0 && toggleItemShop == false)
	{
		toggleItemShop = true;
	}
	else
		toggleItemShop = false;
	
	if (strcmp(self->name, "Weapon Shop") == 0 && toggleWeaponShop == false)
	{
		toggleWeaponShop = true;
	}
	else
		toggleWeaponShop = false;

	if (strcmp(self->name, "Armor Shop") == 0 && toggleArmorShop == false)
	{
		toggleArmorShop = true;
	}
	else
		toggleArmorShop = false;

	if (strcmp(self->name, "Spell Shop") == 0 && toggleSpellShop == false)
	{
		toggleSpellShop = true;
	}
	else
		toggleSpellShop = false;

	if (strcmp(self->name, "Generic") == 0 && toggleGeneric == false)
	{
		toggleGeneric = true;
	}
	else
		toggleGeneric = false;

	if (strcmp(self->name, "Questgiver") == 0 && toggleQuest == false)
	{
		toggleQuest = true;
		if (rpg_get_current_quest() == 0)
			rpg_start_quest(1);
	}
	else
		toggleQuest = false;
}

Bool rpg_get_itemshop_toggle()
{
	return toggleItemShop;
}

Bool rpg_get_weaponshop_toggle()
{
	return toggleWeaponShop;
}

Bool rpg_get_armorshop_toggle()
{
	return toggleArmorShop;
}

Bool rpg_get_spellshop_toggle()
{
	return toggleSpellShop;
}

Bool rpg_get_generic_toggle()
{
	return toggleGeneric;
}

Bool rpg_get_quest_toggle()
{
	return toggleQuest;
}

NPC *rpg_get_npc()
{
	return npc;
}

void set_npc_stats(SJson *info)
{
	npc[count].stats.name = sj_get_string_value(sj_object_get_value(info, "name"));

	sj_get_integer_value(sj_object_get_value(info, "level"), &npc[count].stats.level);
	sj_get_integer_value(sj_object_get_value(info, "exp"), &npc[count].stats.exp);
	sj_get_integer_value(sj_object_get_value(info, "bits"), &npc[count].stats.bits);

	sj_get_integer_value(sj_object_get_value(info, "life"), &npc[count].stats.life);
	sj_get_integer_value(sj_object_get_value(info, "max_life"), &npc[count].stats.life_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_life"), &npc[count].stats.life_regen);
	sj_get_integer_value(sj_object_get_value(info, "mana"), &npc[count].stats.mana);
	sj_get_integer_value(sj_object_get_value(info, "max_mana"), &npc[count].stats.mana_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_mana"), &npc[count].stats.mana_regen);
	sj_get_integer_value(sj_object_get_value(info, "stamina"), &npc[count].stats.stamina);
	sj_get_integer_value(sj_object_get_value(info, "max_stamina"), &npc[count].stats.stamina_max);
	sj_get_integer_value(sj_object_get_value(info, "regen_stamina"), &npc[count].stats.stamina_regen);

	sj_get_integer_value(sj_object_get_value(info, "strength"), &npc[count].stats.strength);
	sj_get_integer_value(sj_object_get_value(info, "max_strength"), &npc[count].stats.strength_max);
	sj_get_integer_value(sj_object_get_value(info, "health"), &npc[count].stats.health);
	sj_get_integer_value(sj_object_get_value(info, "max_health"), &npc[count].stats.health_max);
	sj_get_integer_value(sj_object_get_value(info, "perception"), &npc[count].stats.perception);
	sj_get_integer_value(sj_object_get_value(info, "max_perception"), &npc[count].stats.perception_max);
	sj_get_integer_value(sj_object_get_value(info, "speed"), &npc[count].stats.speed);
	sj_get_integer_value(sj_object_get_value(info, "max_speed"), &npc[count].stats.speed_max);
	sj_get_integer_value(sj_object_get_value(info, "willpower"), &npc[count].stats.willpower);
	sj_get_integer_value(sj_object_get_value(info, "max_willpower"), &npc[count].stats.willpower_max);
	sj_get_integer_value(sj_object_get_value(info, "dexterity"), &npc[count].stats.dexterity);
	sj_get_integer_value(sj_object_get_value(info, "max_dexterity"), &npc[count].stats.dexterity_max);
	sj_get_integer_value(sj_object_get_value(info, "intelligence"), &npc[count].stats.intelligence);
	sj_get_integer_value(sj_object_get_value(info, "max_intelligence"), &npc[count].stats.intelligence_max);
	sj_get_integer_value(sj_object_get_value(info, "luck"), &npc[count].stats.luck);
	sj_get_integer_value(sj_object_get_value(info, "max_luck"), &npc[count].stats.luck_max);

	//print_npc_stats(npc[count]);
}

void print_npc_stats(NPC npc)
{
	slog("            Name: %s", npc.stats.name);
	slog("           Level: %i", npc.stats.level);
	slog("             Exp: %i", npc.stats.exp);
	slog("            Bits: %i", npc.stats.bits);

	slog("            Life: %i", npc.stats.life);
	slog("        Life Max: %i", npc.stats.life_max);
	slog("      Life Regen: %i", npc.stats.life_regen);
	slog("            Mana: %i", npc.stats.mana);
	slog("        Mana Max: %i", npc.stats.mana_max);
	slog("      Mana Regen: %i", npc.stats.mana_regen);
	slog("         Stamina: %i", npc.stats.stamina);
	slog("     Stamina Max: %i", npc.stats.stamina_max);
	slog("   Stamina Regen: %i", npc.stats.stamina_regen);

	slog("        Strength: %i", npc.stats.strength);
	slog("    Strength Max: %i", npc.stats.strength_max);
	slog("          Health: %i", npc.stats.health);
	slog("      Health Max: %i", npc.stats.health_max);
	slog("      Perception: %i", npc.stats.perception);
	slog("  Perception Max: %i", npc.stats.perception_max);
	slog("           Speed: %i", npc.stats.speed);
	slog("       Speed Max: %i", npc.stats.speed_max);
	slog("       Willpower: %i", npc.stats.willpower);
	slog("   Willpower Max: %i", npc.stats.willpower_max);
	slog("       Dexterity: %i", npc.stats.dexterity);
	slog("   Dexterity Max: %i", npc.stats.dexterity_max);
	slog("    Intelligence: %i", npc.stats.intelligence);
	slog("Intelligence Max: %i", npc.stats.intelligence_max);
	slog("            Luck: %i", npc.stats.luck);
	slog("        Luck Max: %i", npc.stats.luck_max);
}

/*eol@eof*/

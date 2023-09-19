#include <math.h>

#include "simple_logger.h"
#include "simple_json.h"

#include "game.h"

#include "gf3d_camera.h"
#include "rpg_cooldowns.h"
#include "rpg_player.h"
#include "rpg_spellbook.h"
#include "rpg_items.h"
#include "rpg_projectile.h"
#include "rpg_levelup.h"
#include "rpg_ui.h";

char file_path[60];

static Player *player = { 0 };

static Uint32 timer;
static Uint32 old_time;

SJson *player_info = NULL;

time_t t;

Cooldown cd_interact;
Cooldown cd_jump;
Cooldown cd_primary_attack;
Cooldown cd_secondary_attack;
Cooldown cd_inventory;
Cooldown cd_stats;
Cooldown cd_map;
Cooldown cd_health_regen;
Cooldown cd_mana_regen;
Cooldown cd_stamina_regen;

float total_movement;
float old_x_pos;
float old_z_pos;

void rpg_player_move(Entity *self);
void rpg_player_update();
void rpg_player_take_damage(Entity* self, float damage, Entity* inflictor);
void rpg_player_interact(Entity* self, void* data);
void rpg_player_death(Entity* self);
void print_inventory();
void print_stats();

void rpg_player_init()
{
	player = (Player *)gfc_allocate_array(sizeof(Player), 1);

	player->ent = gf3d_entity_new();
	if (!player->ent)
	{
		slog("Player ent null");
	}
	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "models/player.model");
	player->ent->model = gf3d_model_load(file_path);
	if (!player->ent->model)
	{
		slog("Player model is NULL");
	}
	player->ent->animated = false;

	player->ent->think = rpg_player_think;
	player->ent->update = rpg_player_update;
	player->ent->damage = rpg_player_take_damage;
	player->ent->on_death = rpg_player_death;
	player->ent->interact = rpg_player_interact;
	player->ent->type = PLAYER;
	player->ent->name = "Player";
	player->ent->position = vector3d(0, 0, 0);
	player->ent->velocity = vector3d(0, 0, 0);
	
	player->ent->rotation = vector3d(-GFC_PI, 0, -GFC_HALF_PI);

	player->ent->boxCollider.width = 3.0;
	player->ent->boxCollider.height = 8.5;
	player->ent->boxCollider.depth = 3.0;
	player->ent->boxCollider.x = player->ent->position.x;
	player->ent->boxCollider.y = player->ent->position.y;
	player->ent->boxCollider.z = player->ent->position.z;

	player->interactBound.radius = 10;
	player->interactBound.x = player->ent->position.x;
	player->interactBound.y = player->ent->position.z;
	
	cd_interact.old_time = 0;
	cd_jump.old_time = 0;
	cd_primary_attack.old_time = 0;
	cd_secondary_attack.old_time = 0;
	cd_inventory.old_time = 0;
	cd_stats.old_time = 0;
	cd_map.old_time = 0;
	cd_health_regen.old_time = 0;
	cd_mana_regen.old_time = 0;
	cd_stamina_regen.old_time = 0;

	total_movement = 0;
	
	/***********************
	 * Player Stats
	 ***********************/

	player->max_speed = 1;
	player->speed = 0.2;
	vector3d_clear(player->acceleration);

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/player.json");
	player_info = sj_load(file_path);

	if (!player_info)
		slog("Failed to load player json data %s", sj_get_error());

	player_info = sj_object_get_value(player_info, "Player");
	player->stats.name =sj_get_string_value(sj_object_get_value(player_info, "name"));

	sj_get_integer_value(sj_object_get_value(player_info, "level"), &player->stats.level);
	sj_get_integer_value(sj_object_get_value(player_info, "exp"), &player->stats.exp);
	sj_get_integer_value(sj_object_get_value(player_info, "bits"), &player->stats.bits);

	sj_get_integer_value(sj_object_get_value(player_info, "life"), &player->stats.life);
	sj_get_integer_value(sj_object_get_value(player_info, "max_life"), &player->stats.life_max);
	sj_get_integer_value(sj_object_get_value(player_info, "regen_life"), &player->stats.life_regen);
	sj_get_integer_value(sj_object_get_value(player_info, "mana"), &player->stats.mana);
	sj_get_integer_value(sj_object_get_value(player_info, "max_mana"), &player->stats.mana_max);
	sj_get_integer_value(sj_object_get_value(player_info, "regen_mana"), &player->stats.mana_regen);
	sj_get_integer_value(sj_object_get_value(player_info, "stamina"), &player->stats.stamina);
	sj_get_integer_value(sj_object_get_value(player_info, "max_stamina"), &player->stats.stamina_max);
	sj_get_integer_value(sj_object_get_value(player_info, "regen_stamina"), &player->stats.stamina_regen);

	sj_get_float_value(sj_object_get_value(player_info, "carry_weight"), &player->stats.carry_weight);
	sj_get_float_value(sj_object_get_value(player_info, "max_weight"), &player->stats.carry_weight_max);

	sj_get_integer_value(sj_object_get_value(player_info, "strength"), &player->stats.strength);
	sj_get_integer_value(sj_object_get_value(player_info, "max_strength"), &player->stats.strength_max);
	sj_get_integer_value(sj_object_get_value(player_info, "health"), &player->stats.health);
	sj_get_integer_value(sj_object_get_value(player_info, "max_health"), &player->stats.health_max);
	sj_get_integer_value(sj_object_get_value(player_info, "perception"), &player->stats.perception);
	sj_get_integer_value(sj_object_get_value(player_info, "max_perception"), &player->stats.perception_max);
	sj_get_integer_value(sj_object_get_value(player_info, "speed"), &player->stats.speed);
	sj_get_integer_value(sj_object_get_value(player_info, "max_speed"), &player->stats.speed_max);
	sj_get_integer_value(sj_object_get_value(player_info, "willpower"), &player->stats.willpower);
	sj_get_integer_value(sj_object_get_value(player_info, "max_willpower"), &player->stats.willpower_max);
	sj_get_integer_value(sj_object_get_value(player_info, "dexterity"), &player->stats.dexterity);
	sj_get_integer_value(sj_object_get_value(player_info, "max_dexterity"), &player->stats.dexterity_max);
	sj_get_integer_value(sj_object_get_value(player_info, "intelligence"), &player->stats.intelligence);
	sj_get_integer_value(sj_object_get_value(player_info, "max_intelligence"), &player->stats.intelligence_max);
	sj_get_integer_value(sj_object_get_value(player_info, "luck"), &player->stats.luck);
	sj_get_integer_value(sj_object_get_value(player_info, "max_luck"), &player->stats.luck_max);

	player->stats.toggleStats = false;
	player->stats.toggleShop  = false;
	player->stats.toggleMap   = false;
	
	/***********************
	*	Player Inventory
	************************/

	player->inventory.bagSize = 30;
	player->inventory.spellbookSize = 5;
	player->inventory.bag = (Item *)gfc_allocate_array(sizeof(Item), player->inventory.bagSize);
	player->inventory.spellbook = (Spell *)gfc_allocate_array(sizeof(Spell), player->inventory.spellbookSize);

	srand((unsigned)time(&t));
}

void rpg_player_free(Player *player)
{
	if (!player) return;
	sj_free(player_info);
	rpg_spellbook_free(player->inventory.spellbook);
	rpg_player_bag_free(player->inventory.bag);
	rpg_player_inventory_free(&player->inventory);
	
	gf3d_entity_free(player->ent);
	memset(player, 0, sizeof(Player));
}

void rpg_player_inventory_free(PlayerInventory *inventory)
{
	if (!inventory) return;
	memset(inventory, 0, sizeof(PlayerInventory));
}

void rpg_player_bag_free(Item *bag)
{
	if (!bag) return;
	int i;

	for (i = 0; i < 30; i++){
		memset(bag, 0, sizeof(bag));
	}
	
}

void rpg_player_update(Entity *self)
{
	//gf3d_camera_set_target_entity(player->ent);
	gf3d_camera_set_rotation(self->rotation);
	gf3d_camera_set_velocity(self->velocity);
	
	if (vector3d_magnitude(self->velocity) > 0.001)
	{
		//self->velocity.y += GRAVITY;
		//vector3d_scale(self->velocity, self->velocity, 0.4);
		//self->position.x += self->velocity.x;

		if (player->state.onGround = true)
			self->position.y += self->velocity.y;

		total_movement += (float)(fabs(self->velocity.x) + fabs(self->velocity.z));
		if (total_movement * 0.1 >= 10)
		{
			total_movement = 0;
			stat_counter("rundistance");
		}
	}
	else
	{
		vector3d_clear(self->velocity);
	}
	
	self->position.x += self->velocity.x;
	self->position.y += self->velocity.y;
	self->position.z += self->velocity.z;

	gfc_matrix_zero(&self->velocity);

	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

	 
	slog("\nPosition: x:%f, y:%f, z:%f\n", self->position.x, self->position.y, self->position.z);
	slog("\nVelocity: x:%f, y:%f, z:%f\n", self->velocity.x, self->velocity.y, self->velocity.z);
	slog("\nAccelera: x:%f, y:%f, z:%f\n", player->acceleration.x, player->acceleration.y, player->acceleration.z);
	slog("\nRotation: x:%f, y:%f, z:%f\n", self->rotation.x, self->rotation.y, self->rotation.z);
}

void rpg_update_player_vitals()
{
	if (player->stats.health != player->stats.health_max)
	{
		if (timer > cd_health_regen.old_time + 5000)
		{
			cd_health_regen.old_time = timer;
			player->stats.life += player->stats.life_regen;
			if (player->stats.life > player->stats.life_max)
			{
				player->stats.life = player->stats.life_max;
			}
		}
	}

	if (player->stats.mana != player->stats.mana_max)
	{
		if (timer > cd_mana_regen.old_time + 5000)
		{
			cd_mana_regen.old_time = timer;
			player->stats.mana += player->stats.mana_regen;
			if (player->stats.mana > player->stats.mana_max)
			{
				player->stats.mana = player->stats.mana_max;
			}
		}
	}

	if (player->stats.stamina != player->stats.stamina_max)
	{
		if (timer > cd_stamina_regen.old_time + 5000)
		{
			cd_stamina_regen.old_time = timer;
			player->stats.stamina += player->stats.stamina_regen;
			if (player->stats.stamina > player->stats.stamina_max)
			{
				player->stats.stamina = player->stats.stamina_max;
			}
		}
	}
}

void rpg_player_think(Entity *self){
	
	int random_luck = rand() % 1000;
	if (random_luck == 1)
		stat_counter("random");
	old_time = timer;
	timer = SDL_GetTicks();
	rpg_update_player_vitals();
	gf3d_entity_collision_test(self);

	rpg_player_move(self);
	
	rpg_player_input(self);
}

void rpg_player_take_damage(Entity *self, float damage, Entity *inflictor)
{
	slog("Player damaged");
}

void rpg_player_interact(Entity *self, void *data)
{
	slog("Player interacting with...");
}

void rpg_player_death(Entity *self)
{
	slog("Player died");
}

Entity *get_player_entity()
{
	return player->ent;
}

Player *get_player()
{
	return player;
}

PlayerStats get_player_stats()
{
	return player->stats;
}

PlayerInventory get_player_inventory()
{
	return player->inventory;
}

int get_player_inventory_size()
{
	return player->inventory.bagSize;
}

int get_player_spellbook_size()
{
	return player->inventory.spellbookSize;
}

float rpg_player_accelerate(float axis)
{
	float delta_time = (timer - old_time) / 1000.0f;
	if (axis != player->max_speed)
	{
		axis += player->speed * delta_time;
	}
	//slog("\n      Time: %i\nLast Time: %i", timer, old_time);
	return axis;
}


void rpg_player_move(Entity *self){
	const Uint8 *keys;
	const Uint32 x, y;
	const int x_rel, y_rel;
	Vector2D w;

	keys = SDL_GetKeyboardState(NULL);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);

	w = vector2d_from_angle(self->rotation.z);
	self->forward.x = w.x;
	self->forward.y = w.y;
	w = vector2d_from_angle(self->rotation.z - GFC_HALF_PI);
	self->right.x = w.x;
	self->right.y = w.y;

	if (player->state.inAir)
	{
		player->state.onGround = false;
		player->state.inAir = true;
	}
	else
	{
		player->state.onGround = true;
		player->state.inAir = false;
	}

	if (keys[SDL_SCANCODE_LSHIFT])
		player->state.crouched = true;
	else
		player->state.crouched = false;

	//Player input
	if (keys[SDL_SCANCODE_W])
	{
		player->acceleration.x = rpg_player_accelerate(player->acceleration.x);
		self->velocity.x += player->acceleration.x;
	}
	if (keys[SDL_SCANCODE_A])
	{
		player->acceleration.z = rpg_player_accelerate(player->acceleration.z);
		self->velocity.z += -player->acceleration.z;
	}
	if (keys[SDL_SCANCODE_S])
	{
		player->acceleration.x = rpg_player_accelerate(player->acceleration.x);
		self->velocity.x += -player->acceleration.x;
	}
	if (keys[SDL_SCANCODE_D])
	{
		player->acceleration.z = rpg_player_accelerate(player->acceleration.z);
		self->velocity.z += player->acceleration.z;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		if (timer > cd_jump.old_time + 1000)
		{
			player->state.inAir = true;
			if (player->state.crouched)
			{
				self->velocity.y = 70;
				player->stats.stamina -= 10;
			}
			
			else
			{
				self->velocity.y = 60;
				player->stats.stamina -= 5;
			}
				
			cd_jump.old_time = timer;
			stat_counter("jumpamount");
		}
	}
	if (player->state.crouched)
	{
		self->velocity.x * 0.5;
		self->velocity.y * 0.5;
		self->velocity.z * 0.5;
	}

	if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0050;
	if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0050;
	if (keys[SDL_SCANCODE_RIGHT])self->rotation.z -= 0.0050;
	if (keys[SDL_SCANCODE_LEFT])self->rotation.z += 0.0050;
	
	if (x_rel != 0)self->rotation.z -= (x_rel * 0.001);
    if (y_rel != 0)self->rotation.x += (y_rel * 0.001);
}

rpg_player_input(Entity *self)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	int x_rel, y_rel;
	SDL_GetRelativeMouseState(&x_rel, &y_rel);

	if (keys[SDL_SCANCODE_E])
	{
		if (timer > cd_interact.old_time + 1000)
		{
			slog("Interact");
			player_interaction();
			cd_interact.old_time = timer;
		}
		else
		{
			slog("Interact cooldown");
		}

	}
	if (keys[SDL_SCANCODE_I])
	{
		if (timer > cd_inventory.old_time + 2000)
		{
			slog("Inventory");
			print_inventory();
		}
		else
			slog("Inventory cooldown");
	}
	if (keys[SDL_SCANCODE_TAB])
	{
		if (timer > cd_stats.old_time + 500)
		{
			slog("Stats");
			if (player->stats.toggleStats == false)
				player->stats.toggleStats = true;
			else
				player->stats.toggleStats = false;

			cd_stats.old_time = timer;
			print_stats();
		}
		else
			slog("Stat cooldown");
	}
	if (keys[SDL_SCANCODE_M])
	{
		if (timer > cd_map.old_time + 500)
		{
			slog("Map");
			if (player->stats.toggleMap == false)
				player->stats.toggleMap = true;
			else
				player->stats.toggleMap = false;

			cd_map.old_time = timer;
		}
		else
			slog("Map cooldown");
	}

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		//slog("Pressed mouse left");
	}

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		slog("Button: %i", SDL_GetMouseState(NULL, NULL));
		if (timer > cd_secondary_attack.old_time + 3000)
		{
			if (player->stats.mana >= 10)
			{
				rpg_fireball_spawn(self);
				player->stats.mana -= 10;
				cd_secondary_attack.old_time = timer;
				stat_counter("magiccast");
			}
			else
			{
				slog("Not enough mana");
			}
		}
		else
		{
			slog("fireball cool down");
		}
	}
}

void player_interaction()
{
	int i;
	slog("Check interaction...");
	for (i = 0; i < gf3d_get_entity_list_count(); i++)
	{
		if (!gf3d_get_entity_list()[i]._inuse) continue;
		if (gf3d_get_entity_list()[i].name == player->ent->name) continue;
//		slog("Interacting...");
		if (gf3d_get_entity_list()[i].type == INTERACT || gf3d_get_entity_list()[i].type == ITEM || gf3d_get_entity_list()[i].type == NONPLAYER)
		{
			
			float x = player->ent->position.x - gf3d_get_entity_list()[i].position.x;
			float y = player->ent->position.z - gf3d_get_entity_list()[i].position.z;
			
			float result = sqrt( (x * x) + (y * y));
			//slog("checking distance...");
			if (result < player->interactBound.radius)
			{
				slog("Interacting with %s",gf3d_get_entity_list()[i].name);
				if (gf3d_get_entity_list()[i].type == NONPLAYER)
				{
					if (player->stats.toggleShop == false)
					{
						slog("toggleshop true");
						player->stats.toggleShop = true;
					}
					else
					{
						stat_counter("npcconvo");
						slog("toggleshop false");
						player->stats.toggleShop = false;
					}
				}
				gf3d_get_entity_list()[i].interact(&gf3d_get_entity_list()[i], NULL);
			}
		}
	}
}

void print_inventory()
{
	int i;
	for (i = 0; i < get_player_inventory_size(); i++)
	{
		if (player->inventory.bag[i]._inuse)
		{
			slog("       Name: %s", player->inventory.bag[i].name);
			slog("       Type: %i", player->inventory.bag[i].type);
			slog("Description: %s", player->inventory.bag[i].description);
			slog("      Armor: %i", player->inventory.bag[i].armor);
			slog("     Damage: %i", player->inventory.bag[i].damage);
			slog("   Quantity: %i", player->inventory.bag[i].quantity);
			slog("     Weight: %f", player->inventory.bag[i].weight);
		}
	}
}

void print_stats()
{
	slog("            Name: %s", get_player()->stats.name);
	slog("           Level: %i", player->stats.level);
	slog("             Exp: %i", player->stats.exp);
	slog("            Bits: %i", player->stats.bits);

	slog("            Life: %i", player->stats.life);
	slog("        Life Max: %i", player->stats.life_max);
	slog("      Life Regen: %i", player->stats.life_regen);
	slog("            Mana: %i", player->stats.mana);
	slog("        Mana Max: %i", player->stats.mana_max);
	slog("      Mana Regen: %i", player->stats.mana_regen);
	slog("         Stamina: %i", player->stats.stamina);
	slog("     Stamina Max: %i", player->stats.stamina_max);
	slog("   Stamina Regen: %i", player->stats.stamina_regen);

	slog("    Carry Weight: %f", player->stats.carry_weight);
	slog("Carry Weight Max: %f", player->stats.carry_weight_max);

	slog("        Strength: %i", player->stats.strength);
	slog("    Strength Max: %i", player->stats.strength_max);
	slog("          Health: %i", player->stats.health);
	slog("      Health Max: %i", player->stats.health_max);
	slog("      Perception: %i", player->stats.perception);
	slog("  Perception Max: %i", player->stats.perception_max);
	slog("           Speed: %i", player->stats.speed);
	slog("       Speed Max: %i", player->stats.speed_max);
	slog("       Willpower: %i", player->stats.willpower);
	slog("   Willpower Max: %i", player->stats.willpower_max);
	slog("       Dexterity: %i", player->stats.dexterity);
	slog("   Dexterity Max: %i", player->stats.dexterity_max);
	slog("    Intelligence: %i", player->stats.intelligence);
	slog("Intelligence Max: %i", player->stats.intelligence_max);
	slog("            Luck: %i", player->stats.luck);
	slog("        Luck Max: %i", player->stats.luck_max);
}

void print_player_name()
{
	slog("Name: %s", player->stats.name);
	slog("Name: %s", get_player()->stats.name);
}

/*eol@eof*/
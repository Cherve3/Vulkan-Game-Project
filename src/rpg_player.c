
#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_object.h"
#include "simple_json_parse.h"
#include "simple_json_value.h"
#include "simple_json_string.h"
#include "simple_json_list.h"
#include "simple_json_array.h"
#include "simple_json_error.h"

#include "gf3d_camera.h"

#include "rpg_player.h"
#include "rpg_spellbook.h"
#include "rpg_items.h"
#include "rpg_projectile.h"

static Player *player = { 0 };
Matrix4 *camera = { 0 };

Uint32 timer;
Uint32 old_time;

void rpg_player_move(Entity *self);
void rpg_player_update();
void print_inventory();

void rpg_player_init(){
	camera = gf3d_get_camera();

	player = (Player *)gfc_allocate_array(sizeof(Player),1);

	player->ent = rpg_player_new();

	
	old_time = 0;
	/**
	 *	Attempting to use the JSON Lib
	 */
	sj_enable_debug();

	SJson *player_info = sj_load("json/player.json");
	if (!player_info)
		slog("player_info is NULL");
	
	sj_echo(player_info);

	slog("Is SJson an object? %i",sj_is_object(player_info));

	slog("Text: %i", player_info->get_string("life")->text);
	slog("Length: %i", player_info->get_string("life")->length);
	slog("Size: %i", player_info->get_string("life")->size);// player_info->get_string(player_info)->text);

	slog("Array Count: %i", player_info->v.array->count);
	slog("Array Elements: %i", player_info->v.array->elements);
	slog("Array Size: %i", player_info->v.array->size);
	slog("Array Count: %i", player_info->v.array->elements[0]);
	
	/**
	 * Player Stats
	 */
	player->ent->model = gf3d_model_load("dino");
	player->ent->think = rpg_player_think;
	player->ent->update = rpg_player_update;
	player->ent->type = PLAYER;
	player->ent->name = "Player";
	player->ent->type = PLAYER;
	player->ent->position = vector3d(0, 3.2, -10);
	player->ent->velocity = vector3d(0, 0, 0);
	player->ent->rotation = vector3d(0, 0, 0);
	player->ent->direction = vector3d(0, 0, 1);

	player->ent->boxCollider.width = 3.0;
	player->ent->boxCollider.height = 2.8;
	player->ent->boxCollider.depth = 3.0;
	player->ent->boxCollider.x = player->ent->position.x;
	player->ent->boxCollider.y = player->ent->position.y;
	player->ent->boxCollider.z = player->ent->position.z;

	player->interactBound.radius = 6;
	player->interactBound.x = player->ent->position.x;
	player->interactBound.y = player->ent->position.z;
	
	player->stats.name				= "Player 1";
	player->stats.level				= 1;
	player->stats.exp				= 0;

	player->stats.life				= 100;
	player->stats.life_max			= 100;
	player->stats.life_regen		= 100;
	player->stats.mana				= 100;
	player->stats.mana_max			= 100;
	player->stats.magic_regen		= 100;
	player->stats.stamina			= 100;
	player->stats.stamina_max		= 100;
	player->stats.stamina_regen		= 100;

	player->stats.carry_weight		= 65.00;	//Value is in kilograms

	player->stats.strength			= 1;
	player->stats.strength_max		= 100;
	player->stats.health			= 1;
	player->stats.health_max		= 100;
	player->stats.perception		= 1;
	player->stats.perception_max	= 100;
	player->stats.speed				= 1;
	player->stats.speed_max			= 100;
	player->stats.willpower			= 1;
	player->stats.willpower_max		= 100;
	player->stats.dexterity			= 1;
	player->stats.dexterity_max		= 100;
	player->stats.intelligence		= 1;
	player->stats.intelligence_max	= 100;
	player->stats.luck				= 1;
	player->stats.luck_max			= 100;

	player->stats.toggleStats = false;
	
	player->inventory.bagSize = 30;
	player->inventory.spellbookSize = 5;
	player->inventory.bag = (Item *)gfc_allocate_array(sizeof(Item), player->inventory.bagSize);
	player->inventory.spellbook = (Spell *)gfc_allocate_array(sizeof(Spell), player->inventory.spellbookSize);
	
}

Entity *rpg_player_new(){

	return gf3d_entity_new();
}

void rpg_player_free(Player *player)
{
	if (!player) return;
	rpg_spellbook_free(player->inventory.spellbook);
	rpg_player_bag_free(player->inventory.bag);
	rpg_player_inventory_free(&player->inventory);
	
	gf3d_entity_free(player->ent);
	memset(player, 0, sizeof(Player));
}

void rpg_player_inventory_free(pInventory *inventory)
{
	if (!inventory) return;
	memset(inventory, 0, sizeof(pInventory));
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
	if (vector3d_magnitude(self->velocity) > 0.001)
	{
		self->velocity.y += GRAVITY;
		vector3d_scale(self->velocity, self->velocity, 0.3);
		self->position.x += self->velocity.x;

		if (player->state.onGround = true);
			self->position.y += self->velocity.y;

		self->position.z += self->velocity.z;
		//vector3d_add(self->position, self->position, self->velocity);
	}
	else
	{
		vector3d_clear(self->velocity);
	}
	
	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

	//slog("\nPosition: x:%f, y:%f, z:%f", self->position.x, self->position.y, self->position.z);
	//slog("\nVelocity: x:%f, y:%f, z:%f", self->velocity.x, self->velocity.y, self->velocity.z);
	//slog("\nRotation: x:%f, y:%f, z:%f", self->rotation.x, self->rotation.y, self->rotation.z);

}

void rpg_player_think(Entity *self){
	
	gf3d_entity_collision_test(self);

	rpg_player_move(self);
	timer = SDL_GetTicks();
	rpg_player_input(self);
}

Entity *get_player_entity()
{
	return player->ent;
}

Player *get_player()
{
	return player;
}

pStats get_player_stats()
{
	return player->stats;
}

pInventory get_player_inventory()
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

void rpg_player_move(Entity *self){
	const Uint8 *keys;
	const Uint32 x, y;
	const int x_rel, y_rel;

	keys = SDL_GetKeyboardState(NULL);

	SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);

	float angle = x_rel*GFC_DEGTORAD;

	//Player input
	if (keys[SDL_SCANCODE_W])
	{
		if (player->state.crouched)
			self->velocity.z -= 0.5;
		else
			self->velocity.z -= 1;
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (player->state.crouched)
			self->velocity.x -= 0.5;
		else
			self->velocity.x -= 1;
	}
	if (keys[SDL_SCANCODE_S])
	{
		if (player->state.crouched)
			self->velocity.z += 0.5;
		else
			self->velocity.z += 1;
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (player->state.crouched)
			self->velocity.x += 0.5;
		else
			self->velocity.x += 1;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		player->state.inAir = true;
		if (player->state.crouched)
			self->velocity.y = 15;
		else
			self->velocity.y = 11;
	}
	
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
	//Toggle Crouch state
	if (keys[SDL_SCANCODE_LSHIFT])
		player->state.crouched = true;
	else
		player->state.crouched = false;

	gfc_matrix_rotate(self->modelMatrix, self->modelMatrix, angle,vector_up());
	
	//Move model to position
	gfc_matrix_new_translation(self->modelMatrix,self->position);
	
	camera_update(self->rotation, self->position, self->modelMatrix, x_rel, y_rel);
	
}

rpg_player_input(Entity *self)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	int x_rel, y_rel;
	SDL_GetRelativeMouseState(&x_rel, &y_rel);

	if (keys[SDL_SCANCODE_E])
	{
		slog("Interact");
		player_interaction();

	}
	if (keys[SDL_SCANCODE_I])
	{
		slog("Inventory");
	}
	if (keys[SDL_SCANCODE_TAB])
	{
		slog("Stats");
		if (player->stats.toggleStats == false)
			player->stats.toggleStats = true;
		else
			player->stats.toggleStats = false;
	}

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		slog("Button: %i", SDL_GetMouseState(NULL, NULL));
		slog("\n      Time: %i\nLast Time: %i", timer, old_time);
		if (timer > old_time + 5000)
		{
			if (player->stats.mana >= 10)
			{
				rpg_fireball_spawn(self);
				player->stats.mana -= 10;
				old_time = timer;
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

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEWHEEL)
		{
			if (event.wheel.y > 0) // scroll up
			{
				gf3d_camera_move(vector_forward());
			}
			else if (event.wheel.y < 0) // scroll down
			{
				gf3d_camera_move(vector_backward());
			}
		}
	}
}

void player_interaction()
{
	int i;
	slog("Initial interacting...");
	for (i = 0; i < gf3d_get_entity_list_count(); i++)
	{
		if (!gf3d_get_entity_list()[i]._inuse) continue;
		if (gf3d_get_entity_list()[i].name == player->ent->name) continue;
		slog("Interacting...");
		if (gf3d_get_entity_list()[i].type == INTERACT)
		{

			float x = player->ent->position.x - gf3d_get_entity_list()[i].position.x;
			float y = player->ent->position.z - gf3d_get_entity_list()[i].position.z;
			
			float result = sqrt( (x * x) + (y * y));
			slog("checking distance...");
			slog("Result: %f", result);
			if (result < player->interactBound.radius)
			{
				slog("Interacting with %s",gf3d_get_entity_list()[i].name);
				gf3d_get_entity_list()[i].interact(&gf3d_get_entity_list()[i]);
				slog("%s\n", player->inventory.bag[0].description);
				print_inventory();
			}
		}
	}
}

void print_inventory()
{
	int i;
	for (i = 0; i < get_player_inventory_size(); i++)
	{
		slog("\n       Name: %s\n", player->inventory.bag[i].name);
		slog("       Type: %i\n", player->inventory.bag[i].type);
		slog("Description: %s\n", player->inventory.bag[i].description);
		slog("      Armor: %i\n", player->inventory.bag[i].armor);
		slog("     Damage: %i", player->inventory.bag[i].damage);
		slog("   Quantity: %i", player->inventory.bag[i].quantity);
		slog("     Weight: %f", player->inventory.bag[i].weight);
	}
}
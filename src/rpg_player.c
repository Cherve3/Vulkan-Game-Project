
#include "simple_logger.h"

#include "gf3d_camera.h"

#include "rpg_player.h"
#include "rpg_spellbook.h"
#include "rpg_items.h"

static Player *player = { 0 };

void rpg_player_move(Entity *self);
void rpg_player_update();

void rpg_player_init(){

	player = (Player *)gfc_allocate_array(sizeof(Player),1);

	player->ent = rpg_player_new();

	player->ent->model = gf3d_model_load("dino");
	player->ent->think = rpg_player_think;
	player->ent->update = rpg_player_update;
	player->ent->name = "Player";
	
	player->ent->position = vector3d(0, 0, 0);
	player->ent->velocity = vector3d(0, 0, 0);
	player->ent->rotation = vector3d(0, 0, 0);

	
	player->ent->boxCollider.depth = 5.0;
	player->ent->boxCollider.height = 5.0;
	player->ent->boxCollider.width = 5.0;
	player->ent->boxCollider.x = player->ent->position.x;
	player->ent->boxCollider.y = player->ent->position.y;
	player->ent->boxCollider.z = player->ent->position.z;
	
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
	
	player->inventory.bag = (Item *)gfc_allocate_array(sizeof(Item), 30);
	player->inventory.spellbook = (Spell *)gfc_allocate_array(sizeof(Spell),5);
	
}

Entity *rpg_player_new(){

	return gf3d_entity_new();
}

void rpg_player_free(Player *player)
{
	if (!player) return;
	rpg_spellbook_free(player->inventory.spellbook);
	rpg_player_bag_free(player->inventory.bag);
	//rpg_player_inventory_free(player->inventory);
	
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
		vector3d_add(self->position, self->position, self->velocity);
		vector3d_scale(self->velocity, self->velocity, 0.01);
	}
	else
	{
		vector3d_clear(self->velocity);
	}
	
	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;

	slog("\nPosition: x:%f, y:%f, z:%f", self->position.x, self->position.y, self->position.z);
	slog("\nVelocity: x:%f, y:%f, z:%f", self->velocity.x, self->velocity.y, self->velocity.z);
	slog("\nRotation: x:%f, y:%f, z:%f", self->rotation.x, self->rotation.y, self->rotation.z);

}

void rpg_player_think(Entity *self){

	gf3d_entity_collision_test(self);

	rpg_player_move(self);
	Vector3D negate;
	vector3d_negate(negate,gf3d_camera_get_position());
	gf3d_camera_look_at(negate,self->position,vector_up());
	
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

void rpg_player_move(Entity *self){
	const Uint8 *keys;
	const Uint32 x, y;
	const int x_rel, y_rel;

	Matrix4 *camera = gf3d_get_camera();
	
	keys = SDL_GetKeyboardState(NULL);
	
	SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);
	
	vector3d_rotate_about_vector(&self->rotation, self->position,self->direction, x_rel*0.01);
	slog("\nRotation: x:%f, y:%f, z:%f", self->rotation.x, self->rotation.y, self->rotation.z);
	//Player input
	if (keys[SDL_SCANCODE_W])
	{
		self->velocity.z += 1;
	}
	if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x += 1;
	}
	if (keys[SDL_SCANCODE_S])
	{
		self->velocity.z -= 1;
	}
	if (keys[SDL_SCANCODE_D])
	{
		self->velocity.x -= 1;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		self->velocity.y += 1;
	}
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		self->velocity.y -= 1;
	}
	if (keys[SDL_SCANCODE_E])
	{
		slog("Interact");
	}
	if (keys[SDL_SCANCODE_I])
	{
		slog("Inventory");
	}
	if (keys[SDL_SCANCODE_TAB])
	{
		slog("Stats");

	}
	Matrix4 temp;
	gfc_matrix_new_translation(self->modelMatrix, self->position);
	//gfc_matrix_new_translation(camera, self->position);
	
}

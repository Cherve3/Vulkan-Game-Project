
#include "simple_logger.h"

#include "gf3d_camera.h"
#include "gf3d_vgraphics.h"

#include "rpg_player.h"
#include "rpg_spellbook.h"
#include "rpg_items.h"

static Player *player = { 0 };

void rpg_player_move(Entity *self);

void rpg_player_init(){

	player = (Player *)gfc_allocate_array(sizeof(Player),3);

	player->ent = rpg_player_new();
	player->ent->position = vector3d(0, 0, 0);
	player->ent->velocity = vector3d(0, 0, 0);
	player->ent->rotation = vector3d(0, 0, 0);

	player->ent->boxCollider.depth = 1.0;
	player->ent->boxCollider.height = 1.0;
	player->ent->boxCollider.width = 1.0;
	player->ent->boxCollider.x = player->ent->position.x;
	player->ent->boxCollider.y = player->ent->position.y;
	player->ent->boxCollider.z = player->ent->position.z;

	
	player->stats.name				= "Player";
	player->stats.level				= 1;
	player->stats.exp				= 0;

	player->stats.life				= 100;
	player->stats.life_max			= 100;
	player->stats.life_regen		= 100;
	player->stats.magic				= 100;
	player->stats.magic_max			= 100;
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

void rpg_player_think(Entity *self){
	
	rpg_player_move(self);
	gf3d_entity_collision_test(self);
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
	Matrix4 *camera = gf3d_vgraphics_get_ubo_view();
	
	keys = SDL_GetKeyboardState(NULL);
	
	SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);


	if (keys[SDL_SCANCODE_W])
	{
		self->velocity.y = -0.2;
		vector3d_add(self->position, self->position, self->velocity);
		gfc_matrix_translate(self->modelMatrix, vector_backward());
		gfc_matrix_translate(gf3d_vgraphics_get_ubo_view(),vector_up());
		
	}

	if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x = 0.2;
		vector3d_add(self->position, self->position, self->velocity);
		gfc_matrix_translate(self->modelMatrix, vector_right());
		gfc_matrix_translate(gf3d_vgraphics_get_ubo_view(), vector_right());
	}

	if (keys[SDL_SCANCODE_S])
	{
		self->velocity.y = 0.2;
		vector3d_add(self->position, self->position, self->velocity);
		gfc_matrix_translate(self->modelMatrix, vector_forward());
		gfc_matrix_translate(gf3d_vgraphics_get_ubo_view(), vector_down());

	}

	if (keys[SDL_SCANCODE_D])
	{
		self->velocity.x = -0.2;
		vector3d_add(self->position, self->position, self->velocity);
		gfc_matrix_translate(self->modelMatrix, vector_left());
		gfc_matrix_translate(gf3d_vgraphics_get_ubo_view(), vector_left());
	}

	if (keys[SDL_SCANCODE_SPACE])
	{
		self->velocity.z = 0.2;
		vector3d_add(self->position, self->position, self->velocity);
	}

	if (keys[SDL_SCANCODE_LSHIFT])
	{
		self->velocity.z = -0.2;
		vector3d_add(self->position, self->position, self->velocity);
	}
	if (keys[SDL_SCANCODE_W] == 0 && keys[SDL_SCANCODE_A] == 0 && keys[SDL_SCANCODE_S] == 0 && keys[SDL_SCANCODE_D] == 0 && keys[SDL_SCANCODE_SPACE]){
		self->velocity = vector3d(0, 0, 0);
	}

	gfc_matrix_rotate(camera, camera, x_rel*0.01, vector3d(0, 0, -1));
	gfc_matrix_rotate(self->modelMatrix, self->modelMatrix, x_rel*0.01, vector3d(0, 0, 1));
	
}

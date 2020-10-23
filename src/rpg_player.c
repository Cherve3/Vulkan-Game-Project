
#include "simple_logger.h"

#include "rpg_player.h"
#include "gf3d_camera.h"
#include "gf3d_vgraphics.h"

static Entity *player = NULL;

void rpg_player_move(Entity *self);

void rpg_player_init(){
	player = gf3d_entity_new();

	player->position = vector3d(0, 0, 0);
	player->velocity = vector3d(0, 0, 0);
	player->rotation = vector3d(0, 0, 0);

	player->boxCollider.depth  = 1.0;
	player->boxCollider.height = 1.0;
	player->boxCollider.width  = 1.0;
	player->boxCollider.x = player->position.x;
	player->boxCollider.y = player->position.y;
	player->boxCollider.z = player->position.z;

	pStats stats;
	pInventory inventory;

	stats.name					= "Player";
	stats.level					= 1;
	stats.exp					= 0;

	stats.life					= 100;
	stats.life_max				= 100;
	stats.life_regen			= 100;
	stats.magic					= 100;
	stats.magic_max				= 100;
	stats.magic_regen			= 100;
	stats.stamina				= 100;
	stats.stamina_max			= 100;
	stats.stamina_regen			= 100;

	stats.carry_weight			= 65.00;	//Value is in kilograms

	stats.strength				= 1;
	stats.strength_max			= 100;
	stats.health				= 1;
	stats.health_max			= 100;
	stats.perception			= 1;
	stats.perception_max		= 100;
	stats.speed					= 1;
	stats.speed_max				= 100;
	stats.willpower				= 1;
	stats.willpower_max			= 100;
	stats.dexterity				= 1;
	stats.dexterity_max			= 100;
	stats.intelligence			= 1;
	stats.intelligence_max		= 100;
	stats.luck					= 1;
	stats.luck_max				= 100;
	
}

Entity *rpg_player_new(){

	return gf3d_entity_new();
}

void rpg_player_think(Entity *self){
	
	rpg_player_move(self);
	gf3d_entity_collision_test(self);
}

void rpg_player_move(Entity *self){
	const Uint8 *keys;
	const Uint32 x, y;
	const int x_rel, y_rel;

	keys = SDL_GetKeyboardState(NULL);
	
	SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);
	if (keys[SDL_SCANCODE_W])
	{
		self->velocity.y = -0.2;
		vector3d_add(self->position, self->position, self->velocity);
	}

	if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x = 0.2;
		vector3d_add(self->position, self->position, self->velocity);
	}

	if (keys[SDL_SCANCODE_S])
	{
		self->velocity.y = 0.2;
		vector3d_add(self->position, self->position, self->velocity);
	}

	if (keys[SDL_SCANCODE_D])
	{
		self->velocity.x = -0.2;
		vector3d_add(self->position, self->position, self->velocity);
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
	if (keys[SDL_SCANCODE_W] == 0 && keys[SDL_SCANCODE_A] == 0 && keys[SDL_SCANCODE_S] == 0 && keys[SDL_SCANCODE_D] == 0){
		self->velocity = vector3d(0, 0, 0);
	}
	
	gfc_matrix_make_translation(self->modelMatrix, self->position);

	self->boxCollider.x = self->position.x;
	self->boxCollider.y = self->position.y;
	self->boxCollider.z = self->position.z;
	//if (y_rel < 3 && y_rel > -3){
		gf3d_vgraphics_rotate_z(x_rel*0.01);
	//}
	//if (x_rel < 3 && x_rel > -3){
		//gf3d_vgraphics_rotate_x(y_rel*0.01);
	//}
	gfc_matrix_rotate(self->modelMatrix, self->modelMatrix, x_rel*0.01, vector3d(0, 0, 1));

}
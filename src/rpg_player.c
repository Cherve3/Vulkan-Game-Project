
#include "simple_logger.h"

#include "rpg_player.h"
#include "gf3d_camera.h"

static Entity *player = NULL;

void rpg_player_move(Entity *self);

void rpg_player_init(){
	player = gf3d_entity_new();

	Stats stats;
	Inventory inventory;

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



}

void rpg_player_move(Entity *self){
	const Uint8 *keys;
	const Uint32 x, y;

	keys = SDL_GetKeyboardState(NULL);
	
	SDL_GetMouseState(&x, &y);

	if (keys[SDL_SCANCODE_W])
	{
		self->velocity.y -= 0.1;
	}
	if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x += 0.1;
	}
	if (keys[SDL_SCANCODE_S])
	{
		self->velocity.y += 0.1;
	}
	if (keys[SDL_SCANCODE_D])
	{
		self->velocity.x -= 0.1;
	}
	gf3d_camera_move(self->velocity);
	gfc_matrix_make_translation(self->modelMatrix, self->velocity);
	
	gfc_matrix_rotate(self->modelMatrix, self->modelMatrix, x*0.01, vector3d(0, 0, 1));
}
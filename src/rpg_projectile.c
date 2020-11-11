#include "simple_logger.h"

#include "rpg_projectile.h"

void rpg_fireball_spawn(Entity *player)
{	
	Fireball fireball;
	
	if (!player)return;

	if (fireball.ent = NULL)
	{
		fireball.ent = gf3d_entity_new();
		if (!fireball.ent)return;
	}
	fireball.ent->model = gf3d_model_load("fire");
	if (!fireball.ent->model){
		slog("Model Null"); 
		return;
	}
	fireball.ent->name = "Fire";
	fireball.damage = 10;
	fireball.ent->position.x = player->position.x;
	fireball.ent->position.y = player->position.y + 2;
	fireball.ent->position.z = player->position.z + 1;
	gfc_matrix_new_translation(fireball.ent->modelMatrix,fireball.ent->position);
	slog("Fireball Position: X: %f Y: %f Z: %f", fireball.ent->position.x, fireball.ent->position.y, fireball.ent->position.z);
/*	fireball.ent->think = rpg_fireball_think;
	fireball.ent->update = rpg_fireball_update;
	fireball.collider.radius = 1;
	fireball.collider.x = fireball.ent->position.x;
	fireball.collider.y = fireball.ent->position.y;
	fireball.collider.z = fireball.ent->position.z;
	*/
}

void rpg_fireball_despawn()
{

}

void rpg_fireball_think(Entity *self)
{
	if (!self)return;
	self->velocity = vector3d(0, 0, 1);
}

void rpg_fireball_update(Entity *self)
{
	if (!self)return;
	vector3d_add(self->position, self->position, self->velocity);
	vector3d_scale(self->velocity, self->velocity, 0.01);
}
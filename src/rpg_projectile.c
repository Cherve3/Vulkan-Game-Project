#include "simple_logger.h"

#include "rpg_projectile.h"

typedef struct
{
	Projectile *projectile_list;
	Uint8 projectile_count;
}ProjectileManager;

ProjectileManager rpg_projectiles = { 0 };

void rpg_projectiles_free(Projectile *proj)
{
	if (!proj) return;
	gf3d_entity_free(proj->ent);
	memset(proj, 0, sizeof(Projectile));
}

void rpg_projectiles_close()
{
	int i;
	if (rpg_projectiles.projectile_list != NULL)
	{
		for (i = 0; i < rpg_projectiles.projectile_count; i++)
		{
			rpg_projectiles_free(&rpg_projectiles.projectile_list[i]);
		}
		free(rpg_projectiles.projectile_list);
	}
	memset(&rpg_projectiles, 0, sizeof(ProjectileManager));
	slog("Projectile System Closed");
}


void rpg_projectile_init(Uint8 maxProjectiles)
{
	if (rpg_projectiles.projectile_list != NULL)
	{
		slog("WARNING: projectile system already initialized");
		return;
	}
	rpg_projectiles.projectile_list = gfc_allocate_array(sizeof(Projectile), maxProjectiles);
	if (!rpg_projectiles.projectile_list)
	{
		slog("failed to allocate projectile list");
		return;
	}
	rpg_projectiles.projectile_count = maxProjectiles;
	atexit(rpg_projectiles_close);
	slog("Projectile System Initialized");
}

void rpg_fireball_spawn(Entity *player)
{	
	int i;
	if (!player)return;

	for (i = 0; i < rpg_projectiles.projectile_count; i++)
	{
		if (!rpg_projectiles.projectile_list[i]._inuse)
		{
			rpg_projectiles.projectile_list[i]._inuse = 1;

			rpg_projectiles.projectile_list[i].ent = gf3d_entity_new();
			rpg_projectiles.projectile_list[i].ent->model = gf3d_model_load("fireball");
			rpg_projectiles.projectile_list[i].ent->parent = player;

			rpg_projectiles.projectile_list[i].ent->position = 
				vector3d(player->position.x, player->position.y +2, player->position.z);

			rpg_projectiles.projectile_list[i].ent->name = "Fireball";
			rpg_projectiles.projectile_list[i].ent->think = rpg_fireball_think;
			rpg_projectiles.projectile_list[i].ent->update = rpg_fireball_update;
			
			rpg_projectiles.projectile_list[i].collider.x = rpg_projectiles.projectile_list[i].ent->position.x;
			rpg_projectiles.projectile_list[i].collider.y = rpg_projectiles.projectile_list[i].ent->position.y;
			rpg_projectiles.projectile_list[i].collider.z = rpg_projectiles.projectile_list[i].ent->position.z;
			rpg_projectiles.projectile_list[i].collider.radius = 5;
			
			
			gfc_matrix_new_translation(
				rpg_projectiles.projectile_list[i].ent->modelMatrix, 
				rpg_projectiles.projectile_list[i].ent->position);

			return &rpg_projectiles.projectile_list[i];
		}
	}
	slog("Failed to provide new fireball, no unused slots");
	return NULL;
	
}

void rpg_fireball_despawn()
{

}

void rpg_fireball_think(Entity *self)
{
	float x = -self->parent->forward.x;
	float z = -self->parent->forward.y;
	slog("X: %f Z: %f", x, z);
	if (!self)return;
		self->velocity = vector3d(x, 0, z);

}

void rpg_fireball_update(Entity *self)
{
	if (!self)return;
	vector3d_scale(self->velocity, self->velocity, 0.2);
	vector3d_add(self->position, self->position, self->velocity);
	gfc_matrix_new_translation(self->modelMatrix,self->position);
	
}
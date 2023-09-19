
#include "simple_logger.h"

#include "gf3d_entity.h"

typedef struct
{
	Entity *entity_list;	/**<List of entities*/
	Uint32 entity_count;	/**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager entity_manager = { 0 };

void gf3d_entity_free(Entity *entity);

void gf3d_entity_close()
{
	int i;
	if (entity_manager.entity_list != NULL)
	{
		for (i = 0; i < entity_manager.entity_count; i++)
		{
			gf3d_entity_free(&entity_manager.entity_list[i]);
		}
		free(entity_manager.entity_list);
	}
	memset(&entity_manager, 0, sizeof(EntityManager));
	slog("Entity System Closed");
}

void gf3d_entity_init(Uint32 maxEntities)
{
	if (entity_manager.entity_list != NULL)
	{
		slog("WARNING: entity system already initialized");
		return;
	}
	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), maxEntities);
	if (!entity_manager.entity_list)
	{
		slog("failed to allocate entity list");
		return;
	}
	entity_manager.entity_count = maxEntities;
	atexit(gf3d_entity_close);
	slog("Entity System Initialized");
}

void gf3d_entity_free(Entity *entity)
{
	if (!entity) return;
	gf3d_model_free(entity->model);
	memset(entity,0,sizeof(Entity));
}

Entity *gf3d_entity_new()
{
	int i;
	for(i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse)
		{
			entity_manager.entity_list[i]._inuse = 1;
			gfc_matrix_identity(entity_manager.entity_list[i].modelMatrix);
			vector3d_clear(entity_manager.entity_list[i].position);
			vector3d_clear(entity_manager.entity_list[i].velocity);
			vector3d_clear(entity_manager.entity_list[i].forward);
			vector3d_clear(entity_manager.entity_list[i].right);
			vector3d_clear(entity_manager.entity_list[i].up);
			entity_manager.entity_list[i].scale.x = 1;
			entity_manager.entity_list[i].scale.y = 1;
			entity_manager.entity_list[i].scale.z = 1;
			
			entity_manager.entity_list[i].color = gfc_color(1, 1, 1, 1);
			return &entity_manager.entity_list[i];
		}
	}
	slog("Failed to provide new entity, no unused slots");
	return NULL;
}

void gf3d_entity_update(Entity *self){
	if (!self) return;

	vector3d_add(self->position, self->position, self->velocity);

	gfc_matrix_identity(self->modelMatrix);

	gfc_matrix_scale(self->modelMatrix, self->scale);
	gfc_matrix_rotate_by_vector(self->modelMatrix, self->modelMatrix, self->rotation);
	gfc_matrix_translate(self->modelMatrix, self->position);

	if (self->update)self->update(self);
	
}

void gf3d_entity_update_all()
{
	int i;
	for (i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;
		gf3d_entity_update(&entity_manager.entity_list[i]);
	}
}

void gf3d_entity_think(Entity *self)
{
	if (!self)return;
	if (!self->think)return; // No think function
	self->think(self);
}

void gf3d_entity_interact(Entity *self, void *data)
{
	if (!self)return;
	if (!self->interact)return; // No interact function
	self->interact(self, data);
}

void gf3d_entity_touch(Entity *self, Entity *other)
{
	if (!self)return;
	if (!self->touch)return; // No touch function
	self->touch(self, other);
}

void gf3d_entity_damage(Entity *self, float damage, Entity *inflictor)
{
	if (!self)return;
	if (!self->damage)return; // No damage function
	self->damage(self, damage, inflictor);
}

void gf3d_entity_on_death(Entity *self)
{
	if (!self)return;
	if (!self->on_death)return; // No on death function
	self->on_death(self);
}

void gf3d_entity_think_all()
{
	int i;
	for (i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;
		gf3d_entity_think(&entity_manager.entity_list[i]);
	}
}

void gf3d_entity_draw(Entity *self)
{
	if (!self) return;
	if (self->animated)return;
	gf3d_model_draw(self->model, self->modelMatrix, gfc_color_to_vector4f(self->color), vector4d(1, 1, 1, 1));

}

void gf3d_entity_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.entity_count; i++)
	{
		if (!entity_manager.entity_list[i]._inuse) continue;
		gf3d_entity_draw(&entity_manager.entity_list[i], gfc_color_to_vector4f(entity_manager.entity_list[i].color), vector4d(1, 1, 1, 1));
	}
}

void gf3d_entity_collision_test(Entity *self){
	int i;

	for (i = 0; i < entity_manager.entity_count; i++)
	{
		
		if (!entity_manager.entity_list[i]._inuse) continue;
		if (&entity_manager.entity_list[i] == self) continue;
		if (&entity_manager.entity_list[i] == self->parent) continue;

		if ( ((self->boxCollider.x - self->boxCollider.width) <= (entity_manager.entity_list[i].boxCollider.x + entity_manager.entity_list[i].boxCollider.width)) &&
			 ((self->boxCollider.x + self->boxCollider.width) >= (entity_manager.entity_list[i].boxCollider.x - entity_manager.entity_list[i].boxCollider.width)) &&
		     ((self->boxCollider.y - self->boxCollider.height) <= (entity_manager.entity_list[i].boxCollider.y + entity_manager.entity_list[i].boxCollider.height)) &&
			 ((self->boxCollider.y + self->boxCollider.height) >= (entity_manager.entity_list[i].boxCollider.y - entity_manager.entity_list[i].boxCollider.height)) &&
		     ((self->boxCollider.z - self->boxCollider.depth) <= (entity_manager.entity_list[i].boxCollider.z + entity_manager.entity_list[i].boxCollider.depth)) &&
			 ((self->boxCollider.z + self->boxCollider.depth) >= (entity_manager.entity_list[i].boxCollider.z - entity_manager.entity_list[i].boxCollider.depth)) )
		{
			//Check X
			if ( (self->boxCollider.x - self->boxCollider.width) <= (entity_manager.entity_list[i].boxCollider.x + entity_manager.entity_list[i].boxCollider.width) &&
				((self->boxCollider.x - self->boxCollider.width) >= (entity_manager.entity_list[i].boxCollider.x - entity_manager.entity_list[i].boxCollider.width)) && entity_manager.entity_list[i].type != WORLD)
			{
				self->position.x += (entity_manager.entity_list[i].boxCollider.x + entity_manager.entity_list[i].boxCollider.width) - (self->boxCollider.x - self->boxCollider.width);
			}
			else if ( (self->boxCollider.x + self->boxCollider.width) <= (entity_manager.entity_list[i].boxCollider.x + entity_manager.entity_list[i].boxCollider.width) &&
				((self->boxCollider.x + self->boxCollider.width) >= (entity_manager.entity_list[i].boxCollider.x - entity_manager.entity_list[i].boxCollider.width)) && entity_manager.entity_list[i].type != WORLD)
			{

				self->position.x += (entity_manager.entity_list[i].boxCollider.x - entity_manager.entity_list[i].boxCollider.width) - (self->boxCollider.x + self->boxCollider.width);
			}

			//Check Y
			if (((self->boxCollider.y - self->boxCollider.height) <= (entity_manager.entity_list[i].boxCollider.y + entity_manager.entity_list[i].boxCollider.height)) &&
				((self->boxCollider.y + self->boxCollider.height) >= (entity_manager.entity_list[i].boxCollider.y - entity_manager.entity_list[i].boxCollider.height)) && entity_manager.entity_list[i].name == "World")
			{
				self->position.y += (entity_manager.entity_list[i].boxCollider.y + entity_manager.entity_list[i].boxCollider.height) - (self->boxCollider.y - self->boxCollider.height);
			}

			//Check Z
			if ((self->boxCollider.z - self->boxCollider.depth) <= (entity_manager.entity_list[i].boxCollider.z + entity_manager.entity_list[i].boxCollider.depth) &&
				((self->boxCollider.z - self->boxCollider.depth) >= (entity_manager.entity_list[i].boxCollider.z - entity_manager.entity_list[i].boxCollider.depth)) && entity_manager.entity_list[i].type != WORLD)
			{
				self->position.z += (entity_manager.entity_list[i].boxCollider.z + entity_manager.entity_list[i].boxCollider.depth) - (self->boxCollider.z - self->boxCollider.depth);
			}
			else if ((self->boxCollider.z + self->boxCollider.depth) <= (entity_manager.entity_list[i].boxCollider.z + entity_manager.entity_list[i].boxCollider.depth) &&
				((self->boxCollider.z + self->boxCollider.depth) >= (entity_manager.entity_list[i].boxCollider.z - entity_manager.entity_list[i].boxCollider.depth)) && entity_manager.entity_list[i].type != WORLD)
			{

				self->position.z += (entity_manager.entity_list[i].boxCollider.z - entity_manager.entity_list[i].boxCollider.depth) - (self->boxCollider.z + self->boxCollider.depth);
			}
			gf3d_entity_touch(self, &entity_manager.entity_list[i]);
		}
	}
}

Entity* gf3d_get_entity_list()
{	
	return entity_manager.entity_list;
}

int gf3d_get_entity_list_count()
{
	return entity_manager.entity_count;
}

/*eol@eof*/

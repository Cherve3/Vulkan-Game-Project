
#include "simple_logger.h"

#include "gf3d_entity.h"

typedef struct
{
	Entity *entity_list;	/**<List of entities*/
	Uint32 entity_count;	/**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager gf3d_entity = { 0 };

void gf3d_entity_free(Entity *entity);

void gf3d_entity_close()
{
	int i;
	if (gf3d_entity.entity_list != NULL)
	{
		for (i = 0; i < gf3d_entity.entity_count; i++)
		{
			gf3d_entity_free(&gf3d_entity.entity_list[i]);
		}
		free(gf3d_entity.entity_list);
	}
	memset(&gf3d_entity, 0, sizeof(EntityManager));
	slog("Entity System Closed");
}

void gf3d_entity_init(Uint32 maxEntities)
{
	if (gf3d_entity.entity_list != NULL)
	{
		slog("WARNING: entity system already initialized");
		return;
	}
	gf3d_entity.entity_list = gfc_allocate_array(sizeof(Entity), maxEntities);
	if (!gf3d_entity.entity_list)
	{
		slog("failed to allocate entity list");
		return;
	}
	gf3d_entity.entity_count = maxEntities;
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
	for(i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse)
		{
			gf3d_entity.entity_list[i]._inuse = 1;
			gfc_matrix_identity(gf3d_entity.entity_list[i].modelMatrix);
			return &gf3d_entity.entity_list[i];
		}
	}
	slog("Failed to provide new entity, no unused slots");
	return NULL;
}

void gf3d_entity_update(Entity *self){
	if (!self) return;
	if (!self->update) return;
	self->update(self);
	
}

void gf3d_entity_update_all()
{
	int i;
	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_update(&gf3d_entity.entity_list[i]);
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

void gf3d_entity_damage(Entity *self)
{
	if (!self)return;
	if (!self->damage)return; // No damage function
	self->damage(self);
}

void gf3d_entity_die(Entity *self)
{
	if (!self)return;
	if (!self->die)return; // No die function
	self->die(self);
}

void gf3d_entity_think_all()
{
	int i;
	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_think(&gf3d_entity.entity_list[i]);
	}
}

void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self) return;
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}

void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;
	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		gf3d_entity_draw(&gf3d_entity.entity_list[i], bufferFrame, commandBuffer);
	}
}

void gf3d_entity_collision_test(Entity *self){
	int i;

	for (i = 0; i < gf3d_entity.entity_count; i++)
	{
		
		if (!gf3d_entity.entity_list[i]._inuse) continue;
		if (&gf3d_entity.entity_list[i] == self) continue;
		if (&gf3d_entity.entity_list[i] == self->parent) continue;

		if ( ((self->boxCollider.x - self->boxCollider.width) <= (gf3d_entity.entity_list[i].boxCollider.x + gf3d_entity.entity_list[i].boxCollider.width)) &&
			 ((self->boxCollider.x + self->boxCollider.width) >= (gf3d_entity.entity_list[i].boxCollider.x - gf3d_entity.entity_list[i].boxCollider.width)) &&
		     ((self->boxCollider.y - self->boxCollider.height) <= (gf3d_entity.entity_list[i].boxCollider.y + gf3d_entity.entity_list[i].boxCollider.height)) &&
			 ((self->boxCollider.y + self->boxCollider.height) >= (gf3d_entity.entity_list[i].boxCollider.y - gf3d_entity.entity_list[i].boxCollider.height)) &&
		     ((self->boxCollider.z - self->boxCollider.depth) <= (gf3d_entity.entity_list[i].boxCollider.z + gf3d_entity.entity_list[i].boxCollider.depth)) &&
			 ((self->boxCollider.z + self->boxCollider.depth) >= (gf3d_entity.entity_list[i].boxCollider.z - gf3d_entity.entity_list[i].boxCollider.depth)) )
		{
			//Check X
			if ( (self->boxCollider.x - self->boxCollider.width) <= (gf3d_entity.entity_list[i].boxCollider.x + gf3d_entity.entity_list[i].boxCollider.width) &&
			   ( (self->boxCollider.x - self->boxCollider.width) >= (gf3d_entity.entity_list[i].boxCollider.x - gf3d_entity.entity_list[i].boxCollider.width)) && gf3d_entity.entity_list[i].name != "World")
			{
				self->position.x += (gf3d_entity.entity_list[i].boxCollider.x + gf3d_entity.entity_list[i].boxCollider.width) - (self->boxCollider.x - self->boxCollider.width);
			}
			else if ( (self->boxCollider.x + self->boxCollider.width) <= (gf3d_entity.entity_list[i].boxCollider.x + gf3d_entity.entity_list[i].boxCollider.width) &&
				( (self->boxCollider.x + self->boxCollider.width) >= (gf3d_entity.entity_list[i].boxCollider.x - gf3d_entity.entity_list[i].boxCollider.width)) && gf3d_entity.entity_list[i].name != "World")
			{

				self->position.x += (gf3d_entity.entity_list[i].boxCollider.x - gf3d_entity.entity_list[i].boxCollider.width) - (self->boxCollider.x + self->boxCollider.width);
			}

			//Check Y
			if (((self->boxCollider.y - self->boxCollider.height) <= (gf3d_entity.entity_list[i].boxCollider.y + gf3d_entity.entity_list[i].boxCollider.height)) &&
				((self->boxCollider.y + self->boxCollider.height) >= (gf3d_entity.entity_list[i].boxCollider.y - gf3d_entity.entity_list[i].boxCollider.height)) && gf3d_entity.entity_list[i].name == "World")
			{
				self->position.y += (gf3d_entity.entity_list[i].boxCollider.y + gf3d_entity.entity_list[i].boxCollider.height) - (self->boxCollider.y - self->boxCollider.height);
			}

			//Check Z
			if ((self->boxCollider.z - self->boxCollider.depth) <= (gf3d_entity.entity_list[i].boxCollider.z + gf3d_entity.entity_list[i].boxCollider.depth) &&
				((self->boxCollider.z - self->boxCollider.depth) >= (gf3d_entity.entity_list[i].boxCollider.z - gf3d_entity.entity_list[i].boxCollider.depth)) && gf3d_entity.entity_list[i].name != "World")
			{
				self->position.z += (gf3d_entity.entity_list[i].boxCollider.z + gf3d_entity.entity_list[i].boxCollider.depth) - (self->boxCollider.z - self->boxCollider.depth);
			}
			else if ((self->boxCollider.z + self->boxCollider.depth) <= (gf3d_entity.entity_list[i].boxCollider.z + gf3d_entity.entity_list[i].boxCollider.depth) &&
				((self->boxCollider.z + self->boxCollider.depth) >= (gf3d_entity.entity_list[i].boxCollider.z - gf3d_entity.entity_list[i].boxCollider.depth)) && gf3d_entity.entity_list[i].name != "World")
			{

				self->position.z += (gf3d_entity.entity_list[i].boxCollider.z - gf3d_entity.entity_list[i].boxCollider.depth) - (self->boxCollider.z + self->boxCollider.depth);
			}
		}
	}
}

Entity* gf3d_get_entity_list()
{	
	return gf3d_entity.entity_list;
}

int gf3d_get_entity_list_count()
{
	return gf3d_entity.entity_count;
}

/*eol@eof*/

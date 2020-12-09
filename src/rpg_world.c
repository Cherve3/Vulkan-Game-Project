#include "rpg_world.h"

static World world = { 0 };

void rpg_world_init()
{
	slog("Initializing world");
	world.ground = gf3d_entity_new();


	if (world.ground)
	{
		world.ground->model = gf3d_model_load("world");
		world.ground->name = "World";
		world.ground->position = vector3d(0, 0, 0);
		gfc_matrix_new_translation(world.ground->modelMatrix, world.ground->position);
		world.ground->boxCollider.depth = 1000.0;
		world.ground->boxCollider.height = 0;
		world.ground->boxCollider.width = 1000.0;
		world.ground->boxCollider.x = world.ground->position.x;
		world.ground->boxCollider.y = world.ground->position.y;
		world.ground->boxCollider.z = world.ground->position.z;
	}
	else
	{
		slog("World ground is null");
		return;
	}

	if (!water_init(vector3d(0, -4, 0)))
	{
		slog("Water is null");
		return;
	}
}

int water_init(Vector3D position)
{
	slog("Initializing Water");
	world.water = gf3d_entity_new();
	if (world.water)
	{
		world.water->model = gf3d_model_load("water");
		world.water->position = position;
		world.water->name = "Water";
		gfc_matrix_new_translation(world.water->modelMatrix, world.water->position);
		return 1;
	}

	return 0;
}

void buildings_init(Uint32 building_count, Vector3D position)
{

}

void trees_init(Uint32 tree_count)
{

}

/*eol@eof*/

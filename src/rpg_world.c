#include "rpg_world.h"
#include "simple_json.h"

static World world = { 0 };

static SJson *world_info = NULL;
static SJson *ground_info = NULL;
static SJson *water_info = NULL;
static SJson *building_info = NULL;
static SJson *tree_info = NULL;

void rpg_world_init()
{
	slog("Initializing world");
	sj_enable_debug();
	world_info = sj_load("world.json");
	slog("%s", sj_get_error());
	if (!world_info)
	{
		slog("World json not loaded");
		slog("%s", sj_get_error());
		exit(0);
	}
	ground_info		= sj_object_get_value(world_info, "Ground");
	if (!ground_info)
	{
		slog("Ground not found in World json");
		exit(0);
	}
	water_info		= sj_object_get_value(world_info, "Water");
	if (!water_info)
	{
		slog("Water not found in World json");
		exit(0);
	}
	building_info	= sj_object_get_value(world_info, "Buildings");
	if (!building_info)
	{
		slog("Buildings not found in World json");
		exit(0);
	}
	tree_info		= sj_object_get_value(world_info, "Trees");
	if (!tree_info)
	{
		slog("Trees not found in World json");
		exit(0);
	}


	world.ground = gf3d_entity_new();
	if (world.ground)
	{
		world.ground->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(ground_info, "model") ));
		world.ground->name = sj_get_string_value(sj_object_get_value(ground_info, "name"));
		world.ground->position = vector3d(0, 0, 0);
		world.ground->type = WORLD;
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
		slog("Ground entity is null");
		return;
	}

	if (!water_init(vector3d(0, -4, 0)))
	{
		slog("Water is null");
		return;
	}

	if ( !buildings_init( 10, vector3d(100, 26, 100) ) )
	{
		slog("Building is null");
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
		world.water->type = WORLD;
		gfc_matrix_new_translation(world.water->modelMatrix, world.water->position);
		return 1;
	}

	return 0;
}

int buildings_init(Uint32 building_count, Vector3D position)
{
	world.buildings = gfc_allocate_array(sizeof(Entity), building_count);
	world.buildings[0] = gf3d_entity_new();
	if (world.buildings[0])
	{
		world.buildings[0]->model = gf3d_model_load("house1");
		world.buildings[0]->name = "House 0";
		world.buildings[0]->type = STRUCTURE;
		world.buildings[0]->position = position;
		world.buildings[0]->boxCollider.x = position.x;
		world.buildings[0]->boxCollider.y = position.y;
		world.buildings[0]->boxCollider.z = position.z;
		world.buildings[0]->boxCollider.width = 26;
		world.buildings[0]->boxCollider.height = 26;
		world.buildings[0]->boxCollider.depth = 26;
		gfc_matrix_new_translation(world.buildings[0]->modelMatrix, world.buildings[0]->position);
		return 1;
	}
	return 0;
}

void trees_init(Uint32 tree_count)
{

}

/*eol@eof*/

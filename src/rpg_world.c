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

	world_info = sj_load("json/world.json");
	if (!world_info)
	{
		slog("World json not loaded");
		slog("%s", sj_get_error());
		exit(0);
	}
	ground_info	= sj_object_get_value(world_info, "Ground");
	if (!ground_info)
	{
		slog("Ground not found in World json");
		exit(0);
	}
	water_info = sj_object_get_value(world_info, "Water");
	if (!water_info)
	{
		slog("Water not found in World json");
		exit(0);
	}
	building_info = sj_object_get_value(world_info, "Buildings");
	if (!building_info)
	{
		slog("Buildings not found in World json");
		exit(0);
	}
	tree_info = sj_object_get_value(world_info, "Trees");
	if (!tree_info)
	{
		slog("Trees not found in World json");
		exit(0);
	}

	world.ground = gf3d_entity_new();
	if (world.ground)
	{
		world.ground->model = gf3d_model_load( sj_get_string_value(sj_object_get_value(ground_info, "model")));
		world.ground->name = "World";//sj_get_string_value(sj_object_get_value(ground_info, "name"));

		sj_get_float_value(sj_array_get_nth(sj_object_get_value(ground_info, "position"), 0), &world.ground->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(ground_info, "position"), 1), &world.ground->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(ground_info, "position"), 2), &world.ground->position.z);
		
		sj_get_integer_value(sj_object_get_value(ground_info, "type"), &world.ground->type);

		gfc_matrix_make_translation(world.ground->modelMatrix, world.ground->position);
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

	if (!water_init())
	{
		slog("Water is null");
		return;
	}

	if ( !buildings_init(10) )
	{
		slog("Building is null");
			return;
	}
	/*
	if (!trees_init(10))
	{
		slog("tree is null");
		return;
	}*/
}

int water_init()
{
	slog("Initializing Water");
	world.water = gf3d_entity_new();
	if (world.water)
	{
		world.water->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(water_info, "model")));
		world.water->name = sj_get_string_value(sj_object_get_value(water_info, "name"));

		sj_get_float_value(sj_array_get_nth(sj_object_get_value(water_info, "position"), 0), &world.water->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(water_info, "position"), 1), &world.water->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(water_info, "position"), 2), &world.water->position.z);

		sj_get_integer_value(sj_object_get_value(water_info, "type"), &world.water->type);
		gfc_matrix_make_translation(world.water->modelMatrix, world.water->position);
		return 1;
	}

	return 0;
}

int buildings_init(Uint32 building_count)
{
	world.buildings = gfc_allocate_array(sizeof(Entity), building_count);

	world.buildings[0] = gf3d_entity_new();
	if (world.buildings[0])
	{
		SJson *house1 = sj_object_get_value(building_info, "house1");
		world.buildings[0]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(house1, "model")));
		world.buildings[0]->name = sj_get_string_value(sj_object_get_value(house1, "name"));;
		sj_get_integer_value(sj_object_get_value(house1, "type"), &world.buildings[0]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house1, "position"), 0), &world.buildings[0]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house1, "position"), 1), &world.buildings[0]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house1, "position"), 2), &world.buildings[0]->position.z);
		world.buildings[0]->boxCollider.x = world.buildings[0]->position.x;
		world.buildings[0]->boxCollider.y = world.buildings[0]->position.y;
		world.buildings[0]->boxCollider.z = world.buildings[0]->position.z;
		world.buildings[0]->boxCollider.width = 26;
		world.buildings[0]->boxCollider.height = 26;
		world.buildings[0]->boxCollider.depth = 26;
		gfc_matrix_make_translation(world.buildings[0]->modelMatrix, world.buildings[0]->position);
	}

	world.buildings[1] = gf3d_entity_new();
	if (world.buildings[1])
	{
		SJson *house2 = sj_object_get_value(building_info, "house2");
		world.buildings[1]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(house2, "model")));
		world.buildings[1]->name = sj_get_string_value(sj_object_get_value(house2, "name"));;
		sj_get_integer_value(sj_object_get_value(house2, "type"), &world.buildings[1]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house2, "position"), 0), &world.buildings[1]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house2, "position"), 1), &world.buildings[1]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house2, "position"), 2), &world.buildings[1]->position.z);
		world.buildings[1]->boxCollider.x = world.buildings[1]->position.x;
		world.buildings[1]->boxCollider.y = world.buildings[1]->position.y;
		world.buildings[1]->boxCollider.z = world.buildings[1]->position.z;
		world.buildings[1]->boxCollider.width = 25;
		world.buildings[1]->boxCollider.height = 25;
		world.buildings[1]->boxCollider.depth = 25;
		gfc_matrix_make_translation(world.buildings[1]->modelMatrix, world.buildings[1]->position);
	}

	world.buildings[2] = gf3d_entity_new();
	if (world.buildings[2])
	{
		SJson *house3 = sj_object_get_value(building_info, "house3");
		world.buildings[2]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(house3, "model")));
		world.buildings[2]->name = sj_get_string_value(sj_object_get_value(house3, "name"));;
		sj_get_integer_value(sj_object_get_value(house3, "type"), &world.buildings[2]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house3, "position"), 0), &world.buildings[2]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house3, "position"), 1), &world.buildings[2]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house3, "position"), 2), &world.buildings[2]->position.z);
		world.buildings[2]->boxCollider.x = world.buildings[2]->position.x;
		world.buildings[2]->boxCollider.y = world.buildings[2]->position.y;
		world.buildings[2]->boxCollider.z = world.buildings[2]->position.z;
		world.buildings[2]->boxCollider.width = 25;
		world.buildings[2]->boxCollider.height = 25;
		world.buildings[2]->boxCollider.depth = 25;
		gfc_matrix_make_translation(world.buildings[2]->modelMatrix, world.buildings[2]->position);
	}

	world.buildings[3] = gf3d_entity_new();
	if (world.buildings[3])
	{
		SJson *house4 = sj_object_get_value(building_info, "house4");
		world.buildings[3]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(house4, "model")));
		world.buildings[3]->name = sj_get_string_value(sj_object_get_value(house4, "name"));;
		sj_get_integer_value(sj_object_get_value(house4, "type"), &world.buildings[3]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house4, "position"), 0), &world.buildings[3]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house4, "position"), 1), &world.buildings[3]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(house4, "position"), 2), &world.buildings[3]->position.z);
		world.buildings[3]->boxCollider.x = world.buildings[3]->position.x;
		world.buildings[3]->boxCollider.y = world.buildings[3]->position.y;
		world.buildings[3]->boxCollider.z = world.buildings[3]->position.z;
		world.buildings[3]->boxCollider.width = 25;
		world.buildings[3]->boxCollider.height = 25;
		world.buildings[3]->boxCollider.depth = 25;
		gfc_matrix_make_translation(world.buildings[3]->modelMatrix, world.buildings[3]->position);
	}
	/*
	world.buildings[4] = gf3d_entity_new();
	if (world.buildings[4])
	{
		SJson *well = sj_object_get_value(building_info, "well");
		world.buildings[4]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(well, "model")));
		world.buildings[4]->name = sj_get_string_value(sj_object_get_value(well, "name"));;
		sj_get_integer_value(sj_object_get_value(well, "type"), &world.buildings[4]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(well, "position"), 0), &world.buildings[4]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(well, "position"), 1), &world.buildings[4]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(well, "position"), 2), &world.buildings[4]->position.z);
		world.buildings[4]->boxCollider.x = world.buildings[4]->position.x;
		world.buildings[4]->boxCollider.y = world.buildings[4]->position.y;
		world.buildings[4]->boxCollider.z = world.buildings[4]->position.z;
		world.buildings[4]->boxCollider.width = 10;
		world.buildings[4]->boxCollider.height = 10;
		world.buildings[4]->boxCollider.depth = 10;
		gfc_matrix_new_translation(world.buildings[4]->modelMatrix, world.buildings[4]->position);
	}
	*/
	world.buildings[5] = gf3d_entity_new();
	if (world.buildings[5])
	{
		SJson *goblinbase = sj_object_get_value(building_info, "goblinbase");
		world.buildings[5]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(goblinbase, "model")));
		world.buildings[5]->name = sj_get_string_value(sj_object_get_value(goblinbase, "name"));;
		sj_get_integer_value(sj_object_get_value(goblinbase, "type"), &world.buildings[5]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(goblinbase, "position"), 0), &world.buildings[5]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(goblinbase, "position"), 1), &world.buildings[5]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(goblinbase, "position"), 2), &world.buildings[5]->position.z);
		world.buildings[5]->boxCollider.x = world.buildings[5]->position.x;
		world.buildings[5]->boxCollider.y = world.buildings[5]->position.y;
		world.buildings[5]->boxCollider.z = world.buildings[5]->position.z;

		gfc_matrix_make_translation(world.buildings[5]->modelMatrix, world.buildings[5]->position);
		return 1;
	}
	return 0;
}

int trees_init(Uint32 tree_count)
{
	world.trees = gfc_allocate_array(sizeof(Entity), tree_count);

	world.trees[0] = gf3d_entity_new();
	if (world.trees[0])
	{
		SJson *tree = sj_object_get_value(tree_info, "tree1");
		world.trees[0]->model = gf3d_model_load(sj_get_string_value(sj_object_get_value(tree, "model")));
		world.trees[0]->name = sj_get_string_value(sj_object_get_value(tree, "name"));;
		sj_get_integer_value(sj_object_get_value(tree, "type"), &world.trees[0]->type);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(tree, "position"), 0), &world.trees[0]->position.x);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(tree, "position"), 1), &world.trees[0]->position.y);
		sj_get_float_value(sj_array_get_nth(sj_object_get_value(tree, "position"), 2), &world.trees[0]->position.z);
		world.trees[0]->boxCollider.x = world.trees[0]->position.x;
		world.trees[0]->boxCollider.y = world.trees[0]->position.y;
		world.trees[0]->boxCollider.z = world.trees[0]->position.z;
		world.trees[0]->boxCollider.width = 3;
		world.trees[0]->boxCollider.height = 3;
		world.trees[5]->boxCollider.depth = 3;
		gfc_matrix_make_translation(world.trees[0]->modelMatrix, world.trees[0]->position);
		return 1;
	}
	return 0;
}

/*eol@eof*/

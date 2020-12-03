#include "game.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
	Uint32 old_time, time;
	unsigned int lastTime = 0, currentTime;
	float deltaTime = 0;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
	Bool toggleStats = false;

	Entity *world = NULL;
	Chest *chest = NULL;
	Entity *water = NULL;
	Entity *wood = NULL;
	Entity *fireball = NULL;



    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }

    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        720,                   //screen width
        480,                    //screen height
        vector4d(0.3,0.75,.5,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

    // main game loop
    slog("gf3d main loop begin");

	gf3d_camera_init();

	gf3d_entity_init(1024);

	rpg_ui_init();
	
	rpg_chests_init(10);

	rpg_projectile_init(10);
	
	rpg_player_init();

	wood		= gf3d_entity_new();
	world		= gf3d_entity_new();
	chest		= rpg_chest_new();
	
	water = gf3d_entity_new();
	
	rpg_goblin_init(GoblinGrunt,vector3d(-10, 5, -10));
	rpg_goblin_init(GoblinHeavy, vector3d(10, 7, -10));
	rpg_goblin_init(GoblinArcher, vector3d(10, 5, 10));
	rpg_goblin_init(GoblinKing, vector3d(-40, 25, 40));

	rpg_npc_init(ItemShop, vector3d(20, 8, -20));

	slog_sync();
	if (world)
	{
		world->model = gf3d_model_load("world");
		world->name = "World";
		world->position = vector3d(0, 0, 0);
		gfc_matrix_new_translation(world->modelMatrix, world->position);
		world->boxCollider.depth = 1000.0;
		world->boxCollider.height = 0;
		world->boxCollider.width = 1000.0;
		world->boxCollider.x = world->position.x;
		world->boxCollider.y = world->position.y;
		world->boxCollider.z = world->position.z;
		
	}
	if (water)
	{
		water->model = gf3d_model_load("water");
		water->position = vector3d(0, -4, 0);
		water->name = "Water";
		gfc_matrix_new_translation(water->modelMatrix, water->position);
	}
	if (wood)
	{
		wood->model = gf3d_model_load("log");
		wood->position = vector3d(-5, 1, 5);
		wood->name = "Wood";
		gfc_matrix_new_translation(wood->modelMatrix,wood->position);
	}

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);


	//Game Loop
	while (!done)
	{
		
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 1000) {
			//slog("\ncurrent time: %i\n", currentTime);
			//slog("\nlast time: %i\n", lastTime);
			lastTime = currentTime;
		}
		// Time
		old_time = time;
		time = SDL_GetTicks();
		deltaTime = ((float)(time - old_time) / 1000);
		

		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		//update game things here
		
		gf3d_entity_think_all();
		gf3d_entity_update_all();

		// configure render command for graphics command pool
		// for each mesh, get a command and configure it from the pool
		bufferFrame = gf3d_vgraphics_render_begin();
		gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_model_pipeline(), bufferFrame);
		gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_overlay_pipeline(), bufferFrame);

		//Model Buffer
		commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_model_pipeline());
		gf3d_entity_draw_all(bufferFrame, commandBuffer);
		gf3d_command_rendering_end(commandBuffer);

		//Sprite Buffer
			commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_overlay_pipeline());

				rpg_ui_draw_all(bufferFrame,commandBuffer);

			gf3d_command_rendering_end(commandBuffer);

        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    //cleanup
	SDL_Quit();
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/

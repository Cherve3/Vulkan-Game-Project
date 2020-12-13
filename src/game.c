#include "game.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
	Uint32 old_time, time;
	float frame = 0;
	unsigned int lastTime = 0, currentTime;
	float deltaTime = 0;
    Uint8 validate = 1;
	Matrix4 modelMat;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
	Bool toggleStats = false;

	Chest *chest = NULL;
	Item *wood = NULL;
	Item *arrow = NULL;
	Item *potion = NULL;
//	Model *model = NULL;

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

	rpg_world_init();

	rpg_player_init();

	rpg_npc_init(ItemShop, vector3d(20, 8, -20));

	rpg_goblin_init();
	rpg_goblin_spawn(GoblinGrunt, vector3d(-10, 5, -10));
	rpg_goblin_spawn(GoblinHeavy, vector3d(10, 5, -10));
	rpg_goblin_spawn(GoblinArcher, vector3d(10, 5, 10));
	rpg_goblin_spawn(GoblinKing, vector3d(-40, 25, 40));

	rpg_item_entity_init(10);
	wood = rpg_item_new(material,"Wood Log",vector3d(-5, 2, 20));
	potion = rpg_item_new(consumable, "Health Potion", vector3d(-5, 2, 30));
	arrow = rpg_item_new(consumable, "Arrow", vector3d(-5, 2, 50));

	rpg_chests_init(10);
	chest = rpg_chest_new();

	rpg_projectile_init(10);

	rpg_ui_init();


	slog_sync();

//	gfc_matrix_identity(modelMat);
//	model = gf3d_model_load_animated("goblinking", 1, 5);

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	//Game Loop
	slog("Game Loop Begin");
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

		frame = frame + 0.5;
		if (frame >= 24)frame = 0;

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
//			gf3d_model_draw_anim(model, bufferFrame, commandBuffer, modelMat, (Uint32)frame);
			gf3d_entity_draw_all(bufferFrame, commandBuffer);
			
		gf3d_command_rendering_end(commandBuffer);

		//Sprite Buffer
			commandBuffer = gf3d_command_rendering_begin(bufferFrame, gf3d_vgraphics_get_graphics_overlay_pipeline());

				rpg_ui_update();
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

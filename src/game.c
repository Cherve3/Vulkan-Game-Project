#include "game.h"

int main(int argc,char *argv[])
{
    int done = 0;
	int start = 0;
	int charchoice = 0;
    int a;
	Uint32 old_time = 0, time = 0;
	float frame = 0;
	float frame2 = 0;
	unsigned int lastTime = 0, currentTime;
	float deltaTime = 0;
    Uint8 validate = 1;
	Matrix4D modelMat;
	Matrix4D modelMat2, modelMat3, modelMat4, modelMat5, modelMat6, modelMat7;
    const Uint8 * keys;
	int x, y;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

	SDL_Event event;
	SDL_Renderer *renderer	= NULL;
	SDL_Texture *menu		= NULL;
	SDL_Texture *char_create = NULL;
	char* model_name		= NULL;
	SDL_Texture *loading	= NULL;
	SDL_Texture *load_icon  = NULL;
	SDL_Rect start_rect;
	SDL_Rect quit_rect;
	SDL_Rect load_rect;

	char* path = "D:/Git/Projects/Vulkan-Game-Project/";
	char file_path[60];

	Bool toggleStats = false;

	Chest *chest = NULL;
	Item *wood = NULL;
	Item *arrow = NULL;
	Item *potion = NULL;
	Item *woodsword = NULL;

	Model *model = NULL;
	Model *dino = NULL;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }

    init_logger("gf3d.log", true);    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        720,                   //screen width
        480,                    //screen height
		vector4d_create(0.3,0.75,.5,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();
	slog("initializing menu");
	// Main Menu
	renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!renderer){ slog("Renderer not created."); SDL_Quit(); exit(0); }

	snprintf(file_path,sizeof(file_path), "%s%s", path, "images/mainmenu.png");

	menu = IMG_LoadTexture(renderer, file_path);
	if (!menu){ slog("Main menu texture not created."); SDL_Quit(); exit(0); }

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,menu,NULL,NULL);

	VkExtent2D extent = gf3d_vgraphics_get_view_extent();
	keys = SDL_GetKeyboardState(NULL);

	slog("initializing menu");
	start_rect.h = 80;
	start_rect.w = 150;
	start_rect.x = 250;//100 * 2 / (float)extent.width;
	start_rect.y = 230;// 70 * 2 / (float)extent.height;

	quit_rect.h = 80;
	quit_rect.w = 150;
	quit_rect.x = 250;//100 * 2 / (float)extent.width;
	quit_rect.y = 350;//100 * 2 / (float)extent.height;
	
	slog("Main Menu loaded");
	while (!start)
	{
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEMOTION)
			{
				x = event.motion.x;
				y = event.motion.y;

				if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
					(y > start_rect.y) && (y < start_rect.y + start_rect.h))
				{
					slog("Hovering over start");
				}

				if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
					(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
				{
					slog("Hovering over Quit");
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				x = event.button.x;
				y = event.button.y;

				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
						(y > start_rect.y) && (y < start_rect.y + start_rect.h))
					{
						slog("Pressed Start button");
						start = 1;

						snprintf(file_path, sizeof(file_path), "%s%s", path, "images/charchoice.png");

						char_create = IMG_LoadTexture(renderer, file_path);
						if (!char_create){ slog("Character choice texture not created."); SDL_Quit(); exit(0); }

						snprintf(file_path, sizeof(file_path), "%s%s", path, "images/load_icon.png");
						load_icon = IMG_LoadTexture(renderer, file_path);

						start_rect.h = 80;
						start_rect.w = 150;
						start_rect.x = 250;//100 * 2 / (float)extent.width;
						start_rect.y = 230;// 70 * 2 / (float)extent.height;

						quit_rect.h = 80;
						quit_rect.w = 150;
						quit_rect.x = 250;//100 * 2 / (float)extent.width;
						quit_rect.y = 350;//100 * 2 / (float)extent.height;

						SDL_RenderClear(renderer);
						
						SDL_RenderCopy(renderer, char_create, NULL, NULL);


						SDL_RenderPresent(renderer);

					}

					if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
						(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
					{
						slog("Pressed Quit button");
						SDL_DestroyRenderer(renderer);
						SDL_DestroyTexture(menu);
						done = 1;
						SDL_Quit();
						return 0;
					}
				}
			}
		}
	}

	while (!charchoice)
	{
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEMOTION)
			{
				x = event.motion.x;
				y = event.motion.y;

				if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
					(y > start_rect.y) && (y < start_rect.y + start_rect.h))
				{
					slog("Hovering over start");
				}

				if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
					(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
				{
					slog("Hovering over Quit");
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				x = event.button.x;
				y = event.button.y;

				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if ((x > start_rect.x) && (x < start_rect.x + start_rect.w) &&
						(y > start_rect.y) && (y < start_rect.y + start_rect.h))
					{
						slog("Pressed Knight button");
						charchoice = 1;
						model_name = "player";
					}
					else if ((x > quit_rect.x) && (x < quit_rect.x + quit_rect.w) &&
						(y > quit_rect.y) && (y < quit_rect.y + quit_rect.h))
					{
						slog("Pressed Mage button");
						charchoice = 1;
						model_name = "dino";
					}

					snprintf(file_path, sizeof(file_path), "%s%s", path, "images/loading.png");
					loading = IMG_LoadTexture(renderer, file_path);
					if (!loading) { slog("Loading texture not created."); SDL_Quit(); exit(0); }

					load_icon = IMG_LoadTexture(renderer, "images/load_icon.png");

					load_rect.h = 150;
					load_rect.w = 150;
					load_rect.x = (extent.width - load_rect.w) / 2;
					load_rect.y = (extent.width - load_rect.h) / 2;

					SDL_RenderClear(renderer);

					SDL_RenderCopy(renderer, loading, NULL, NULL);

					SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 1, NULL, SDL_FLIP_NONE);
					SDL_RenderPresent(renderer);
				}
			}
		}
	}
    // main game loop
    slog("gf3d main loop begin");

	gf3d_camera_init();

	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 36, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);

	gf3d_entity_init(1024);

	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 72, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);

	rpg_world_init();

	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 108, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	
	rpg_player_init(model_name);
	
	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 144, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);

//	rpg_npc_init();
//	rpg_npc_spawn(ItemShop, vector3d(20, 8, -20));
//	rpg_npc_spawn(WeaponShop, vector3d(90, 5, -40));
//	rpg_npc_spawn(ArmorShop, vector3d(90, 5.5, -60));
//	rpg_npc_spawn(SpellShop, vector3d(60, 9.3, -20));
//	rpg_npc_spawn(Generic, vector3d(100, 8.3, -80));
//	rpg_npc_spawn(Questgiver, vector3d(90, 8.3, -20));

//	print_npc_stats(rpg_get_npc()[0]);
	
	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 216, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
//
//	rpg_goblin_init();
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-420, 5, -505));
//	rpg_goblin_spawn(GoblinHeavy, vector3d(-420, 5, -510));
//	rpg_goblin_spawn(GoblinArcher, vector3d(-420, 5, -515));
//	rpg_goblin_spawn(GoblinKing, vector3d(-410, 25, -550));

	// For quest 2
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -205));
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -210));
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -215));
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-210, 5, -225));
//	rpg_goblin_spawn(GoblinGrunt, vector3d(-210, 5, -205));
	
	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 252, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	
	rpg_item_entity_init(10);
	potion		= rpg_item_new(consumable, "Health Potion", vector3d_create(-5, 2, 30));
	arrow		= rpg_item_new(consumable, "Arrow", vector3d_create(-5, 2, 50));
	woodsword	= rpg_item_new(weapon, "Wooden Sword", vector3d_create(-5, 2, 70));
	
	rpg_quest_init();

	rpg_main_quests_init();
	
	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 288, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	
	rpg_chests_init(10);
	chest = rpg_chest_new();

	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 324, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	
	rpg_projectile_init(10);

	rpg_ui_init();

	slog_sync();
	
	matrix4d_identity(modelMat);
	matrix4d_identity(modelMat2);
	matrix4d_identity(modelMat3);
	matrix4d_identity(modelMat4);
	matrix4d_identity(modelMat5);
	matrix4d_identity(modelMat6);
	matrix4d_identity(modelMat7);

	model = gf3d_model_load("dino");
	dino = gf3d_model_load("dino");
	matrix4d_translate(vector3d_create(0,0,0),modelMat);
	matrix4d_translate(vector3d_create(10, 0, 0), modelMat2);
	matrix4d_translate(vector3d_create(0, 10, 0), modelMat3);
	matrix4d_translate(vector3d_create(0, 0, 10), modelMat4);
	matrix4d_translate(vector3d_create(-10, 0, 0), modelMat5);
	matrix4d_translate(vector3d_create(0, -10, 0), modelMat6);
	matrix4d_translate(vector3d_create(0, 0, -10), modelMat7);
	
//		get_player()->ent->model = gf3d_model_load_animated("player", 1,19);

	SDL_RenderCopyEx(renderer, load_icon, NULL, &load_rect, 360, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
	
	SDL_ShowCursor(0);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	
	SDL_DestroyTexture(menu);
	SDL_DestroyTexture(loading);
	
	SDL_DestroyRenderer(renderer);
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

		frame = frame + 0.1;
		if (frame >= 19)frame = 5;

		frame2 = frame2 + 0.2;
		if (frame2 >= 19)frame2 = 1;

		
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

			gf3d_model_draw(dino, bufferFrame, commandBuffer,modelMat);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat2);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat3);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat4);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat5);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat6);
			gf3d_model_draw(dino, bufferFrame, commandBuffer, modelMat7);
			if (get_player()->ent->animated)
				gf3d_model_draw_anim(get_player_entity()->model, bufferFrame, commandBuffer, get_player_entity()->modelMatrix, frame2);

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

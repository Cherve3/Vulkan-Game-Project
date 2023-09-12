
#include <string.h>

#include <SDL.h>            
#include <SDL_vulkan.h>
#include <SDL_image.h>

#include "game.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"
#include "gf3d_sprite.h"

#include "rpg_world.h"
#include "rpg_projectile.h"
#include "rpg_goblin.h"
#include "rpg_player.h"
#include "rpg_npc.h"
#include "rpg_input.h"
#include "rpg_quest.h"
#include "rpg_items.h"
#include "rpg_chests.h"
#include "rpg_ui.h"

char *FILE_PATH;

int quit() 
{
	vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
	//cleanup
	
	SDL_Quit();
	slog_sync();
	if (FILE_PATH)
	{
		free(FILE_PATH);
	}
	slog("gf3d program end");
	return 0;
}

int main(int argc,char *argv[])
{
    int done = 0;
	int main_menu = 0;
    
	Uint32 old_time = 0, time = 0;
	float frame = 0;
	float frame2 = 0;
	unsigned int lastTime = 0, currentTime;
	float deltaTime = 0;
    Uint8 validate = 1;
	const Uint8* keys;

	Matrix4D modelMat;
	Matrix4D modelMat2;

	SDL_Event event;

	SDL_Texture *char_create = NULL;

	int file_path_length = 0;
	int argv_length = strlen(argv[0]);

	/**
	 *	Traverses by starting at the end of argv[0] and getting the position of the first
	 *	'\' because the file path has the exe file name which is not needed for the access
	 *	of files in other classes.
	 */
	for (int i = argv_length -1; i > 0; i--)
	{
		if (argv[0][i] == '/' || argv[0][i] == '\\')
		{
			file_path_length = i;
			FILE_PATH = (char*)malloc((file_path_length + 1) * sizeof(char*));
			if (FILE_PATH)
			{
				strncpy(FILE_PATH, argv[0], file_path_length +1);
				FILE_PATH[file_path_length + 1] = '\0';
			}
			else
			{
				slog("Cannot determine the file path for the program.");
				return quit();
			}
			break;
		}
	}

	Bool toggleStats = false;

	Chest *chest = NULL;
	Item *wood = NULL;
	Item *arrow = NULL;
	Item *potion = NULL;
	Item *woodsword = NULL;

	Model *model = NULL;
	Model *dino = NULL;

	int i;
    for (i = 1; i < argc;i++)
    {
        if (strcmp(argv[i],"-disable_validate") == 0)
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
	
	rpg_main_menu_init();
	main_menu = rpg_main_menu_load_screen(StartScreen);
	if (main_menu == -1)
	{
		return quit();
	}
	
	main_menu = rpg_main_menu_load_screen(CharacterSelectScreen);

	SDL_SetRelativeMouseMode(SDL_TRUE); // DO NOT move this it crashes moved below and is useless above.

    slog("gf3d loading game begin");
	
	// Called multiple times to reload the rotated loading texture.
	rpg_update_loading_texture(36, SDL_FLIP_NONE);

	gf3d_entity_init(1024);

	rpg_update_loading_texture(72, SDL_FLIP_NONE);

	rpg_world_init();

	rpg_update_loading_texture(108, SDL_FLIP_NONE);
	
	rpg_player_init();
	gf3d_camera_init();

	rpg_update_loading_texture(144, SDL_FLIP_NONE);

	rpg_npc_init();
	//rpg_npc_spawn(ItemShop, vector3d_create(0, -200, 0));
	//rpg_npc_spawn(WeaponShop, vector3d_create(90, 5, -40));
	//rpg_npc_spawn(ArmorShop, vector3d_create(90, 5.5, -60));
	//rpg_npc_spawn(SpellShop, vector3d_create(60, 9.3, -20));
	//rpg_npc_spawn(Generic, vector3d_create(100, 8.3, -80));
	//rpg_npc_spawn(Questgiver, vector3d_create(90, 8.3, -20));

	//print_npc_stats(rpg_get_npc()[0]);

	rpg_update_loading_texture(216, SDL_FLIP_NONE);

	rpg_goblin_init();
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-420, 5, -505));
	//rpg_goblin_spawn(GoblinHeavy, vector3d_create(-420, 5, -510));
	//rpg_goblin_spawn(GoblinArcher, vector3d_create(-420, 5, -515));
	//rpg_goblin_spawn(GoblinKing, vector3d_create(-410, 25, -550));

	//// For quest 2
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -205));
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -210));
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-200, 5, -215));
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-210, 5, -225));
	//rpg_goblin_spawn(GoblinGrunt, vector3d_create(-210, 5, -205));
	
	rpg_update_loading_texture(252, SDL_FLIP_NONE);
	
	rpg_item_entity_init(10);
	//potion		= rpg_item_new(consumable, "healthpotion", vector3d_create(-5, 2, 30));
	//arrow		= rpg_item_new(consumable, "arrow", vector3d_create(-5, 2, 50));
	//woodsword	= rpg_item_new(weapon, "woodensword", vector3d_create(-5, 2, 70));
	
	rpg_quest_init();

	rpg_main_quests_init();

	rpg_update_loading_texture(288, SDL_FLIP_NONE);
	
	rpg_chests_init(10);
	/*chest = rpg_chest_new();*/

	rpg_update_loading_texture(324, SDL_FLIP_NONE);
	
	rpg_projectile_init(10);

	rpg_ui_init();

	slog_sync();
	
	matrix4d_identity(modelMat);
	matrix4d_identity(modelMat2);

	//model = gf3d_model_load("dino");
	dino = gf3d_model_load("dino");
	matrix4d_translate(vector3d_create(0,0,0),modelMat);
	//matrix4d_translate(vector3d_create(10, 0, 0), modelMat2);
	
//		get_player()->ent->model = gf3d_model_load_animated("player", 1,19);

	rpg_update_loading_texture(360, SDL_FLIP_NONE);
	
	
	//SDL_ShowCursor(SDL_FALSE);
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
		gf3d_camera_update();
		gf3d_entity_think_all();
		gf3d_entity_update_all();

		gf3d_vgraphics_render_start();

			gf3d_entity_draw_all();
			rpg_ui_update();
			rpg_ui_draw_all();

        gf3d_vgraphics_render_end();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
	return quit();
}

/*eol@eof*/

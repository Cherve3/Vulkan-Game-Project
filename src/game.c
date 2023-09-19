
#include <string.h>

#include <SDL.h>            
#include <SDL_vulkan.h>
#include <SDL_image.h>

#include "game.h"

#include "gfc_input.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"
#include "gf3d_sprite.h"
#include "gf3d_particle.h"

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
extern int __DEBUG;

int quit() 
{
	vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
	//cleanup
	
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
	char filename[128];
    int done = 0;
	int main_menu = 0;
    
	Uint32 old_time = 0, time = 0;
	float frame = 0;
	float frame2 = 0;
	unsigned int lastTime = 0, currentTime;
	float deltaTime = 0;
    Uint8 validate = 1;
	const Uint8* keys;

	Matrix4 modelMat;
	Matrix4 modelMat2;

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

	// check arguments for nable/disable validation layers and debug
    for (int i = 1; i < argc;i++)
    {
        if (strcmp(argv[i],"-disable_validate") == 0)
        {
            validate = 0;
        }
		if (strcmp(argv[i], "-debug") == 0)
		{
			__DEBUG = 1;
		}
    }

	Bool toggleStats = false;

	Chest *chest = NULL;
	Item *wood = NULL;
	Item *arrow = NULL;
	Item *potion = NULL;
	Item *woodsword = NULL;

	Matrix4 dinoMat;
	Model *dino;
	Matrix4 skyMat;
	Model* sky;

	slog("gf3d begin");

    init_logger("gf3d.log", false);
	snprintf(filename, sizeof(filename), "%s%s", FILE_PATH, "config/input.cfg");
	gfc_input_init(filename);

	snprintf(filename, sizeof(filename), "%s%s", FILE_PATH, "config/setup.cfg");
	gf3d_vgraphics_init(filename);
	slog_sync();
	
	rpg_main_menu_init();
	main_menu = rpg_main_menu_load_screen(StartScreen);
	if (main_menu == -1)
	{
		return quit();
	}
	
	main_menu = rpg_main_menu_load_screen(CharacterSelectScreen);

	/** 
	*	Mouse seems to lose focus on game sometimes and crashes the game. It is happening
	*	because the mouse is able to leave the game focus before this is called if it is not within
	*	the screen when this is called it will crash. I am leaning towards having a cursor drawn in
	*	the position of the mouse and calling this at start up to avoid the issue.
	*/
	SDL_SetRelativeMouseMode(SDL_TRUE);

    slog("gf3d loading game begin");
	
	// Called multiple times to reload and rotate loading texture.
	rpg_update_loading_texture(36, SDL_FLIP_NONE);

	gf3d_entity_init(1024);

	rpg_update_loading_texture(72, SDL_FLIP_NONE);

	rpg_world_init();

	rpg_update_loading_texture(108, SDL_FLIP_NONE);
	
	rpg_player_init();
	gf3d_camera_init();

	rpg_update_loading_texture(144, SDL_FLIP_NONE);

	rpg_npc_init();
	rpg_npc_spawn(ItemShop, vector3d(0, -200, 0));
	rpg_npc_spawn(WeaponShop, vector3d(90, 5, -40));
	rpg_npc_spawn(ArmorShop, vector3d(90, 5.5, -60));
	rpg_npc_spawn(SpellShop, vector3d(60, 9.3, -20));
	rpg_npc_spawn(Generic, vector3d(100, 8.3, -80));
	rpg_npc_spawn(Questgiver, vector3d(90, 8.3, -20));

	print_npc_stats(rpg_get_npc()[0]);

	rpg_update_loading_texture(216, SDL_FLIP_NONE);

	rpg_goblin_init();
	rpg_goblin_spawn(GoblinGrunt, vector3d(-420, 5, -505));
	rpg_goblin_spawn(GoblinHeavy, vector3d(-420, 5, -510));
	rpg_goblin_spawn(GoblinArcher, vector3d(-420, 5, -515));
	rpg_goblin_spawn(GoblinKing, vector3d(-410, 25, -550));

	//// For quest 2
	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -205));
	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -210));
	rpg_goblin_spawn(GoblinGrunt, vector3d(-200, 5, -215));
	rpg_goblin_spawn(GoblinGrunt, vector3d(-210, 5, -225));
	rpg_goblin_spawn(GoblinGrunt, vector3d(-210, 5, -205));
	
	rpg_update_loading_texture(252, SDL_FLIP_NONE);
	
	rpg_item_entity_init(10);
	potion		= rpg_item_new(consumable, "healthpotion", vector3d(-5, 2, 30));
	arrow		= rpg_item_new(consumable, "arrow", vector3d(-5, 2, 50));
	woodsword	= rpg_item_new(weapon, "woodensword", vector3d(-5, 2, 70));
	
	rpg_quest_init();

	rpg_main_quests_init();

	rpg_update_loading_texture(288, SDL_FLIP_NONE);
	
	rpg_chests_init(10);
	chest = rpg_chest_new();

	rpg_update_loading_texture(324, SDL_FLIP_NONE);
	
	rpg_projectile_init(10);

	rpg_ui_init();

	slog_sync();

	gf3d_camera_set_scale(vector3d(1, 1, 1));

	rpg_update_loading_texture(360, SDL_FLIP_NONE);
	
	sky = gf3d_model_load("D:/Git/Projects/Vulkan-Game-Project/models/sky.model");
	gfc_matrix_identity(skyMat);
	gfc_matrix_scale(skyMat, vector3d(1000, 1000, 1000));

	//Game Loop
	slog("Game Loop Begin");
	while (!done)
	{	
		gfc_input_update();

		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		// Update
		gf3d_entity_think_all();
		gf3d_entity_update_all();
		rpg_ui_update();

		gf3d_camera_update();
		gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

		// Draw
		gf3d_vgraphics_render_start();
			gf3d_model_draw_sky(sky, skyMat, gfc_color(1, 1, 1, 1));
			gf3d_entity_draw_all();
			
			rpg_ui_draw_all();

        gf3d_vgraphics_render_end();

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
	return quit();
}

/*eol@eof*/

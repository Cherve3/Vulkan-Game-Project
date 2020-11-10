#include <SDL.h>            

#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_types.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"

#include "rpg_chests.h"
#include "rpg_goblin.h"
#include "rpg_player.h"
#include "rpg_npc.h"
#include "rpg_ui.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;

	Entity *world = NULL;
	Chest *chest = NULL;
	Entity *water = NULL;
	Entity *fire = NULL;
	Bool *toggleStats = false;

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

	rpg_chests_init(10);

	rpg_player_init();

	world = gf3d_entity_new();
	chest = rpg_chest_new();
	fire = gf3d_entity_new();
	
	//water = gf3d_entity_new();
	
	rpg_goblin_init(GoblinGrunt,vector3d(-10,0, -10));
	rpg_goblin_init(GoblinHeavy, vector3d(10, 0, -10));
	rpg_goblin_init(GoblinArcher, vector3d(10, 0, 10));

	slog_sync();
	if (world)
	{
		world->model = gf3d_model_load("world");
		world->name = "World";
		world->position = vector3d(0, 0, 0);
		gfc_matrix_new_translation(world->modelMatrix, world->position);
		world->boxCollider.depth = 10000.0;
		world->boxCollider.height = 1;
		world->boxCollider.width = 10000.0;
		world->boxCollider.x = world->position.x;
		world->boxCollider.y = world->position.y;
		world->boxCollider.z = world->position.z;
		
	}
	if (chest){
/*		chest->loot[0].description = "A wood log.";
		chest->loot[0].name = "Wood";
		chest->loot[0].type = 1;
		chest->loot[0].weight = 2;
		chest->loot[0].quantity = 1;*/
		//chest->model = gf3d_model_load("chest");
		//chest->position = vector3d(0, 0, 0);
		//gfc_matrix_new_translation(chest->modelMatrix, chest->position);
		//chest->boxCollider.depth = 2.0;
		//chest->boxCollider.height = 2.0;
		//chest->boxCollider.width = 2.0;
		//chest->boxCollider.x = chest->position.x;
		//chest->boxCollider.y = chest->position.y;
		//chest->boxCollider.z = chest->position.z;
		//chest->name = "Chest";
	}
	if (water)
	{
		water->model = gf3d_model_load("water");
		water->position = vector3d(20, -2, 20);
		water->name = "Water";
	}
	if (fire)
	{
		fire->model = gf3d_model_load("fire");
		fire->position = vector3d(0, 10, 2);
		fire->name = "Fire";
		gfc_matrix_new_translation(fire->modelMatrix, fire->position);
	}
	
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
    
	//UI TEST
	SDL_Renderer *renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	rpg_ui_init(renderer, 720, 480);

	while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

		// configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
			gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
				
				//SDL_RenderPresent(renderer);//UI TEST
				
				camera_update();

				gf3d_entity_think_all();
				gf3d_entity_update_all();
				gf3d_entity_draw_all(bufferFrame, commandBuffer);

				//toggle_stats(renderer, toggleStats);
/*				if (!toggleStats){
					toggleStats = true;
					stats_on(renderer);
				
				}
				else
				{
					toggleStats = false;
					//stats_off(SDL_GetRenderer(gf3d_vgraphics_get_window()));
				}*/


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

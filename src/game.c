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
#include "rpg_projectile.h"
#include "rpg_goblin.h"
#include "rpg_player.h"
#include "rpg_npc.h"
#include "rpg_ui.h"
#include "rpg_input.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
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
	Entity *box1 = NULL;
	Entity *box2 = NULL;

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

	rpg_projectile_init(10);
	
	rpg_player_init();

	wood		= gf3d_entity_new();
	world		= gf3d_entity_new();
	chest		= rpg_chest_new();
	box1		= gf3d_entity_new();
	box2		= gf3d_entity_new();
	
	water = gf3d_entity_new();
	
	rpg_goblin_init(GoblinGrunt,vector3d(-10,2, -10));
	rpg_goblin_init(GoblinHeavy, vector3d(10, 2, -10));
	rpg_goblin_init(GoblinArcher, vector3d(10, 2, 10));
	
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
	
	if (box1)
	{
		box1->model = gf3d_model_load("box");
		box1->name = "Box1";
		box1->position = vector3d(0, 1, 0);
		gfc_matrix_new_translation(box1->modelMatrix, box1->position);
		box1->boxCollider.depth = 1;
		box1->boxCollider.height = 1;
		box1->boxCollider.width = 1;
		box1->boxCollider.x = box1->position.x;
		box1->boxCollider.y = box1->position.y;
		box1->boxCollider.z = box1->position.z;
	}

	if (box2)
	{
		box2->model = gf3d_model_load("box");
		box2->name = "Box2";
		box2->position = vector3d(2, 3, 2);
		gfc_matrix_new_translation(box2->modelMatrix, box2->position);
		box2->boxCollider.depth = 1;
		box2->boxCollider.height = 1;
		box2->boxCollider.width = 1;
		box2->boxCollider.x = box2->position.x;
		box2->boxCollider.y = box2->position.y;
		box2->boxCollider.z = box2->position.z;
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
    
	//UI TEST
	/*SDL_Renderer *renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	rpg_ui_init(renderer, 720, 480);*/
	
	//SDL_RenderClear(renderer);
	while(!done)
    {
		
       SDL_PumpEvents();   // update SDL's internal event structures
       keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        //update game things here
	   //SDL_RenderPresent(renderer);

		
		// configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
			gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
			
			gf3d_entity_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

		gf3d_entity_think_all();
		gf3d_entity_update_all();


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

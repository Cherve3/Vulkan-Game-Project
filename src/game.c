#include <SDL.h>            

#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"

#include "rpg_player.h"
#include "rpg_npc.h"

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
	Entity *ent1 = NULL;
	Entity *playerEnt = NULL;
	Entity *goblinGrunt = NULL;
	Entity *goblinHeavy = NULL;
	Entity *world = NULL;
	Entity *chest = NULL;
	Player *player;

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

	gf3d_entity_init(1024);

    // main game loop
    slog("gf3d main loop begin");
	
	rpg_player_init();
	player = get_player();
	playerEnt = get_player_entity();

	goblinGrunt = rpg_npc_new();
	world = gf3d_entity_new();
	chest = gf3d_entity_new();
	goblinHeavy = rpg_npc_new();

	if (goblinGrunt)
	{
		goblinGrunt->model = gf3d_model_load("goblingrunt");
		goblinGrunt->think = rpg_npc_think;
		goblinGrunt->name = "Goblin Grunt";
		gfc_matrix_make_translation(goblinGrunt->modelMatrix, vector3d(gfc_crandom() * 3, gfc_crandom() * 3, gfc_crandom() * 3));
		gfc_matrix_rotate(goblinGrunt->modelMatrix, goblinGrunt->modelMatrix, 0.002, vector3d(0, 0, 1));
		
	}
	if (goblinHeavy)
	{
		goblinHeavy->model = gf3d_model_load("goblinheavy");
		goblinHeavy->think = rpg_npc_think;
		goblinHeavy->name = "Goblin Heavy";
		gfc_matrix_make_translation(goblinHeavy->modelMatrix, vector3d(gfc_crandom() * 8, gfc_crandom() * 8, gfc_crandom() * 8));
		gfc_matrix_rotate(goblinHeavy->modelMatrix, goblinHeavy->modelMatrix, 0.002, vector3d(0, 0, 1));

	}
	if (playerEnt)
	{
		playerEnt->model = gf3d_model_load("dino");
		playerEnt->think = rpg_player_think;
		playerEnt->name = "Player";
	}
	slog_sync();
	if (world)
	{
		world->model = gf3d_model_load("world");
		world->name = "World";
		world->position = vector3d(0, 0, -6);
		gfc_matrix_make_translation(world->modelMatrix, world->position);
		world->boxCollider.depth = 1.0;
		world->boxCollider.height = 1.0;
		world->boxCollider.width = 1.0;
		world->boxCollider.x = 1.0;
		world->boxCollider.y = 1.0;
		world->boxCollider.z = 1.0;
		
	}
	if (chest){
		chest->model = gf3d_model_load("chest");
		gfc_matrix_make_translation(chest->modelMatrix, vector3d(0, 0, 0));
		chest->name = "Chest";
	}
	
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
    
	while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here

		gf3d_entity_think_all();

		//slog("\nPosition: x:%f, y:%f, z:%f", playerEnt->position.x, playerEnt->position.y, playerEnt->position.z);
		//slog("\nVelocity: x:%f, y:%f, z:%f", playerEnt->velocity.x, playerEnt->velocity.y, playerEnt->velocity.z);
		//slog("\nRotation: x:%f, y:%f, z:%f", player->rotation.x, player->rotation.y, player->rotation.z);
		
		// configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
			gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);
				
				gf3d_entity_draw_all(bufferFrame, commandBuffer);
                
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/

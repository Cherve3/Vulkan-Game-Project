#include <SDL.h>            

#include "simple_logger.h"
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
	Entity *player = NULL;
	Entity *goblin = NULL;
	Entity *goblin2 = NULL;
	Entity *world = NULL;
	Entity *chest = NULL;
    
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
	
	player = rpg_player_new();
	goblin = rpg_npc_new();
	world = gf3d_entity_new();
	//chest = gf3d_entity_new();
	//goblin2 = gf3d_entity_new();

	if (goblin)
	{
		goblin->model = gf3d_model_load("goblin");
		goblin->think = rpg_npc_think;
		goblin->name = "Goblin";
		gfc_matrix_make_translation(goblin->modelMatrix, vector3d(gfc_crandom() * 3, gfc_crandom() * 3, gfc_crandom() * 3));
		gfc_matrix_rotate(goblin->modelMatrix, goblin->modelMatrix, 0.002, vector3d(0, 0, 1));
		
	}

	if (player)
	{
		player->model = gf3d_model_load("dino");
		player->think = rpg_player_think;
		player->name = "Player";
		player->position = vector3d(0, 0, 7);
	}
	slog_sync();
	if (world)
	{
		world->model = gf3d_model_load("world");
		world->name = "World";
		//world->position = vector3d(0, 0, -2);
		//gfc_matrix_make_translation(world->modelMatrix, world->position);
		world->boxCollider.depth = 1.0;
		world->boxCollider.height = 1.0;
		world->boxCollider.width = 1.0;
		world->boxCollider.x = 1.0;
		world->boxCollider.y = 1.0;
		world->boxCollider.z = 1.0;
		
	}
	if (chest){
		chest->model = gf3d_model_load("chest");
		gfc_matrix_make_translation(chest->modelMatrix, vector3d(gfc_crandom() * 8, gfc_crandom() * 8, gfc_crandom() * 8));
		chest->name = "Chest";
	}

	gf3d_camera_set_view(gf3d_vgraphics_get_uniform_buffer_object().view);
	gf3d_camera_set_position(vector3d(10,100,10));

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
		
		SDL_ShowCursor(0);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		
        //gf3d_vgraphics_rotate_camera(0.001);

		gf3d_entity_think_all();
		slog("\nPosition: x:%f, y:%f, z:%f", player->boxCollider.x, player->boxCollider.y, player->boxCollider.z);
		//slog("\nVelocity: x:%f, y:%f, z:%f", player->velocity.x, player->velocity.y, player->velocity.z);
		//slog("\nRotation: x:%f, y:%f, z:%f", player->rotation.x, player->rotation.y, player->rotation.z);
		
		//gfc_matrix_slog(player->modelMatrix);
		
		
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

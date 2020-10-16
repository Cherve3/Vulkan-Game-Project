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
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();

	gf3d_entity_init(1024);

    // main game loop
    slog("gf3d main loop begin");
	
	player = rpg_player_new();
	goblin = gf3d_entity_new();
	if (goblin)
	{
		goblin->model = gf3d_model_load("goblin");
		gfc_matrix_make_translation(goblin->modelMatrix, vector3d(gfc_crandom() * 5, gfc_crandom() * 5, gfc_crandom() * 5));
		gfc_matrix_rotate(goblin->modelMatrix, goblin->modelMatrix,0.002, vector3d(0, 0, 1));
	}

	if (player)
	{
		player->model = gf3d_model_load("dino");
		player->think = rpg_player_think;
		player->position = vector3d(0, 0, 0);
	}

    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
        
		//SDL_CaptureMouse(0);
		SDL_ShowCursor(0);
        //gf3d_vgraphics_rotate_camera(0.001);

		gf3d_entity_think_all();
		gfc_matrix_rotate(goblin->modelMatrix, goblin->modelMatrix, 0.002, vector3d(0, 0, 1));
		
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

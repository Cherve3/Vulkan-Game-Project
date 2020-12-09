#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>            
#include <SDL_vulkan.h>

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
#include "gf3d_sprite.h"

#include "rpg_world.h"
#include "rpg_items.h"
#include "rpg_chests.h"
#include "rpg_projectile.h"
#include "rpg_goblin.h"
#include "rpg_player.h"
#include "rpg_npc.h"
#include "rpg_ui.h"
#include "rpg_input.h"

#define GRAVITY -9.8

#endif
#ifndef __RPG_UI_H__
#define __RPG_UI_H__

#include "gf3d_vgraphics.h"
#include "gf3d_sprite.h"

#include "rpg_player.h"

typedef struct
{

	Sprite *health;
	Sprite *mana;
	Sprite *stamina;
	Sprite *base;
}HUD;

typedef struct
{
	SDL_Surface *menuSurface;
	SDL_Color white;

	Sprite *menu;
	Sprite *stats;
	Sprite **text;
	Sprite *map;

	Uint8 text_sprite_count;
}Menu;

void rpg_ui_init();

void rpg_ui_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void rpg_ui_update();

void toggle_stats(SDL_Renderer *renderer);

void stats_on(SDL_Renderer *renderer);

void stats_off(SDL_Renderer *renderer);



#endif
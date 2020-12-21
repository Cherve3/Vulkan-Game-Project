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
	Sprite *map_point;
	Vector2D point_position;

	Uint8 text_count;
}Menu;

typedef struct
{
	Sprite *menu;
	Sprite **text;
	Sprite *textbox;

	Uint8 text_count;

}ShopUI;

void rpg_ui_init();

void rpg_ui_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void rpg_ui_update();


#endif
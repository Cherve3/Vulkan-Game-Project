#ifndef __RPG_UI_H__
#define __RPG_UI_H__

#include <stdbool.h>

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
	SDL_Renderer *renderer;
	SDL_Texture  *bg_texture;
	SDL_Texture  *tex_loading;
	SDL_Rect	 load_rect;
	SDL_Rect	 button_1_rect;
	SDL_Rect	 button_2_rect;
	char         *model_name;
}MainMenu;

typedef struct
{
	Sprite *menu;
	Sprite **text;
	Sprite *textbox;

	Uint8 text_count;

}ShopUI;

typedef enum
{
	StartScreen,
	CharacterSelectScreen,
	LoadingScreen
}MainMenuScreen;

void rpg_main_menu_close();

void rpg_ui_close();

void rpg_destroy_main_menu();

bool isCursorHoveringButton(SDL_Rect *button, int x, int y);

char* rpg_ui_get_player_model_name();

int rpg_main_menu_load_screen(MainMenuScreen screen);

void rpg_update_loading_texture(const float angle, const SDL_RendererFlip flip);

void rpg_main_menu_init();

void rpg_ui_init();

void rpg_ui_draw_all();

void rpg_ui_update();


#endif
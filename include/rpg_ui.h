#ifndef __RPG_UI_H__
#define __RPG_UI_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_surface.h"
#include "rpg_player.h"

typedef struct
{
	SDL_Surface *HUDsurface;

	SDL_Texture *health;
	SDL_Texture *mana;
	SDL_Texture *stamina;
	SDL_Texture **base;
}HUD;

typedef struct
{
	SDL_Surface *menuSurface;

	SDL_Texture *menu;
	SDL_Texture *stats;
	SDL_Texture *text;

	SDL_Color white;
}Menu;

void rpg_ui_init(SDL_Renderer *renderer, int screenWidth, int screenHeight);

void toggle_stats(SDL_Renderer *renderer);

void stats_on(SDL_Renderer *renderer);

void stats_off(SDL_Renderer *renderer);

#endif
#ifndef __RPG_UI_H__
#define __RPG_UI_H__

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_surface.h"

typedef struct
{
	SDL_Surface *health;
	SDL_Surface *mana;
	SDL_Surface *stamina;
	SDL_Surface *base;

	SDL_Texture *healthTexture;
	SDL_Texture *manaTexture;
	SDL_Texture *staminaTexture;
	SDL_Texture *baseTexture;
}HUD;

typedef struct
{
	SDL_Texture *menu;
	SDL_Texture *stats;
}Menu;

void rpg_ui_init(SDL_Renderer *renderer);

void toggle_stats(SDL_Renderer *renderer);

#endif
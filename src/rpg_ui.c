#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_ui.h"

void rpg_ui_init(SDL_Window *window)
{
	HUD hud;
	hud.base    = IMG_Load("images/base_bars.png");
	hud.health  = IMG_Load("images/health_bar.png");
	hud.mana	= IMG_Load("images/mana_bar.png");
	hud.stamina = IMG_Load("images/stamina_bar.png");
	hud.base->w = 250;
	hud.base->h = 150;
	
	hud.baseTexture = SDL_CreateTextureFromSurface(SDL_GetRenderer(window), hud.base);
/*	
	SDL_Renderer *renderer = SDL_CreateRenderer(gf3d_vgraphics.main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_Surface* base = IMG_Load("images/base_bars.png");

	SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, base);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);

	SDL_RenderPresent(renderer);

	gf3d_texture_load("images/base_bars.png");*/
	//SDL_FreeSurface();
}
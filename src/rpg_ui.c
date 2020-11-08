#include "simple_logger.h"

#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_ui.h"

void rpg_ui_init(SDL_Renderer *renderer)
{
	HUD hud;
	hud.base = IMG_Load("images/base_bars.png");
	hud.health = IMG_Load("images/health_bar.png");
	hud.mana = IMG_Load("images/mana_bar.png");
	hud.stamina = IMG_Load("images/stamina_bar.png");
	hud.base->w = 250;
	hud.base->h = 100;

	hud.baseTexture = SDL_CreateTextureFromSurface(SDL_GetRenderer(gf3d_vgraphics_get_window()), hud.base);

	//SDL_Renderer *renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!renderer){
		slog("Renderer NULL");
	}
	//SDL_Surface* base = IMG_Load("images/base_bars.png");

	//if (!base){
	//	slog("Texture surface NULL");
	//}

	SDL_Texture *test = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,250,100);

	SDL_Texture *base = IMG_LoadTexture(renderer, "images/base_bars.png");
	SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, hud.base);
	if (!lettuce_tex){
		slog("Texture NULL");
	}
	SDL_SetRenderTarget(renderer,test);
	SDL_SetTextureBlendMode(lettuce_tex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(test, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(base, SDL_BLENDMODE_BLEND);
	SDL_Rect rect;
	rect.x = 10;
	rect.y = 10;
	rect.h = 100;
	rect.w = 250;
	//SDL_UpdateTexture(renderer,rect,img);
	SDL_RenderClear(renderer);
	int copy = SDL_RenderCopy(renderer, base, NULL, &rect);

	//SDL_RenderPresent(renderer);

	//gf3d_texture_load("images/base_bars.png");
	//SDL_FreeSurface();
}
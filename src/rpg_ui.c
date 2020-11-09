
//#include <string>

#include "simple_logger.h"

#include "SDL_ttf.h"

#include "gfc_types.h"

#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_player.h"
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
		return;
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

	SDL_RenderClear(renderer);
	int copy = SDL_RenderCopy(renderer, base, NULL, &rect);

	//SDL_RenderPresent(renderer);

	//gf3d_texture_load("images/base_bars.png");
	//SDL_FreeSurface();
	if (TTF_Init() < 0) {
		SDL_GetError();
	}
}

void toggle_stats(SDL_Renderer *renderer)
{
	const Uint8 *keys;
	Bool toggle = false;
	keys = SDL_GetKeyboardState(NULL);

	Player *player = get_player();
	
	SDL_Texture *statmenu = IMG_LoadTexture(renderer, "images/statmenu.png");
	SDL_Rect rect ,rect2;

	TTF_Font *font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 64);
	if (!font)
	{
		slog("Font is NULL");
		slog(SDL_GetError());
	}
	char* textName = "Name: ";
	
	slog(textName);
	SDL_Color color;
	color.b = 255;
	color.r = 255;
	color.g = 255;
	color.a = 255;

	SDL_Surface *text = TTF_RenderText_Solid(font, textName,color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer,text);
	if (!text)
	{
		slog("Surface is NULL");
		slog(SDL_GetError());
	}

	if (!tex)
	{
		slog("Texture is NULL");
		slog(SDL_GetError());
	}

	rect.x = 50;
	rect.y = 40;
	rect.h = 500;
	rect.w = 400;

	rect2.x = 90;
	rect2.y = 70;
	rect2.h = 50;
	rect2.w = 100;

	if (keys[SDL_SCANCODE_TAB] && toggle == false)
	{
		slog("Toggled stats ON");
		SDL_FreeSurface(text);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, statmenu, NULL, &rect);
		SDL_RenderCopy(renderer, tex, NULL, &rect2);

		toggle = true;
	}
	else if (toggle == true)
	{
		slog("Toggled stats OFF");
		toggle = false;
		SDL_DestroyTexture(statmenu);
		SDL_RenderClear(renderer);
		SDL_DestroyRenderer(renderer);
	}
	
}
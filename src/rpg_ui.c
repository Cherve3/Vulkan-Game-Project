
//#include <string>

#include "simple_logger.h"

#include "SDL_ttf.h"

#include "gfc_types.h"

#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_player.h"
#include "rpg_ui.h"


typedef struct
{
	HUD hud;
	Menu menu;
	TTF_Font *font;
	SDL_Rect *rect_list;
}UIManager;

static UIManager UI = { 0 };

void rpg_ui_init()
{

	UI.hud.base = NULL;
	UI.hud.health = NULL;
	UI.hud.mana = NULL;
	UI.hud.stamina = NULL;


	if (TTF_Init() < 0) {
		SDL_GetError();
	}

	UI.font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 64);
	if (!UI.font)
	{
		slog("Font is NULL");
		slog(SDL_GetError());
	}

	UI.hud.base = gf3d_sprite_load("images/base_bars.png", -1, -1, 0);
	if (!UI.hud.base) slog("base sprite not loaded");

	UI.hud.health = gf3d_sprite_load("images/health_bar.png", -1, -1, 0);
	if (!UI.hud.health) slog("health sprite not loaded");

	UI.hud.mana = gf3d_sprite_load("images/mana_bar.png", -1, -1, 0);
	if (!UI.hud.mana) slog("mana sprite not loaded");

	UI.hud.stamina = gf3d_sprite_load("images/stamina_bar.png", -1, -1, 0);
	if (!UI.hud.stamina) slog("stamina sprite not loaded");

}

void rpg_ui_draw_all(Uint32 bufferFrame, VkCommandBuffer commandbuffer)
{
	float mana_size = 1;
	float mana_ratio = (float)get_player_stats().mana / (float)get_player_stats().mana_max;
	if (mana_ratio != 1)
		mana_size = mana_size * mana_ratio;

//	slog("mana size: %f", mana_size);
//	slog("mana ratio: %f", mana_ratio);

	gf3d_sprite_draw(UI.hud.base, vector2d(10, 10), vector2d(1, 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.health, vector2d(12, 13), vector2d(1, 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.mana, vector2d(12, 31), vector2d(mana_ratio, 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.stamina, vector2d(12, 51), vector2d(1, 1), 0, bufferFrame, commandbuffer);
}

void rpg_ui_update()
{
	float mana_ratio = get_player_stats().mana / get_player_stats().mana_max;
}

void toggle_stats(SDL_Renderer *renderer, Bool toggle)
{
	const Uint8 *keys;
	keys = SDL_GetKeyboardState(NULL);

	Player *player = get_player();

	SDL_Texture *statmenu;

	if (keys[SDL_SCANCODE_TAB] && player->stats.toggleStats == true)
	{
		renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		statmenu = IMG_LoadTexture(renderer, "images/statmenu.png");
		SDL_Rect rect, rect2;

		TTF_Font *font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 64);
		if (!font)
		{
			slog("Font is NULL");
			slog(SDL_GetError());
		}
		char* textName = strcat("Name: ",player->stats.name);

		slog(textName);
		SDL_Color color;
		color.b = 255;
		color.r = 255;
		color.g = 255;
		color.a = 255;

		SDL_Surface *text = TTF_RenderText_Solid(font, textName, color);
		SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, text);
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

	
		slog("Toggled stats ON");
		SDL_FreeSurface(text);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, statmenu, NULL, &rect);
		SDL_RenderCopy(renderer, tex, NULL, &rect2);
		SDL_RenderPresent(renderer);
		toggle = true;
	}
	else if (keys[SDL_SCANCODE_TAB] && player->stats.toggleStats == false)
	{
		slog("Toggled stats OFF");
		toggle = false;
		//SDL_DestroyTexture(statmenu);
		SDL_RenderClear(renderer);
		SDL_DestroyRenderer(renderer);
	}

}
/*
void stats_on(SDL_Renderer *renderer)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	SDL_Rect rectMenu, rectText;
	SDL_Texture *menu, *text;
	if (!menu && !text)
	{
		menu = IMG_LoadTexture(renderer, "images/statmenu.png");

		UI.menu->menuSurface = TTF_RenderText_Blended(UI.font, "Name: Player 1", UI.menu->white);
		UI.menu->text = SDL_CreateTextureFromSurface(renderer, UI.menu->menuSurface);
		text = IMG_LoadTexture(renderer, "images/stamina_bar.png");
	}
	rectMenu.x = 50;
	rectMenu.y = 40;
	rectMenu.h = 400;
	rectMenu.w = 400;

	rectText.x = 90;
	rectText.y = 70;
	rectText.h = 40;
	rectText.w = 100;

	if (keys[SDL_SCANCODE_TAB])
	{
		
		slog("Toggled stats ON");
		//SDL_FreeSurface(text);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, UI.menu->stats, NULL, &rectMenu);
		SDL_RenderCopy(renderer, UI.menu->text, NULL, &rectText);
		SDL_RenderPresent(renderer);
		//SDL_DestroyTexture(menu);
		//SDL_DestroyTexture(text);
	}
}

void stats_off(SDL_Renderer *renderer)
{
	const Uint8 *keys;

	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_TAB])
	{
		SDL_RenderPresent(renderer);
		//SDL_DestroyTexture(statmenu);
		SDL_RenderClear(renderer);
		SDL_DestroyRenderer(renderer);

	}
}*/

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
	HUD *hud;
	Menu *menu;
	TTF_Font *font;
	SDL_Rect *rect_list;
}UIManager;

UIManager UI = { 0 };

void rpg_ui_init(SDL_Renderer *renderer, int screenWidth, int screenHeight)
{
	if (TTF_Init() < 0) {
		SDL_GetError();
	}

	UI.font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 64);
	if (!UI.font)
	{
		slog("Font is NULL");
		slog(SDL_GetError());
	}
	

	SDL_Texture *base		= IMG_LoadTexture(renderer, "images/base_bars.png");
	SDL_Texture *health		= IMG_LoadTexture(renderer, "images/health_bar.png");
	SDL_Texture *mana		= IMG_LoadTexture(renderer, "images/mana_bar.png");
	SDL_Texture *stamina	= IMG_LoadTexture(renderer, "images/stamina_bar.png");


	SDL_Rect rectBase, rectHealth, rectMana, rectStamina;

	rectBase.w = screenWidth * 0.25;	//250
	rectBase.h = screenHeight * 0.20;	//100
	rectBase.x = 10;
	rectBase.y = 10;

	rectHealth.w = screenWidth * 0.24;	//241
	rectHealth.h = screenHeight * 0.03; //14
	rectHealth.x = 15;
	rectHealth.y = 16;

	rectMana.w = screenWidth * 0.24;	//241
	rectMana.h = screenHeight * 0.03;	//14
	rectMana.x = 15;
	rectMana.y = 50;

	rectStamina.w = screenWidth * 0.24;	//241
	rectStamina.h = screenHeight * 0.03;//14
	rectStamina.x = 15;
	rectStamina.y = 87;

	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, base, NULL, &rectBase);
	SDL_RenderCopy(renderer, health, NULL, &rectHealth);
	SDL_RenderCopy(renderer, mana, NULL, &rectMana);
	SDL_RenderCopy(renderer, stamina, NULL, &rectStamina);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
/*	UI.hud->base = IMG_LoadTexture(renderer, "images/base_bars.png");
	if (!UI.hud->base)
	{
		slog("TExture NULL");
		slog(SDL_GetError());
	}
	UI.rect_list[0].w = 250 * (screenWidth * 0.25);
	UI.rect_list[0].h = 100 * (screenHeight * 0.25);
	UI.rect_list[0].x = 10;
	UI.rect_list[0].y = 10;

	UI.hud->health	= IMG_LoadTexture(renderer, "images/health_bar.png");
	UI.rect_list[1].w = 240 * (screenWidth * 0.25);
	UI.rect_list[1].h = 15  * (screenHeight * 0.25);
	UI.rect_list[1].x = 15;
	UI.rect_list[1].y = 15;
	
	UI.hud->mana	= IMG_LoadTexture(renderer, "images/mana_bar.png");
	UI.rect_list[2].w = 240 * (screenWidth * 0.25);
	UI.rect_list[2].h = 15  * (screenHeight * 0.25);
	UI.rect_list[2].x = 15;
	UI.rect_list[2].y = 35;
	
	UI.hud->stamina = IMG_LoadTexture(renderer, "images/stamina_bar.png");
	UI.rect_list[3].w = 240 * (screenWidth * 0.25);
	UI.rect_list[3].h = 15  * (screenHeight * 0.25);
	UI.rect_list[3].x = 15;
	UI.rect_list[3].y = 55;

	UI.menu->stats	= IMG_LoadTexture(renderer, "images/statmenu.png");
	UI.rect_list[4].w = 400 * (screenWidth * 0.25);
	UI.rect_list[4].h = 500 * (screenHeight * 0.25);
	UI.rect_list[4].x = 50;
	UI.rect_list[4].y = 40;
	
	UI.menu->white.a = 255;
	UI.menu->white.a = 255;
	UI.menu->white.a = 255;
	UI.menu->white.a = 255;

	UI.menu->menuSurface = TTF_RenderText_Blended(UI.font, UI.menu->text, UI.menu->white);
	UI.menu->text	= SDL_CreateTextureFromSurface(renderer, UI.menu->menuSurface);
	UI.rect_list[5].w = 100 * (screenWidth * 0.25);
	UI.rect_list[5].h = 40  * (screenHeight * 0.25);
	UI.rect_list[5].x = 90;
	UI.rect_list[5].y = 70;
	
	*/
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
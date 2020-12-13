
//#include <string>

#include "simple_logger.h"

#include "SDL_ttf.h"

#include "gfc_types.h"
#include "gfc_text.h"

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

float life_ratio;
float mana_ratio;
float stamina_ratio;

Sprite *sprite;
Sprite *sprite2;

Sprite *rpg_ui_text_stats(char* name, char* text);

void rpg_ui_init()
{
	SDL_Surface *text = NULL;
	SDL_Surface *text2 = NULL;

	slog("Initializing UI...");
	UI.hud.base		= NULL;
	UI.hud.health	= NULL;
	UI.hud.mana		= NULL;
	UI.hud.stamina	= NULL;
	UI.menu.stats	= NULL;
	UI.menu.map     = NULL;
	UI.menu.text	= NULL;

	life_ratio	= 0;
	mana_ratio		= 0;
	stamina_ratio	= 0;



	if (TTF_Init() < 0) {
		SDL_GetError();
	}

	slog("Loading font...");
	UI.font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 32);
	if (!UI.font)
	{
		slog("Font is NULL");
		slog(SDL_GetError());
	}

	UI.menu.text_sprite_count = 17;
	UI.menu.text = gfc_allocate_array(sizeof(Sprite*), UI.menu.text_sprite_count);

//	Attempting to render text
	UI.menu.white.a = 1;
	UI.menu.white.r = 255;
	UI.menu.white.g = 255;
	UI.menu.white.b = 255;

	UI.hud.base = gf3d_sprite_load("images/base_bars.png", NULL, -1, -1, 0);
	if (!UI.hud.base) slog("base sprite not loaded");

	UI.hud.health = gf3d_sprite_load("images/health_bar.png", NULL, -1, -1, 0);
	if (!UI.hud.health) slog("health sprite not loaded");

	UI.hud.mana = gf3d_sprite_load("images/mana_bar.png", NULL, -1, -1, 0);
	if (!UI.hud.mana) slog("mana sprite not loaded");

	UI.hud.stamina = gf3d_sprite_load("images/stamina_bar.png", NULL, -1, -1, 0);
	if (!UI.hud.stamina) slog("stamina sprite not loaded");

	UI.menu.stats = gf3d_sprite_load("images/statmenu.png", NULL, -1, -1, 0);
	if (!UI.menu.stats) slog("stat menu sprite not loaded");
	
	UI.menu.map = gf3d_sprite_load("images/map.png",NULL,  -1, -1, 0);
	if (!UI.menu.stats) slog("map menu sprite not loaded");


	char buffer[26];

	strcpy(buffer, "Name: ");
	strcat(buffer, get_player()->stats.name);
	slog("Buffer 1: %s",buffer);
	UI.menu.text[0] = rpg_ui_text_stats("NameText", buffer);

	char int_buffer[3];
	strcpy(buffer, "Level: ");
	_itoa(get_player()->stats.level, int_buffer, 10);
	strcat(buffer, int_buffer);
		slog("Buffer : %s", buffer);

	UI.menu.text[1] = rpg_ui_text_stats("LevelText", buffer);

	strcpy(buffer, "Exp: ");
	_itoa(get_player()->stats.exp, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[2] = rpg_ui_text_stats("ExpText", buffer);

	strcpy(buffer, "Bits: ");
	_itoa(get_player()->stats.bits, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[3] = rpg_ui_text_stats("BitsText", buffer);

	strcpy(buffer, "Life: ");
	_itoa(get_player()->stats.life, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.life_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[4] = rpg_ui_text_stats("LifeText", buffer);

	strcpy(buffer, "Mana: ");
	_itoa(get_player()->stats.mana, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.mana_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[5] = rpg_ui_text_stats("ManaText", buffer);

	strcpy(buffer, "Stamina: ");
	_itoa(get_player()->stats.stamina, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.stamina_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[6] = rpg_ui_text_stats("StaminaText", buffer);

	char float_buffer[10];
	strcpy(buffer, "Carry Weight: ");
	_gcvt(get_player()->stats.carry_weight, 5, float_buffer);
	strcat(buffer, float_buffer);
	strcat(buffer, "/");
	_gcvt(get_player()->stats.carry_weight_max, 5, float_buffer);
	strcat(buffer, float_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[7] = rpg_ui_text_stats("CarryText", buffer);

	strcpy(buffer, "Strength: ");
	_itoa(get_player()->stats.strength, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[8] = rpg_ui_text_stats("StrengthText", buffer);

	strcpy(buffer, "Health: ");
	_itoa(get_player()->stats.health, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[9] = rpg_ui_text_stats("HealthText", buffer);

	strcpy(buffer, "Perception: ");
	_itoa(get_player()->stats.perception, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[10] = rpg_ui_text_stats("PerceptionText", buffer);

	strcpy(buffer, "Speed: ");
	_itoa(get_player()->stats.speed, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[11] = rpg_ui_text_stats("SpeedText", buffer);

	strcpy(buffer, "Willpower: ");
	_itoa(get_player()->stats.willpower, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[12] = rpg_ui_text_stats("WillpowerText", buffer);

	strcpy(buffer, "Dexterity: ");
	_itoa(get_player()->stats.dexterity, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[13] = rpg_ui_text_stats("DexterityText", buffer);

	strcpy(buffer, "Intelligence: ");
	_itoa(get_player()->stats.intelligence, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[14] = rpg_ui_text_stats("IntelligenceText", buffer);

	strcpy(buffer, "Luck: ");
	_itoa(get_player()->stats.luck, int_buffer, 10);
	strcat(buffer, int_buffer);
	slog("Buffer : %s", buffer);

	UI.menu.text[15] = rpg_ui_text_stats("LuckText", buffer);
}

void rpg_ui_draw_all(Uint32 bufferFrame, VkCommandBuffer commandbuffer)
{
	int i = 0;
	int x_pos = 110;
	int y_pos = 90;

	gf3d_sprite_draw(UI.hud.base, vector2d(10, 10), vector2d(1, 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.health, vector2d(12, 13), vector2d(life_ratio , 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.mana, vector2d(12, 31), vector2d(mana_ratio, 1), 0, bufferFrame, commandbuffer);
	gf3d_sprite_draw(UI.hud.stamina, vector2d(12, 51), vector2d(stamina_ratio, 1), 0, bufferFrame, commandbuffer);

	if (get_player_stats().toggleStats)
	{
		gf3d_sprite_draw(UI.menu.stats, vector2d(250, 175), vector2d(1.5, 1.5), 0, bufferFrame, commandbuffer);
		for (i = 0; i < UI.menu.text_sprite_count; i++)
		{
			
			if (UI.menu.text[i])
				gf3d_sprite_draw(UI.menu.text[i], vector2d(x_pos, y_pos), vector2d(1, 1), 0, bufferFrame, commandbuffer);
			
			y_pos += 20;
		}
	}

	if (get_player_stats().toggleMap)
		gf3d_sprite_draw(UI.menu.map, vector2d(0, 0), vector2d(1, 1), 0, bufferFrame, commandbuffer);



}

void rpg_ui_update()
{
	life_ratio = (float)get_player_stats().life / (float)get_player_stats().life_max;
	mana_ratio = (float)get_player_stats().mana / (float)get_player_stats().mana_max;
	stamina_ratio = (float)get_player_stats().stamina / (float)get_player_stats().stamina_max;


}

Sprite *rpg_ui_text_stats(char* name, char* text)
{
	TTF_Font *font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 32);
	SDL_Color color;
	color.a = 1;
	color.r = 255;
	color.g = 255;
	color.b = 255;

	SDL_Surface	*surface = TTF_RenderText_Blended(font, text, color);
	Sprite *sprite = gf3d_sprite_load(name, surface, -1, -1, 0);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return sprite;
}

/*eol@eof*/

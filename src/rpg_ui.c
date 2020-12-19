
//#include <string>

#include "simple_logger.h"
#include "simple_json.h"

#include "SDL_ttf.h"

#include "gfc_types.h"
#include "gfc_text.h"

#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_player.h"
#include "rpg_npc.h"
#include "rpg_ui.h"

typedef struct
{
	HUD hud;
	Menu menu;
	ShopUI shop;
	TTF_Font *font;
}UIManager;

static UIManager UI = { 0 };

static SJson  *dialog_info	= NULL;
static SJson  *quest_info	= NULL;

float life_ratio;
float mana_ratio;
float stamina_ratio;

float old_x, old_y;

Sprite *rpg_ui_text_stats(char* name, char* text);
void setup_hud();
void setup_npc_ui();

void rpg_ui_init()
{	
	slog("Initializing UI...");

	old_x = 0;
	old_y = 0;

	UI.hud.base		  = NULL;
	UI.hud.health	  = NULL;
	UI.hud.mana		  = NULL;
	UI.hud.stamina	  = NULL;
	UI.menu.stats	  = NULL;
	UI.menu.map       = NULL;
	UI.menu.map_point = NULL;
	UI.menu.text	  = NULL;

	UI.menu.point_position = vector2d(75, 65);

	life_ratio	  = 0;
	mana_ratio	  = 0;
	stamina_ratio = 0;

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

	UI.menu.text_count = 17;
	UI.menu.text = gfc_allocate_array(sizeof(Sprite*), UI.menu.text_count);

	UI.menu.white.a = 1;
	UI.menu.white.r = 255;
	UI.menu.white.g = 255;
	UI.menu.white.b = 255;

	setup_hud();

	setup_npc_ui();
	
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
		for (i = 0; i < UI.menu.text_count; i++)
		{
			
			if (UI.menu.text[i])
				gf3d_sprite_draw(UI.menu.text[i], vector2d(x_pos, y_pos), vector2d(1, 1), 0, bufferFrame, commandbuffer);
			
			y_pos += 20;
		}
	}

	if (get_player_stats().toggleMap)
	{
		gf3d_sprite_draw(UI.menu.map, vector2d(100, 0), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		gf3d_sprite_draw(UI.menu.map_point, UI.menu.point_position, vector2d(.3, .3), 0, bufferFrame, commandbuffer);
	}

	if (get_player_stats().toggleShop)
	{
		slog("Toggling Store");
		if (rpg_get_itemshop_toggle())
		{
			slog("Drawing sprite store");
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
		else if (rpg_get_weaponshop_toggle())
		{
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
		else if (rpg_get_armorshop_toggle())
		{
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
		else if (rpg_get_spellshop_toggle())
		{
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
		else if (rpg_get_generic_toggle())
		{
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
		else if (rpg_get_quest_toggle())
		{
			gf3d_sprite_draw(UI.shop.menu, vector2d(400, 175), vector2d(1, 1), 0, bufferFrame, commandbuffer);
		}
	}

}

void rpg_ui_update()
{
	life_ratio = (float)get_player_stats().life / (float)get_player_stats().life_max;
	mana_ratio = (float)get_player_stats().mana / (float)get_player_stats().mana_max;
	stamina_ratio = (float)get_player_stats().stamina / (float)get_player_stats().stamina_max;

	if (old_x != get_player()->ent->position.x || old_y != get_player()->ent->position.z)
	{
		old_x = get_player()->ent->position.x+500;
		old_y = get_player()->ent->position.z+150;
		UI.menu.point_position = vector2d(old_x/5, old_y/5);

	}
}

Sprite *rpg_ui_text(char* name, char* text)
{
	TTF_Font *font = TTF_OpenFont("fonts/OfenbacherSchwabCAT.ttf", 32);
	if (!font)
	{
		slog("Font null");
	}
	SDL_Color color;
	color.a = 1;
	color.r = 255;
	color.g = 255;
	color.b = 255;

	SDL_Surface	*surface = TTF_RenderText_Blended(font, text, color);
	if (!surface)
	{
		slog("Surface null");
	}
	Sprite *sprite = gf3d_sprite_load(name, surface, -1, -1, 0);
	if (!sprite)
		slog("Sprite null");

//	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return sprite;
}

void setup_hud()
{
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

	UI.menu.map = gf3d_sprite_load("images/map.png", NULL, -1, -1, 0);
	if (!UI.menu.stats) slog("map menu sprite not loaded");

	UI.menu.map_point = gf3d_sprite_load("images/mappointer.png", NULL, -1, -1, 0);
	if (!UI.menu.stats) slog("map menu sprite not loaded");

	char buffer[26];

	strcpy(buffer, "Name: ");
	strcat(buffer, get_player()->stats.name);
	//	slog("Buffer 1: %s",buffer);
	UI.menu.text[0] = rpg_ui_text("NameText", buffer);

	char int_buffer[3];
	strcpy(buffer, "Level: ");
	_itoa(get_player()->stats.level, int_buffer, 10);
	strcat(buffer, int_buffer);
	//		slog("Buffer : %s", buffer);

	UI.menu.text[1] = rpg_ui_text("LevelText", buffer);

	strcpy(buffer, "Exp: ");
	_itoa(get_player()->stats.exp, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[2] = rpg_ui_text("ExpText", buffer);

	strcpy(buffer, "Bits: ");
	_itoa(get_player()->stats.bits, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[3] = rpg_ui_text("BitsText", buffer);

	strcpy(buffer, "Life: ");
	_itoa(get_player()->stats.life, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.life_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[4] = rpg_ui_text("LifeText", buffer);

	strcpy(buffer, "Mana: ");
	_itoa(get_player()->stats.mana, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.mana_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[5] = rpg_ui_text("ManaText", buffer);

	strcpy(buffer, "Stamina: ");
	_itoa(get_player()->stats.stamina, int_buffer, 10);
	strcat(buffer, int_buffer);
	strcat(buffer, "/");
	_itoa(get_player()->stats.stamina_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[6] = rpg_ui_text("StaminaText", buffer);

	char float_buffer[10];
	strcpy(buffer, "Carry Weight: ");
	_gcvt(get_player()->stats.carry_weight, 5, float_buffer);
	strcat(buffer, float_buffer);
	strcat(buffer, "/");
	_gcvt(get_player()->stats.carry_weight_max, 5, float_buffer);
	strcat(buffer, float_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[7] = rpg_ui_text("CarryText", buffer);

	strcpy(buffer, "Strength: ");
	_itoa(get_player()->stats.strength, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[8] = rpg_ui_text("StrengthText", buffer);

	strcpy(buffer, "Health: ");
	_itoa(get_player()->stats.health, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);
	
	UI.menu.text[9] = rpg_ui_text("HealthText", buffer);

	strcpy(buffer, "Perception: ");
	_itoa(get_player()->stats.perception, int_buffer, 10);
	strcat(buffer, int_buffer);
		slog("Buffer : %s", buffer);

	UI.menu.text[10] = rpg_ui_text("PerceptionText", buffer);

	strcpy(buffer, "Speed: ");
	_itoa(get_player()->stats.speed, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[11] = rpg_ui_text("SpeedText", buffer);

	strcpy(buffer, "Willpower: ");
	_itoa(get_player()->stats.willpower, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[12] = rpg_ui_text("WillpowerText", buffer);

	strcpy(buffer, "Dexterity: ");
	_itoa(get_player()->stats.dexterity, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[13] = rpg_ui_text("DexterityText", buffer);

	strcpy(buffer, "Intelligence: ");
	_itoa(get_player()->stats.intelligence, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[14] = rpg_ui_text("IntelligenceText", buffer);

	strcpy(buffer, "Luck: ");
	_itoa(get_player()->stats.luck, int_buffer, 10);
	strcat(buffer, int_buffer);
	//	slog("Buffer : %s", buffer);

	UI.menu.text[15] = rpg_ui_text("LuckText", buffer);
}

void setup_npc_ui()
{
	char buffer[100];

	dialog_info = sj_load("json/npc_dialog.json");
	if (!dialog_info){ slog("Dialog info json not found."); return; }

	quest_info = sj_load("json/quest.json");
	if (!dialog_info){ slog("Quest info json not found."); return; }

	UI.shop.menu = gf3d_sprite_load("images/shopmenu.png", NULL, -1, -1, 0);
	if (!UI.shop.menu) slog("shop menu sprite not loaded");

	//strcpy(buffer, "Name: ");
	//strcat(buffer, get_player()->stats.name);
	//UI.shop.text[0] = rpg_ui_text();



}

/*eol@eof*/

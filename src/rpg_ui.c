
//#include <string>

#include "game.h"
#include "simple_logger.h"
#include "simple_json.h"

#include "SDL_ttf.h"

#include "gfc_types.h"
#include "gfc_text.h"

#include "gf3d_vgraphics.h"
#include "gf3d_texture.h"

#include "rpg_player.h"
#include "rpg_quest.h"
#include "rpg_npc.h"
#include "rpg_ui.h"

typedef struct
{
	HUD hud;
	Menu menu;
	MainMenu main_menu;
	ShopUI shop;
	TTF_Font* font;
}UIManager;

static UIManager UI = { 0 };

static SJson  *dialog_info	= NULL;
static SJson  *quest_info	= NULL;

char file_path[70];

float life_ratio;
float mana_ratio;
float stamina_ratio;

float old_x, old_y;

Sprite *rpg_ui_text_stats(char* name, char* text);
void setup_hud();
void setup_npc_ui();
void rpg_menu_loading_screen();

void rpg_ui_close()
{
	memset(&UI, 0, sizeof(UIManager));
	slog("UIManager System Closed");
}

void rpg_destroy_main_menu()
{

}

bool isCursorHoveringButton(SDL_Rect button, int x, int y)
{
	return (x > button.x) && (x < button.x + button.w) &&
		(y > button.y) && (y < button.y + button.h) ? true : false;
}

char* rpg_ui_get_player_model_name()
{
	return UI.main_menu.model_name;
}

void rpg_main_menu_init()
{
	slog("initializing menu...");

	UI.main_menu.renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!UI.main_menu.renderer)
	{ 
		slog("Main menu renderer not created. SDL ERROR: %s", SDL_GetError()); 
		SDL_Quit(); exit(0); 
	}

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "images/mainmenu.png");

	UI.main_menu.bg_texture = IMG_LoadTexture(UI.main_menu.renderer, file_path);
	if (!UI.main_menu.bg_texture) { slog("Main menu texture not created."); SDL_Quit(); exit(0); }

	SDL_RenderClear(UI.main_menu.renderer);
	SDL_RenderCopy(UI.main_menu.renderer, UI.main_menu.bg_texture, NULL, NULL);

	SDL_ShowCursor(SDL_TRUE);

	UI.main_menu.button_1_rect.h = 80;
	UI.main_menu.button_1_rect.w = 150;
	UI.main_menu.button_1_rect.x = 250;
	UI.main_menu.button_1_rect.y = 230;

	UI.main_menu.button_2_rect.h = 80;
	UI.main_menu.button_2_rect.w = 150;
	UI.main_menu.button_2_rect.x = 250;
	UI.main_menu.button_2_rect.y = 350;

	SDL_RenderPresent(UI.main_menu.renderer);
	slog("Main Menu loaded");
}

void rpg_ui_init()
{	
	slog("Initializing UI...");

	dialog_info = NULL;
	quest_info = NULL;

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

	UI.menu.point_position = vector2d_create(75, 65);

	life_ratio	  = 0;
	mana_ratio	  = 0;
	stamina_ratio = 0;

	if (TTF_Init() < 0) {
		SDL_GetError();
	}

	slog("Loading font...");
	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "fonts/OfenbacherSchwabCAT.ttf");
	UI.font = TTF_OpenFont(file_path, 32);
	if (!UI.font)
	{
		slog("Font is NULL");
		slog(SDL_GetError());
	}

	UI.menu.text_count = 17;
	UI.menu.text = gfc_allocate_array(sizeof(Sprite*), UI.menu.text_count);

	UI.shop.text_count = 22;
	UI.shop.text = gfc_allocate_array(sizeof(Sprite*), UI.menu.text_count);

	UI.menu.white.a = 1;
	UI.menu.white.r = 255;
	UI.menu.white.g = 255;
	UI.menu.white.b = 255;


	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/npc_dialog.json");
	dialog_info = sj_load(file_path);

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/quest.json");
	quest_info = sj_load(file_path);

	setup_hud();

	setup_npc_ui();
	atexit(rpg_ui_close);
}

int rpg_menu_start_screen(int x, int y)
{
	int exit = 0;
	if (isCursorHoveringButton(UI.main_menu.button_1_rect, x, y))
	{
		slog("Pressed Start button");

		snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "images/charchoice.png");

		UI.main_menu.bg_texture = IMG_LoadTexture(UI.main_menu.renderer, file_path);
		if (!UI.main_menu.bg_texture)
		{
			slog("Character choice texture not created.");
		}
		SDL_RenderClear(UI.main_menu.renderer);
		

		SDL_RenderCopy(UI.main_menu.renderer, UI.main_menu.bg_texture, NULL, NULL);
		SDL_RenderPresent(UI.main_menu.renderer);
		exit = 1;
	}

	if (isCursorHoveringButton(UI.main_menu.button_2_rect, x, y))
	{
		slog("Pressed Quit button");
		SDL_DestroyRenderer(UI.main_menu.renderer);
		SDL_DestroyTexture(UI.main_menu.bg_texture);
		exit = -1;
	}
	return exit;
}

int rpg_menu_character_select_screen(int x, int y)
{
	int done = 0;
	if (isCursorHoveringButton(UI.main_menu.button_1_rect, x, y))
	{
		slog("Pressed Knight button");
		done = 1;
		UI.main_menu.model_name = "player";
		rpg_menu_loading_screen();
	}
	else if (isCursorHoveringButton(UI.main_menu.button_2_rect, x, y))
	{
		slog("Pressed Mage button");
		done = 1;
		UI.main_menu.model_name = "player2";
		rpg_menu_loading_screen();
	}
	return done;
}

void rpg_menu_loading_screen()
{
	SDL_RenderClear(UI.main_menu.renderer);
	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "images/loading.png");
	UI.main_menu.bg_texture = IMG_LoadTexture(UI.main_menu.renderer, file_path);
	if (!UI.main_menu.bg_texture) { slog("Loading texture not created."); SDL_Quit(); exit(0); }

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "images/load_icon.png");
	UI.main_menu.tex_loading = IMG_LoadTexture(UI.main_menu.renderer, file_path);
	
	VkExtent2D extent = gf3d_vgraphics_get_view_extent();

	UI.main_menu.button_1_rect.h = 0;
	UI.main_menu.button_1_rect.w = 0;
	UI.main_menu.button_1_rect.x = 0;
	UI.main_menu.button_1_rect.y = 0;

	UI.main_menu.button_2_rect.h = 0;
	UI.main_menu.button_2_rect.w = 0;
	UI.main_menu.button_2_rect.x = 0;
	UI.main_menu.button_2_rect.y = 0;

	UI.main_menu.load_rect.h = 150;
	UI.main_menu.load_rect.w = 150;
	UI.main_menu.load_rect.x = (extent.width - UI.main_menu.load_rect.w) / 2;
	UI.main_menu.load_rect.y = (extent.width - UI.main_menu.load_rect.h) / 2;

	SDL_RenderCopy(UI.main_menu.renderer, UI.main_menu.bg_texture, NULL, NULL);
	
	SDL_RenderCopyEx(UI.main_menu.renderer, UI.main_menu.tex_loading, NULL, &UI.main_menu.load_rect, 1, NULL, SDL_FLIP_NONE);
	
	SDL_RenderPresent(UI.main_menu.renderer);
	
}

int rpg_main_menu_load_screen(MainMenuScreen screen)
{
	SDL_Event event;
	int x, y;
	int done = 0;

	while (!done)
	{
		if (SDL_PollEvent(&event))
		{				
			x = event.motion.x;
			y = event.motion.y;
			if (event.type == SDL_MOUSEMOTION)
			{
				if (isCursorHoveringButton(UI.main_menu.button_1_rect, x, y))
				{
					slog("Hovering over button 1");
				}

				if (isCursorHoveringButton(UI.main_menu.button_2_rect, x, y))
				{
					slog("Hovering over button 2");
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					switch (screen)
					{
						case StartScreen:
							done = rpg_menu_start_screen(x, y);
							break;
						case CharacterSelectScreen:
							done = rpg_menu_character_select_screen(x, y);
							break;
						case LoadingScreen:
							rpg_menu_loading_screen();
							done = 1;
							break;
						default:
							slog("There is no menu loaded.");
					};
					
				}
			}
		}
	}
	return done;
}

void rpg_update_loading_texture(const float angle, const SDL_RendererFlip flip)
{
	SDL_RenderClear(UI.main_menu.renderer);
	SDL_RenderCopy(UI.main_menu.renderer, UI.main_menu.bg_texture, NULL, NULL);
	SDL_RenderCopyEx(UI.main_menu.renderer, UI.main_menu.tex_loading, NULL, &UI.main_menu.load_rect, angle, NULL, flip);
	SDL_RenderPresent(UI.main_menu.renderer);
}

void rpg_ui_draw_all()
{
	int i = 0;
	int x_pos = 110;
	int y_pos = 90;

	gf3d_sprite_draw(UI.hud.base, vector2d_create(10, 10), vector2d_create(1, 1), 0);
	gf3d_sprite_draw(UI.hud.health, vector2d_create(12, 13), vector2d_create(life_ratio , 1), 0);
	gf3d_sprite_draw(UI.hud.mana, vector2d_create(12, 31), vector2d_create(mana_ratio, 1), 0);
	gf3d_sprite_draw(UI.hud.stamina, vector2d_create(12, 51), vector2d_create(stamina_ratio, 1), 0);

	if (get_player_stats().toggleStats)
	{
		gf3d_sprite_draw(UI.menu.stats, vector2d_create(250, 175), vector2d_create(1.5, 1.5), 0);
		for (i = 0; i < UI.menu.text_count; i++)
		{
			
			if (UI.menu.text[i])
				gf3d_sprite_draw(UI.menu.text[i], vector2d_create(x_pos, y_pos), vector2d_create(1, 1), 0);
			
			y_pos += 20;
		}
	}

	if (get_player_stats().toggleMap)
	{
		gf3d_sprite_draw(UI.menu.map, vector2d_create(100, 0), vector2d_create(1, 1), 0);
		gf3d_sprite_draw(UI.menu.map_point, UI.menu.point_position, vector2d_create(.3, .3), 0);
	}

	if (get_player_stats().toggleShop)
	{
		//slog("Toggling Store");
		if (rpg_get_itemshop_toggle())
		{
			//slog("Item shop");
			gf3d_sprite_draw(UI.shop.menu, vector2d_create(400, 175), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[16], vector2d_create(420, 200), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[0], vector2d_create(420, 220), vector2d_create(1, 1), 0);
		}
		else if (rpg_get_weaponshop_toggle())
		{
			//slog("Weapon shop");
			gf3d_sprite_draw(UI.shop.menu, vector2d_create(400, 175), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[17], vector2d_create(420, 200), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[1], vector2d_create(420, 220), vector2d_create(1, 1), 0);
		}
		else if (rpg_get_armorshop_toggle())
		{
			//slog("Armor shop");
			gf3d_sprite_draw(UI.shop.menu, vector2d_create(400, 175), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[18], vector2d_create(420, 200), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[2], vector2d_create(420, 220), vector2d_create(1, 1), 0);
		}
		else if (rpg_get_spellshop_toggle())
		{
			//slog("Spellshop shop");
			gf3d_sprite_draw(UI.shop.menu, vector2d_create(400, 175), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[19], vector2d_create(420, 200), vector2d_create(1, 1), 0);
			gf3d_sprite_draw(UI.shop.text[3], vector2d_create(420, 220), vector2d_create(1, 1), 0);
		}
		else if (rpg_get_generic_toggle())
		{
			//slog("Talking with NPC");
			gf3d_sprite_draw(UI.shop.textbox, vector2d_create(430, 420), vector2d_create(1.6, 1.5), 0);
			gf3d_sprite_draw(UI.shop.text[20], vector2d_create(240, 320), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 1)
				gf3d_sprite_draw(UI.shop.text[4], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 2)
				gf3d_sprite_draw(UI.shop.text[5], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 3)
				gf3d_sprite_draw(UI.shop.text[6], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 4)
				gf3d_sprite_draw(UI.shop.text[7], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 5)
				gf3d_sprite_draw(UI.shop.text[8], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 6)
				gf3d_sprite_draw(UI.shop.text[9], vector2d_create(260, 340), vector2d_create(1, 1), 0);
		}

		else if (rpg_get_quest_toggle())
		{
			//slog("Talking to Questgiver");
			gf3d_sprite_draw(UI.shop.textbox, vector2d_create(430, 420), vector2d_create(1.6, 1.5), 0);
			gf3d_sprite_draw(UI.shop.text[21], vector2d_create(240, 320), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 1)
				gf3d_sprite_draw(UI.shop.text[10], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 2)
				gf3d_sprite_draw(UI.shop.text[11], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 3)
				gf3d_sprite_draw(UI.shop.text[12], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 4)
				gf3d_sprite_draw(UI.shop.text[13], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 5)
				gf3d_sprite_draw(UI.shop.text[14], vector2d_create(260, 340), vector2d_create(1, 1), 0);
			if (rpg_get_current_quest() == 6)
				gf3d_sprite_draw(UI.shop.text[15], vector2d_create(260, 340), vector2d_create(1, 1), 0);
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
		UI.menu.point_position = vector2d_create(old_x/5, old_y/5);
	}
}

Sprite *rpg_ui_text(char* name, char* text, Bool wrap)
{
	SDL_Surface	*surface = NULL;
	if (!UI.font)
	{
		slog("Font null");
	}

	if (!wrap)
		surface = TTF_RenderText_Blended(UI.font, text, UI.menu.white);
	else
		surface = TTF_RenderText_Blended_Wrapped(UI.font, text, UI.menu.white, 380);
	
	if (!surface)
	{
		slog("Surface null");
	}
	Sprite *sprite = gf3d_sprite_load(name, surface, -1, -1, 0);
	if (!sprite)
		slog("Sprite null");

	return sprite;
}

void load_npc_dialog()
{
	SJson *item_shop = sj_object_get_value(dialog_info, "itemshop");
	SJson *weapon_shop = sj_object_get_value(dialog_info, "weaponshop");
	SJson *armor_shop = sj_object_get_value(dialog_info, "armorshop");
	SJson *spell_shop = sj_object_get_value(dialog_info, "spellshop");
	SJson *generic_info = sj_object_get_value(dialog_info, "generic");
	SJson *quest_info = sj_object_get_value(dialog_info, "quest");

	UI.shop.text[0] = rpg_ui_text("Item Shop Text", sj_get_string_value(sj_object_get_value(item_shop, "1")), true);
	UI.shop.text[1] = rpg_ui_text("Weapon Shop Text", sj_get_string_value(sj_object_get_value(weapon_shop, "1")), true);
	UI.shop.text[2] = rpg_ui_text("Armor Shop Text", sj_get_string_value(sj_object_get_value(armor_shop, "1")), true);
	UI.shop.text[3] = rpg_ui_text("Spell Shop Text", sj_get_string_value(sj_object_get_value(spell_shop, "1")), true);

	UI.shop.text[4] = rpg_ui_text("Generic Text 1", sj_get_string_value(sj_object_get_value(generic_info, "1")), true);
	UI.shop.text[5] = rpg_ui_text("Generic Text 2", sj_get_string_value(sj_object_get_value(generic_info, "2")), false);
	UI.shop.text[6] = rpg_ui_text("Generic Text 3", sj_get_string_value(sj_object_get_value(generic_info, "3")), false);
	UI.shop.text[7] = rpg_ui_text("Generic Text 4", sj_get_string_value(sj_object_get_value(generic_info, "4")), false);
	UI.shop.text[8] = rpg_ui_text("Generic Text 5", sj_get_string_value(sj_object_get_value(generic_info, "5")), false);
	UI.shop.text[9] = rpg_ui_text("Generic Text 6", sj_get_string_value(sj_object_get_value(generic_info, "6")), false);

	UI.shop.text[10] = rpg_ui_text("Quest Text 1", sj_get_string_value(sj_object_get_value(quest_info, "1")), true);
	UI.shop.text[11] = rpg_ui_text("Quest Text 2", sj_get_string_value(sj_object_get_value(quest_info, "2")), true);
	UI.shop.text[12] = rpg_ui_text("Quest Text 3", sj_get_string_value(sj_object_get_value(quest_info, "3")), true);
	UI.shop.text[13] = rpg_ui_text("Quest Text 4", sj_get_string_value(sj_object_get_value(quest_info, "4")), true);
	UI.shop.text[14] = rpg_ui_text("Quest Text 5", sj_get_string_value(sj_object_get_value(quest_info, "5")), true);
	UI.shop.text[15] = rpg_ui_text("Quest Text 6", sj_get_string_value(sj_object_get_value(quest_info, "6")), true);

	UI.shop.text[16] = rpg_ui_text("Item Shop Name", "Item Shop", false);
	UI.shop.text[17] = rpg_ui_text("Weapon Shop Name", "Weapon Shop", false);
	UI.shop.text[18] = rpg_ui_text("Armor Shop Name", "Armor Shop", false);
	UI.shop.text[19] = rpg_ui_text("Spell Shop Name", "Spell Shop", false);
	UI.shop.text[20] = rpg_ui_text("Generic", "Generic", false);
	UI.shop.text[21] = rpg_ui_text("Questgiver", "Questgiver", false);

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
	char int_buffer[10];
	char float_buffer[10];

	strcpy(buffer, "Name: ");
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, get_player()->stats.name);
	UI.menu.text[0] = rpg_ui_text("NameText", buffer, false);
	
	strcpy(buffer, "Level: ");
	_itoa(get_player()->stats.level, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[1] = rpg_ui_text("LevelText", buffer, false);

	strcpy(buffer, "Exp: ");
	_itoa(get_player()->stats.exp, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[2] = rpg_ui_text("ExpText", buffer, false);

	strcpy(buffer, "Bits: ");
	_itoa(get_player()->stats.bits, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[3] = rpg_ui_text("BitsText", buffer, false);

	strcpy(buffer, "Life: ");
	_itoa(get_player()->stats.life, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s%s", buffer, "/", int_buffer);
	_itoa(get_player()->stats.life_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	UI.menu.text[4] = rpg_ui_text("LifeText", buffer, false);

	strcpy(buffer, "Mana: ");
	_itoa(get_player()->stats.mana, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s%s", buffer, "/", int_buffer);
	_itoa(get_player()->stats.mana_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	UI.menu.text[5] = rpg_ui_text("ManaText", buffer, false);

	strcpy(buffer, "Stamina: ");
	_itoa(get_player()->stats.stamina, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s%s", buffer, "/", int_buffer);
	_itoa(get_player()->stats.stamina_max, int_buffer, 10);
	strcat(buffer, int_buffer);
	UI.menu.text[6] = rpg_ui_text("StaminaText", buffer, false);

	strcpy(buffer, "Carry Weight: ");
	_gcvt(get_player()->stats.carry_weight, 5, float_buffer);
	snprintf(buffer, sizeof(buffer), "%s%s%s", buffer, "/", float_buffer);
	_gcvt(get_player()->stats.carry_weight_max, 5, float_buffer);
	strcat(buffer, float_buffer);
	UI.menu.text[7] = rpg_ui_text("CarryText", buffer, false);

	strcpy(buffer, "Strength: ");
	_itoa(get_player()->stats.strength, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[8] = rpg_ui_text("StrengthText", buffer, false);

	strcpy(buffer, "Health: ");
	_itoa(get_player()->stats.health, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[9] = rpg_ui_text("HealthText", buffer, false);

	strcpy(buffer, "Perception: ");
	_itoa(get_player()->stats.perception, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[10] = rpg_ui_text("PerceptionText", buffer, false);

	strcpy(buffer, "Speed: ");
	_itoa(get_player()->stats.speed, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[11] = rpg_ui_text("SpeedText", buffer, false);

	strcpy(buffer, "Willpower: ");
	_itoa(get_player()->stats.willpower, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[12] = rpg_ui_text("WillpowerText", buffer, false);

	strcpy(buffer, "Dexterity: ");
	_itoa(get_player()->stats.dexterity, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[13] = rpg_ui_text("DexterityText", buffer, false);

	strcpy(buffer, "Intelligence: ");
	_itoa(get_player()->stats.intelligence, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[14] = rpg_ui_text("IntelligenceText", buffer, false);
	
	strcpy(buffer, "Luck: ");
	_itoa(get_player()->stats.luck, int_buffer, 10);
	snprintf(buffer, sizeof(buffer), "%s%s", buffer, int_buffer);
	UI.menu.text[15] = rpg_ui_text("LuckText", buffer, false);

}

void setup_npc_ui()
{
	char buffer[100];

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/npc_dialog.json");
	dialog_info = sj_load(file_path);
	if (!dialog_info){ slog("Dialog info json not found."); return; }

	snprintf(file_path, sizeof(file_path), "%s%s", FILE_PATH, "json/quest.json");
	quest_info = sj_load(file_path);
	if (!dialog_info){ slog("Quest info json not found."); return; }

	UI.shop.menu = gf3d_sprite_load("images/shopmenu.png", NULL, -1, -1, 0);
	if (!UI.shop.menu) slog("shop menu sprite not loaded");

	UI.shop.textbox = gf3d_sprite_load("images/textbox.png", NULL, -1, -1, 0);
	if (!UI.shop.textbox) slog("text box sprite not loaded");

	load_npc_dialog();
}

/*eol@eof*/

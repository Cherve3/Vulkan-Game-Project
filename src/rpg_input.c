
#include "simple_logger.h"

#include "rpg_input.h"
#include "rpg_player.h"
#include "rpg_projectile.h"

void check_inputs(const Uint8 *keys)
{
	SDL_Event e;
	SDL_PollEvent(&e);

	int x, y;
	int x_rel, y_rel;

	Uint32 mouse_button = SDL_GetMouseState(&x, &y);
	SDL_GetRelativeMouseState(&x_rel, &y_rel);
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			break;
		}
	}
/*				slog("Move forward");
				
				break;
			case SDLK_a:
				slog("Move left");
				break;
			case SDLK_s:
				slog("Move backward");
				break;
			case SDLK_d:
				slog("Move right");
				break;
			case SDLK_TAB:
				slog("Stats");
				break;
			case SDLK_e:
				slog("Interact");
				break;
			case SDLK_i:
				slog("Inventory");
				break;
		}

		switch (e.button.button)
		{
			case SDL_BUTTON_LEFT:
				slog("Mouse left");
				break;
			case SDL_BUTTON_RIGHT:
				slog("Mouse Right");
				break;
		}
	}*/
}

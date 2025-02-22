#pragma once

#include <SDL3/SDL_render.h>

struct wlSprite {
	SDL_Texture *			texture{ nullptr };
	SDL_FRect				srcRect{};
	SDL_FRect				dstRect{};
	SDL_Color				color{};
	int						posZ{ 0 };
	float					scale{ 1.0f };

	double					angle{ 0.0 };
	SDL_FlipMode			flip{ SDL_FLIP_NONE };

	SDL_FPoint				center{};

	bool					isSimpleSprite{ true };
	bool					isHasCenter{ false };
};

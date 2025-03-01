#pragma once

#include "entt.hpp"
#include "SDL3_ttf/SDL_ttf.h"

enum class xAnchor_t : uint8_t {
	none = 0,
	left,
	right,
	center
};

enum class yAnchor_t : uint8_t {
	none = 0,
	top,
	bottom,
	center
};

struct wlUIElement {;
	SDL_FRect				dstRect{};
	wlVec2					position{};
	int						positionZ{0};
	xAnchor_t				xAnchor{ xAnchor_t::none };
	yAnchor_t				yAnchor{ yAnchor_t::none };

	wlVec2					scale{1.0f, 1.0f};

	SDL_FlipMode flip{ SDL_FLIP_NONE };
};

struct wlUIChild {
	entt::entity			parentEnt = entt::null;
};

struct wlUISprite {
	SDL_Texture* texture{ nullptr };
	SDL_FRect srcRect{};
	SDL_Color color{};

	SDL_FPoint center{};
	bool isHasCenter{ false };
};

struct wlButton {
	bool					isPressed{ false };

	std::function<void( entt::registry& )> onClick;
};

struct wlText {
	TTF_Text* text;
};

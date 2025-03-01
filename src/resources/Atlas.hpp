#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

struct wlSpriteAtlasData {
	SDL_FRect srcRect{};
};

class wlAtlas {
public:
	std::unordered_map<std::string, wlSpriteAtlasData> dataSprites;
	std::string pathToImage;

	SDL_Texture* texture = nullptr;

	const wlSpriteAtlasData&	GetSpriteData( const std::string& keySprite);
	void						ParseAtlas( const std::string& assetsPath, const std::string& atlasFile );
	void						Unload();
};

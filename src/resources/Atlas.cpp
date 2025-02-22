#include "Atlas.hpp"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "../components/RenderComponents.hpp"

#include "ResourcesUtils.hpp"

using json = nlohmann::json;


const wlSpriteAtlasData& wlAtlas::GetSpriteData( const std::string& keySprite ) {
	return dataSprites[keySprite];
}

void wlAtlas::ParseAtlas( const std::string& assetsPath, const std::string& atlasFile ) {
	const std::filesystem::path atlasJsonPath = std::filesystem::path( assetsPath ) / atlasFile;
	std::ifstream file( atlasJsonPath );
	if ( !file.is_open() ) {
		std::cerr << "Failed open file!" << std::endl;
		return;
	}

	json j;
	file >> j;

	if ( j.is_discarded() ) {
		std::cerr << "Failed parsing!" << std::endl;
		return;
	}

	for ( const auto& textureIt : j.items() ) {
		for ( const auto& frame : textureIt.value() ) {
			std::string filename = frame["name"];
			auto& spriteData = dataSprites[filename];
			spriteData.srcRect = SDL_FRect{ .x = frame["frame"]["x"],
											.y = frame["frame"]["y"],
											.w = frame["frame"]["w"],
											.h = frame["frame"]["h"] };
		}

		pathToImage = assetsPath + textureIt.key();
		if ( texture == nullptr ) {
			texture = loadTexture( pathToImage.c_str() );
		}
	}

	file.close();
}

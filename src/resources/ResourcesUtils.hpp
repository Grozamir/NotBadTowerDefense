#pragma once

#include <iostream>

#include "SDL3_image/SDL_image.h"

#include "../Application.hpp"

inline SDL_Texture* loadTexture( const char* filePath ) {
	bool ok = true;

	SDL_Texture* texture = nullptr;

	SDL_Surface* tempSurf = IMG_Load( filePath );
	if ( tempSurf == nullptr ) {
		std::cout << "Can't load image: " << SDL_GetError() << '\n';
		ok = false;
	} else {
		texture = SDL_CreateTextureFromSurface( appState->renderer, tempSurf );
		if ( texture == nullptr ) {
			std::cout << "Can't create texture from surface: " << SDL_GetError() << '\n';
			ok = false;
		} else {
			SDL_SetTextureScaleMode( texture, SDL_SCALEMODE_NEAREST );
		}
	}

	SDL_DestroySurface( tempSurf );

	if ( !ok ) {
		texture = nullptr;
	}

	return texture;
}
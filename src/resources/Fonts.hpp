#pragma once

#include "SDL3_ttf/SDL_ttf.h"

namespace wlFonts {

inline TTF_Font* mainFont;

inline void Load() {
	mainFont = TTF_OpenFont( "assets/Monocraft.ttf", 24 );
}

inline void Unload() {
	TTF_CloseFont( mainFont );
}

}
#include "Resources.hpp"

#include "Sprites.hpp"
#include "Fonts.hpp"

void wlResources::Load() {
	wlSprites::Load();
	wlFonts::Load();
}

void wlResources::Unload() {
	wlSprites::Unload();
	wlFonts::Unload();
}

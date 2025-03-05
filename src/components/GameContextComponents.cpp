#include "GameContextComponents.hpp"

#include <string>

#include "SDL3_ttf/SDL_ttf.h"

void wlGameState::UpdateTxtMoney() const {
	const std::string txt = std::to_string( money ) + "$";
	TTF_SetTextString( txtMoney, txt.data(), txt.length() );
}

#include "GameContextComponents.hpp"

#include <string>

#include <SDL3_ttf/SDL_ttf.h>

#include <entt.hpp>

void wlGameState::TakeDamage( entt::registry& reg, const int32_t damage ) {
	gateHealth -= damage;
	gateHealth = std::max( gateHealth, 0 );
	onDefeat(reg);
}

void wlGameState::UpdateTxtMoney() const {
	const std::string txt = std::to_string( money ) + "$";
	TTF_SetTextString( txtMoney, txt.data(), txt.length() );
}

#include "Rendering.hpp"

#include "../Application.hpp"

#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/GameContextComponents.hpp"

#include "../resources/Sprites.hpp"

wlAppState* appState;

void drawSprites( entt::registry& reg ) {
	reg.sort<wlSprite>( []( const wlSprite& lhs, const wlSprite& rhs ) { return lhs.posZ < rhs.posZ; } );
	for ( auto [ent, sprite] : reg.view<const wlSprite>().each() ) {
		if ( sprite.isSimpleSprite ) {
			SDL_RenderTexture( appState->renderer, sprite.texture, &sprite.srcRect, &sprite.dstRect );
		} else {
			SDL_RenderTextureRotated( appState->renderer, sprite.texture, &sprite.srcRect, &sprite.dstRect, sprite.angle,
									  sprite.isHasCenter ? &sprite.center : nullptr, sprite.flip );
		}
	}
}

void updateDstRectSprites( entt::registry& reg ) {
	const auto& offset = reg.ctx().get<wlCenteringOffset>();
	for ( auto [ent, pos, sprite] : reg.view<wlPosition, wlSprite>( ).each() ) {
		sprite.dstRect = {
			pos.value.x + offset.value.x,
			pos.value.y + offset.value.y,
			sprite.srcRect.w * sprite.scale,
			sprite.srcRect.h * sprite.scale
		};
	}
}
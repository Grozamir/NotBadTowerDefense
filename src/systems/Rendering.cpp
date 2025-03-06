#include "Rendering.hpp"

#include "../Application.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/UIComponents.hpp"
#include "../helpers/UIHelper.hpp"
#include "../resources/Sprites.hpp"

wlAppState* appState;

void DrawSprites( entt::registry& reg ) {
	reg.sort<wlSprite>( []( const wlSprite& lhs, const wlSprite& rhs ) { return lhs.posZ < rhs.posZ; } );
	for ( auto [ent, sprite] : reg.view<const wlSprite>().each() ) {
		if ( sprite.isSimpleSprite ) {
			SDL_RenderTexture( appState->renderer, sprite.texture, &sprite.srcRect, &sprite.dstRect );
		} else {
			SDL_RenderTextureRotated( appState->renderer, sprite.texture, &sprite.srcRect, &sprite.dstRect, sprite.angle,
									  sprite.isHasCenter ? &sprite.center : nullptr, sprite.flip );
		}
	}

	for ( auto [ent, sprite, pos] : reg.view<const wlText, wlPosition>().each() ) {
		TTF_DrawRendererText( sprite.text, pos.value.x, pos.value.y );
	}
}

void UpdateDstRectSprites( entt::registry& reg ) {
	const auto& offset = reg.ctx().get<wlCenteringOffset>();
	for ( auto [ent, pos, sprite] : reg.view<wlPosition, wlSprite>().each() ) {
		sprite.dstRect = {
			pos.value.x + offset.value.x,
			pos.value.y + offset.value.y,
			sprite.srcRect.w * sprite.scale,
			sprite.srcRect.h * sprite.scale };
	}
}

void DrawUIElements( entt::registry& reg ) {
	reg.sort<wlUIElement>( []( const wlUIElement& lhs, const wlUIElement& rhs ) { return lhs.positionZ < rhs.positionZ; } );
	for ( auto [ent, ui_element] : reg.view<const wlUIElement>( entt::exclude<wlHideUIElement> ).each() ) {
		if ( HasHiddenParent( reg, ent ) ) {
			continue;
		}

		if ( const auto sprite = reg.try_get<wlUISprite>( ent ); sprite ) {
			SDL_RenderTextureRotated( appState->renderer, sprite->texture, &sprite->srcRect, &ui_element.dstRect,
									  0.0f, sprite->isHasCenter ? &sprite->center : nullptr, ui_element.flip );
		}
		if ( const auto text = reg.try_get<wlText>( ent ) ) {
			TTF_DrawRendererText( text->text, ui_element.dstRect.x, ui_element.dstRect.y );
		}
	}
}

namespace {
void UpdateUIElementRecursively( entt::registry& reg, const entt::entity ent, const wlUIElement* parent = nullptr ) {
	auto& uiElement = reg.get<wlUIElement>( ent );

	SDL_FRect srcRect{ 0.0f, 0.0f, 1.0f, 1.0f };
	if ( const auto comp = reg.try_get<wlUISprite>( ent ) ) {
		srcRect = comp->srcRect;
	}
	if ( const auto comp = reg.try_get<wlText>( ent ) ) {
		int textWidth{ 0 };
		int textHeight{ 0 };
		TTF_GetTextSize( comp->text, &textWidth, &textHeight );
		srcRect.w = textWidth;
		srcRect.h = textHeight;
	}

	uiElement.dstRect = { 0.0f, 0.0f, srcRect.w * uiElement.scale.x, srcRect.h * uiElement.scale.y };

	if ( parent ) {
		if ( uiElement.xAnchor == xAnchor_t::left ) {
			uiElement.dstRect.x = parent->dstRect.x + uiElement.position.x;
		} else if ( uiElement.xAnchor == xAnchor_t::right ) {
			uiElement.dstRect.x = ( parent->dstRect.x + parent->dstRect.w - uiElement.dstRect.w ) - uiElement.position.x;
		} else if ( uiElement.xAnchor == xAnchor_t::center ) {
			uiElement.dstRect.x = ( parent->dstRect.x + ( parent->dstRect.w - uiElement.dstRect.w ) / 2.0f ) + uiElement.position.x;
		}

		if ( uiElement.yAnchor == yAnchor_t::top ) {
			uiElement.dstRect.y = parent->dstRect.y + uiElement.position.y;
		} else if ( uiElement.yAnchor == yAnchor_t::bottom ) {
			uiElement.dstRect.y = ( parent->dstRect.y + parent->dstRect.h - uiElement.dstRect.h ) - uiElement.position.y;
		} else if ( uiElement.yAnchor == yAnchor_t::center ) {
			uiElement.dstRect.y = ( parent->dstRect.y + ( parent->dstRect.h - uiElement.dstRect.h ) / 2.0f ) + uiElement.position.y;
		}
	} else {
		if ( uiElement.xAnchor == xAnchor_t::left ) {
			uiElement.dstRect.x = uiElement.position.x;
		} else if ( uiElement.xAnchor == xAnchor_t::right ) {
			uiElement.dstRect.x = appState->currentWidthScreen - uiElement.dstRect.w - uiElement.position.x;
		} else if ( uiElement.xAnchor == xAnchor_t::center ) {
			uiElement.dstRect.x = appState->currentWidthScreen / 2.0f + uiElement.position.x - uiElement.dstRect.w / 2.0f;
		}

		if ( uiElement.yAnchor == yAnchor_t::top ) {
			uiElement.dstRect.y = uiElement.position.y;
		} else if ( uiElement.yAnchor == yAnchor_t::bottom ) {
			uiElement.dstRect.y = appState->currentHeightScreen - uiElement.position.y - uiElement.dstRect.h;
		} else if ( uiElement.yAnchor == yAnchor_t::center ) {
			uiElement.dstRect.y = appState->currentHeightScreen / 2.0f - uiElement.position.y - uiElement.dstRect.h / 2.0f;
		}
	}

	for ( auto [childEnt, child] : reg.view<wlUIChild>().each() ) {
		if ( child.parentEnt == ent ) {
			UpdateUIElementRecursively( reg, childEnt, &uiElement );
		}
	}
}
}


void UpdateDstUIElements( entt::registry& reg ) {
	std::vector<entt::entity> rootElements;

	for ( auto ent : reg.view<wlUIElement>( entt::exclude<wlUIChild, wlHideUIElement> ) ) {
		rootElements.push_back( ent );
	}

	for (const auto ent : rootElements ) {
		UpdateUIElementRecursively( reg, ent );
	}
}
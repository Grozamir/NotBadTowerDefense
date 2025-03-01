#include "Rendering.hpp"

#include "../Application.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/UIComponents.hpp"
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
	for ( auto [ent, ui_element] : reg.view<const wlUIElement>().each() ) {
		wlUIElement current_elm = ui_element;
		if ( const auto child = reg.try_get<wlUIChild>( ent ); child ) {
			const auto parent_elm = reg.get<wlUIElement>( child->parentEnt );
		}
		if ( const auto sprite = reg.try_get<wlUISprite>( ent ); sprite ) {
			SDL_RenderTextureRotated( appState->renderer, sprite->texture, &sprite->srcRect, &current_elm.dstRect,
									  0.0f, sprite->isHasCenter ? &sprite->center : nullptr, current_elm.flip );
		}
		if ( const auto text = reg.try_get<wlText>( ent ) ) {
			TTF_DrawRendererText( text->text, current_elm.dstRect.x, current_elm.dstRect.y );
		}
	}
}

void UpdateDstUIElements( entt::registry& reg ) {
	for ( auto [ent, ui_element] : reg.view<wlUIElement>().each() ) {
		SDL_FRect srcRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		if ( const auto comp = reg.try_get<wlUISprite>( ent ) ) {
			srcRect = comp->srcRect;
		}
		if ( const auto comp = reg.try_get<wlText>( ent ) ) {
			int textWidth, textHeight;
			TTF_GetTextSize( comp->text, &textWidth, &textHeight );

			srcRect.w = textWidth;
			srcRect.h = textHeight;
		}

		ui_element.dstRect = { 0.0f,
							   0.0f,
							   srcRect.w * ui_element.scale.x,
							   srcRect.h * ui_element.scale.y };

		if ( ui_element.xAnchor == xAnchor_t::left ) {
			ui_element.dstRect.x = ui_element.position.x;
		} else if ( ui_element.xAnchor == xAnchor_t::right ) {
			ui_element.dstRect.x = appState->currentWidthScreen - ui_element.dstRect.w - ui_element.position.x;
		} else if ( ui_element.xAnchor == xAnchor_t::center ) {
			ui_element.dstRect.x =
				appState->currentWidthScreen / 2.0f + ui_element.position.x - ui_element.dstRect.w / 2.0f;
		} else {
			ui_element.dstRect.x = ui_element.position.x - ui_element.dstRect.w / 2.0f;
		}

		if ( ui_element.yAnchor == yAnchor_t::top ) {
			ui_element.dstRect.y = ui_element.position.y;
		} else if ( ui_element.yAnchor == yAnchor_t::bottom ) {
			ui_element.dstRect.y =
				appState->currentHeightScreen - ui_element.position.y - ui_element.dstRect.h;
		} else if ( ui_element.yAnchor == yAnchor_t::center ) {
			ui_element.dstRect.y =
				appState->currentHeightScreen / 2.0f - ui_element.position.y - ui_element.dstRect.h / 2.0f;
		} else {
			ui_element.dstRect.y = ui_element.position.y - ui_element.dstRect.h / 2.0f;
		}
	}

	for ( auto [ent, ui_element, child] : reg.view<wlUIElement, wlUIChild>().each() ) {
		const auto& parentElm = reg.get<wlUIElement>( child.parentEnt );

		SDL_FRect srcRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		if ( const auto comp = reg.try_get<wlUISprite>( ent ) ) {
			srcRect = comp->srcRect;
		}
		if ( const auto comp = reg.try_get<wlText>( ent ) ) {
			int textWidth, textHeight;
			TTF_GetTextSize( comp->text, &textWidth, &textHeight );

			srcRect.w = textWidth;
			srcRect.h = textHeight;
		}

		ui_element.dstRect = { 0.0f, 0.0f, srcRect.w * ui_element.scale.x,
							   srcRect.h * ui_element.scale.y };

		if ( ui_element.xAnchor == xAnchor_t::left ) {
			ui_element.dstRect.x = parentElm.dstRect.x + ui_element.position.x;
		} else if ( ui_element.xAnchor == xAnchor_t::right ) {
			ui_element.dstRect.x = ( parentElm.dstRect.x + parentElm.dstRect.w - ui_element.dstRect.w ) - ui_element.position.x;
		} else if ( ui_element.xAnchor == xAnchor_t::center ) {
			ui_element.dstRect.x = ( parentElm.dstRect.x + ( parentElm.dstRect.w - ui_element.dstRect.w ) / 2.0f ) + ui_element.position.x;
		}

		if ( ui_element.yAnchor == yAnchor_t::top ) {
			ui_element.dstRect.y = parentElm.dstRect.y + ui_element.position.y;
		} else if ( ui_element.yAnchor == yAnchor_t::bottom ) {
			ui_element.dstRect.y = ( parentElm.dstRect.y + parentElm.dstRect.h - ui_element.dstRect.h ) - ui_element.position.y;
		} else if ( ui_element.yAnchor == yAnchor_t::center ) {
			ui_element.dstRect.y = ( parentElm.dstRect.y + ( parentElm.dstRect.h - ui_element.dstRect.h ) / 2.0f ) + ui_element.position.y;
		}
	}
}

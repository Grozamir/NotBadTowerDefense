#include "UIHelper.hpp"

#include "../Application.hpp"
#include "../components/UIComponents.hpp"
#include "../resources/Fonts.hpp"
#include "../resources/Sprites.hpp"

bool HasHiddenParent( entt::registry& reg, entt::entity ent ) {
	while ( reg.valid( ent ) ) {
		if ( reg.any_of<wlHideUIElement>( ent ) ) {
			return true;
		}
		if ( const auto* child = reg.try_get<wlUIChild>( ent ) ) {
			ent = child->parentEnt;
		} else {
			break;
		}
	}
	return false;
}

entt::entity UIFactory::CreateButton( entt::registry& reg, const std::string& spriteName,
									  const wlVec2& scale, const xAnchor_t xAnchor, const yAnchor_t yAnchor, const wlVec2& position,
									  std::function<void( entt::registry& )> onClick, entt::entity parent, const int positionZ ) {
	const auto btnEnt = reg.create();
	auto& btn = reg.emplace<wlButton>( btnEnt );
	btn.onClick = std::move( onClick );

	auto& sprite = reg.emplace<wlUISprite>( btnEnt );
	sprite.texture = wlSprites::uiAtlas.texture;
	sprite.srcRect = wlSprites::uiAtlas.GetSpriteData( spriteName ).srcRect;

	auto& uiElm = reg.emplace<wlUIElement>( btnEnt );
	uiElm.scale = scale;
	uiElm.xAnchor = xAnchor;
	uiElm.yAnchor = yAnchor;
	uiElm.position = position;
	uiElm.positionZ = positionZ;

	if ( parent != entt::null ) {
		uiElm.positionZ += 1;
		reg.emplace<wlUIChild>( btnEnt, parent );
	}

	return btnEnt;
}

entt::entity UIFactory::CreateText( entt::registry& reg, const std::string& text, entt::entity parent,
									const xAnchor_t xAnchor, const yAnchor_t yAnchor, const int positionZ ) {
	const auto textEnt = reg.create();
	auto& textComp = reg.emplace<wlText>( textEnt );
	textComp.text = TTF_CreateText( appState->textEngine, wlFonts::mainFont, text.c_str(), 0 );

	auto& uiElm = reg.emplace<wlUIElement>( textEnt );
	uiElm.xAnchor = xAnchor;
	uiElm.yAnchor = yAnchor;
	uiElm.positionZ = positionZ;

	if ( parent != entt::null ) {
		uiElm.positionZ += 1;
		reg.emplace<wlUIChild>( textEnt, parent );
	}

	return textEnt;
}

entt::entity UIFactory::CreatePanel( entt::registry& reg, const std::string& spriteName,
									 const wlVec2& scale, const xAnchor_t xAnchor, const yAnchor_t yAnchor ) {
	const auto panelEnt = reg.create();
	auto& sprite = reg.emplace<wlUISprite>( panelEnt );
	sprite.texture = wlSprites::uiAtlas.texture;
	sprite.srcRect = wlSprites::uiAtlas.GetSpriteData( spriteName ).srcRect;

	auto& uiElm = reg.emplace<wlUIElement>( panelEnt );
	uiElm.scale = scale;
	uiElm.xAnchor = xAnchor;
	uiElm.yAnchor = yAnchor;

	return panelEnt;
}

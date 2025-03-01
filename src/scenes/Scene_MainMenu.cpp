#include "Scene_MainMenu.hpp"

#include "SDL3_ttf/SDL_ttf.h"

#include "SceneManager.hpp"

#include "../components/PhysicsComponents.hpp"
#include "../components/UIComponents.hpp"

#include "../systems/Rendering.hpp"
#include "../systems/UIEvents.hpp"

#include "../resources/Sprites.hpp"
#include "../resources/Fonts.hpp"

#include "../Application.hpp"

void wlScene_MainMenu::Start() {
	{
		const auto btnStartGameEnt = reg.create();
		{
			auto& btn = reg.emplace<wlButton>( btnStartGameEnt );

			btn.onClick = [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {
				sceneMgr->ChangeScene( sceneType_t::TEST_GAME );
			};

			auto& sprite = reg.emplace<wlUISprite>( btnStartGameEnt );
			sprite.texture = wlSprites::uiAtlas.texture;
			sprite.srcRect = wlSprites::uiAtlas.GetSpriteData( "btn_01" ).srcRect;

			auto& uiElm = reg.emplace<wlUIElement>( btnStartGameEnt );
			uiElm.scale = wlVec2{ 4.0f, 4.0f };
			uiElm.xAnchor = xAnchor_t::center;
			uiElm.yAnchor = yAnchor_t::center;
		}

		{
			const auto txtStartGameEnt = reg.create();
			auto& textComp = reg.emplace<wlText>( txtStartGameEnt );
			textComp.text = TTF_CreateText( appState->textEngine, wlFonts::mainFont, "Play", 0 );

			auto& uiElm = reg.emplace<wlUIElement>( txtStartGameEnt );
			uiElm.xAnchor = xAnchor_t::center;
			uiElm.yAnchor = yAnchor_t::center;
			uiElm.positionZ = 1;

			reg.emplace<wlUIChild>( txtStartGameEnt, btnStartGameEnt );
		}
	}

	UpdateDstUIElements( reg );
}

void wlScene_MainMenu::OnUpdate( double deltaTime ) {
	DrawUIElements( reg );
}

void wlScene_MainMenu::OnEvent( SDL_Event* event ) {

	if ( event->type == SDL_EVENT_WINDOW_RESIZED ) {
		UpdateDstUIElements( reg );
	}

	if ( HandleUIMouseEvent( reg, event ) ) {
		UpdateDstUIElements( reg );
	}
}

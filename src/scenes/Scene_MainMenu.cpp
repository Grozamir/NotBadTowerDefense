#include "Scene_MainMenu.hpp"

#include "SceneManager.hpp"

#include "../components/UIComponents.hpp"

#include "../systems/Rendering.hpp"
#include "../systems/UIEvents.hpp"

#include "../resources/Sprites.hpp"

#include "../Application.hpp"
#include "../helpers/UIHelper.hpp"

void wlScene_MainMenu::Start() {

	UIFactory::CreateText( reg, "Not Bad Tower Defense", entt::null,
						   xAnchor_t::center, yAnchor_t::top, 1, {0.0f, 150.0f}, { 255, 128, 13, 255 } );

	const auto btnStartGameEnt = UIFactory::CreateButton( reg, "btn_01", wlVec2{ 4.0f, 4.0f },
														  xAnchor_t::center, yAnchor_t::center, {},							  //
														  [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {  //
															  sceneMgr->ChangeScene( sceneType_t::TEST_GAME );				  //
														  } );

	UIFactory::CreateText( reg, "Play", btnStartGameEnt, xAnchor_t::center, yAnchor_t::center, 1);

	UpdateDstUIElements( reg );
}

void wlScene_MainMenu::OnUpdate( double deltaTime ) {
	SDL_SetRenderDrawColor( appState->renderer, 89, 89, 89, 255 );
	SDL_RenderClear( appState->renderer );

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

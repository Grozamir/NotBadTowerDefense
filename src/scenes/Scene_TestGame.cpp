#include "Scene_TestGame.hpp"

#include <SDL3/SDL_render.h>

#include "../Application.hpp"

#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/UIComponents.hpp"

#include "../helpers/SpawnHelper.hpp"
#include "../helpers/TowerBuldingHelper.hpp"
#include "../helpers/UIHelper.hpp"

#include "../resources/Sprites.hpp"

#include "../systems/BulletSystems.hpp"
#include "../systems/MovementSystems.hpp"
#include "../systems/Rendering.hpp"
#include "../systems/TowerSystems.hpp"
#include "../systems/UIEvents.hpp"

#include "DataMaps.hpp"
#include "SceneManager.hpp"

namespace {
void UpdateOffset( entt::registry& reg ) {
	const auto& levelState = reg.ctx().get<wlLevelState>();
	auto& offset = reg.ctx().get<wlCenteringOffset>();
	offset.value.x = appState->currentWidthScreen / 2.0f - levelState.offsetCell * levelState.GetSourceMap()[0].size() / 2.0f;
	offset.value.y = appState->currentHeightScreen / 2.0f - levelState.offsetCell * levelState.GetSourceMap().size() / 2.0f;
}
}  // namespace

void wlScene_TestGame::Start() {
	auto& levelState = reg.ctx().emplace<wlLevelState>();
	levelState.sourceMap = &game::level::TestLevel;
	levelState.currentMap = game::level::TestLevel;

	levelState.pathForEnemy = game::level::findPath( *levelState.sourceMap );

	{
		auto& sourceMap = levelState.GetSourceMap();
		for ( std::size_t i = 0; i < sourceMap.size(); i++ ) {
			for ( std::size_t j = 0; j < sourceMap[i].size(); j++ ) {
				const auto ent = reg.create();
				reg.emplace<wlPosition>( ent, wlVec2{ levelState.offsetCell * j, levelState.offsetCell * i } );
				auto& sprite = reg.emplace<wlSprite>( ent );
				sprite.texture = wlSprites::gameAtlas.texture;
				if ( sourceMap[i][j] == cellType_t::WALL ) {
					sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "place_for_tower_01" ).srcRect;
				} else if ( sourceMap[i][j] == cellType_t::FLOOR ) {
					sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
				} else if ( sourceMap[i][j] == cellType_t::START ) {
					sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
				} else {
					sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
				}
				sprite.scale = levelState.offsetCell / sprite.srcRect.w;
				sprite.posZ = -1;
			}
		}
	}

	reg.ctx().emplace<wlCenteringOffset>();
	UpdateOffset( reg );

	// UI

	auto& panelMaps = reg.ctx().emplace<wlPanelMaps>();

	// Button "Open Pause"
	UIFactory::CreateButton( reg, "pause_btn",
							 { 4.0f, 4.0f }, xAnchor_t::right, yAnchor_t::top, { 15.0f, 15.0f },
							 []( entt::registry& reg ) {
								 auto& gameState = reg.ctx().get<wlGameState>();
								 if (gameState.isPaused) {
									 return;
								 }
								 auto& panelMaps = reg.ctx().get<wlPanelMaps>();
								 reg.remove<wlHideUIElement>( panelMaps.panels["PauseMenu"] );

								 gameState.isPaused = true;
							 } );

	{  // Pause Menu
		const auto pauseMenuPanelEnt = UIFactory::CreatePanel( reg, "btn_01",
															   { 30.0f, 30.0f }, xAnchor_t::center, yAnchor_t::center );
		panelMaps.panels["PauseMenu"] = pauseMenuPanelEnt;
		reg.emplace<wlHideUIElement>( pauseMenuPanelEnt );

		// Title "Pause"
		UIFactory::CreateText( reg, "Pause", pauseMenuPanelEnt, xAnchor_t::center, yAnchor_t::top, 1 );

		// Button "Back To Game"
		const auto btnBackToGame = UIFactory::CreateButton( reg, "btn_01", { 7.0f, 4.0f },										//
															xAnchor_t::center, yAnchor_t::center, { 0.0f, -100.0f },			//
															[]( entt::registry& reg ) {											//
																auto& panelMaps = reg.ctx().get<wlPanelMaps>();					//
																reg.emplace<wlHideUIElement>( panelMaps.panels["PauseMenu"] );	//
																reg.ctx().get<wlGameState>().isPaused = false;					//
															},
															pauseMenuPanelEnt );

		UIFactory::CreateText( reg, "Back To Game", btnBackToGame, xAnchor_t::center, yAnchor_t::center, 1 );

		// Button "Main Menu"
		const auto btnMainMenuEnt = UIFactory::CreateButton( reg, "btn_01", { 7.0f, 4.0f },										 //
															 xAnchor_t::center, yAnchor_t::center, {},							 //
															 [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {	 //
																 sceneMgr->ChangeScene( sceneType_t::MAIN_MENU );				 //
															 },
															 pauseMenuPanelEnt );

		UIFactory::CreateText( reg, "Main Menu", btnMainMenuEnt, xAnchor_t::center, yAnchor_t::center, 2 );

		// Button "Restart Game"
		const auto btnRestartGame = UIFactory::CreateButton( reg, "btn_01", { 7.0f, 4.0f },										 //
															 xAnchor_t::center, yAnchor_t::center, { 0.0f, 100.0f },			 //
															 [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {	 //
																 sceneMgr->ChangeScene( sceneType_t::TEST_GAME );				 //
															 },
															 pauseMenuPanelEnt );

		UIFactory::CreateText( reg, "Restart Game", btnRestartGame, xAnchor_t::center, yAnchor_t::center, 2 );
	}

	{  // Defeat Menu
		const auto defeatMenuPanelEnt = UIFactory::CreatePanel( reg, "btn_01",
															   { 30.0f, 30.0f }, xAnchor_t::center, yAnchor_t::center );
		panelMaps.panels["DefeatMenu"] = defeatMenuPanelEnt;
		reg.emplace<wlHideUIElement>( defeatMenuPanelEnt );

		// Title "Defeat"
		UIFactory::CreateText( reg, "Defeat", defeatMenuPanelEnt, xAnchor_t::center, yAnchor_t::top, 1, {}, { 255, 0, 0, 255 } );

		// Button "Main Menu"
		const auto btnMainMenuEnt = UIFactory::CreateButton( reg, "btn_01", { 7.0f, 4.0f },										 //
															 xAnchor_t::center, yAnchor_t::center, {},							 //
															 [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {	 //
																 sceneMgr->ChangeScene( sceneType_t::MAIN_MENU );				 //
															 },
															 defeatMenuPanelEnt );

		UIFactory::CreateText( reg, "Main Menu", btnMainMenuEnt, xAnchor_t::center, yAnchor_t::center, 2 );

		// Button "Restart Game"
		const auto btnRestartGame = UIFactory::CreateButton( reg, "btn_01", { 7.0f, 4.0f },										 //
															 xAnchor_t::center, yAnchor_t::center, { 0.0f, 100.0f },			 //
															 [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {	 //
																 sceneMgr->ChangeScene( sceneType_t::TEST_GAME );				 //
															 },
															 defeatMenuPanelEnt );

		UIFactory::CreateText( reg, "Restart Game", btnRestartGame, xAnchor_t::center, yAnchor_t::center, 2 );
	}


	// Game State
	{
		auto& gameState = reg.ctx().emplace<wlGameState>();
		gameState.txtMoney = reg.get<wlText>( UIFactory::CreateText( reg, "...",
							entt::null, xAnchor_t::left, yAnchor_t::top, 1, { 25.0f, 10.0f } ) ).text;

		gameState.SetMoney( 20 );

		gameState.onDefeat = []( entt::registry& reg ) {
			auto& panelMaps = reg.ctx().get<wlPanelMaps>();
			reg.remove<wlHideUIElement>( panelMaps.panels["DefeatMenu"] );
			reg.ctx().get<wlGameState>().isPaused = true;

			UpdateDstUIElements( reg );
		};
	}

	UpdateDstUIElements( reg );
}

void wlScene_TestGame::OnUpdate( const double deltaTime ) {
	auto& levelState = reg.ctx().get<wlLevelState>();

	static double currentTimeForSpawn;
	currentTimeForSpawn += deltaTime;

	if ( !reg.ctx().get<wlGameState>().isPaused ) {
		if ( currentTimeForSpawn > 1.0 ) {
			currentTimeForSpawn = 0.0;
			SpawnBaseEnemy( reg, wlVec2{
									 levelState.pathForEnemy[0].second * levelState.offsetCell - 50.0f,
									 levelState.pathForEnemy[0].first * levelState.offsetCell + 50.0f } );
		}

		UpdatePosition( reg, deltaTime );
		UpdatePathFollowing( reg );

		// game logic

		UpdateTowerTargetSelection( reg );

		UpdateTowerAttack( reg, deltaTime );

		UpdateTowerLookAtEnemy( reg );

		UpdateBulletTracking( reg );

		UpdateEnemyHealthOnHit( reg );

		UpdateDestroyBulletOnInvalidTarget( reg );
	}

	// rendering

	SDL_SetRenderDrawColor( appState->renderer, 95, 84, 84, 255 );
	SDL_RenderClear( appState->renderer );

	UpdateDstRectSprites( reg );
	DrawSprites( reg );

	DrawUIElements( reg );

	SDL_SetRenderDrawColor( appState->renderer, 100, 100, 100, 255 );
}

void wlScene_TestGame::OnEvent( SDL_Event* event ) {
	if ( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == 1 ) {
		TryBuildTower( reg, event );
	} else if ( event->type == SDL_EVENT_WINDOW_RESIZED ) {
		UpdateOffset( reg );

		UpdateDstUIElements( reg );
	}

	if ( HandleUIMouseEvent( reg, event ) ) {
		UpdateDstUIElements( reg );
	}
}

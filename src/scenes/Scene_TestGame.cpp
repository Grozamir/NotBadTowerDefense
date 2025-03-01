#include "Scene_TestGame.hpp"

#include <SDL3/SDL_render.h>

#include "../Application.hpp"

#include "DataMaps.hpp"
#include "SceneManager.hpp"

#include "../components/GameComponents.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../components/UIComponents.hpp"

#include "../helpers/SpawnHelper.hpp"
#include "../resources/Sprites.hpp"

#include "../systems/MovementSystems.hpp"
#include "../systems/TowerSystems.hpp"
#include "../systems/BulletSystems.hpp"
#include "../systems/Rendering.hpp"
#include "../systems/UIEvents.hpp"


namespace {
void updateOffset( entt::registry& reg ) {
	const auto& levelState = reg.ctx().get<wlLevelState>();
	auto& offset = reg.ctx().get<wlCenteringOffset>();
	offset.value.x = appState->currentWidthScreen / 2.0f - levelState.offsetCell * levelState.GetSourceMap()[0].size() / 2.0f;
	offset.value.y = appState->currentHeightScreen / 2.0f - levelState.offsetCell * levelState.GetSourceMap().size() / 2.0f;
}
}

void wlScene_TestGame::Start() {
	auto& levelState = reg.ctx().emplace<wlLevelState>();
	levelState.sourceMap = &game::level::TestLevel;
	levelState.currentMap = game::level::TestLevel;

	levelState.pathForEnemy = game::level::findPath( *levelState.sourceMap);

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
	updateOffset( reg );

	{
		const auto btnPauseEnt = reg.create();
		auto& btn = reg.emplace<wlButton>( btnPauseEnt );

		btn.onClick = [sceneMgr = appState->app->GetSceneMgr()]( entt::registry& reg ) {
			sceneMgr->ChangeScene( sceneType_t::MAIN_MENU );
		};

		auto& sprite = reg.emplace<wlUISprite>( btnPauseEnt );
		sprite.texture = wlSprites::uiAtlas.texture;
		sprite.srcRect = wlSprites::uiAtlas.GetSpriteData( "pause_btn" ).srcRect;

		auto& uiElm = reg.emplace<wlUIElement>( btnPauseEnt );
		uiElm.scale = wlVec2{ 4.0f, 4.0f };
		uiElm.xAnchor = xAnchor_t::right;
		uiElm.yAnchor = yAnchor_t::top;
		uiElm.position = wlVec2{ 15.0f, 15.0f };
	}

	UpdateDstUIElements( reg );
}

void wlScene_TestGame::OnUpdate( const double deltaTime ) {
	auto& levelState = reg.ctx().get<wlLevelState>();

	static double currentTimeForSpawn;
	currentTimeForSpawn += deltaTime;

	if (currentTimeForSpawn > 1.0) {
		currentTimeForSpawn = 0.0;
		spawnBaseEnemy( reg, wlVec2{
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

	// rendering

	UpdateDstRectSprites( reg );
	DrawSprites( reg );

	DrawUIElements( reg );
}

void wlScene_TestGame::OnEvent( SDL_Event* event ) {
	if ( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == 1 ) {
		const auto& offset = reg.ctx().get<wlCenteringOffset>();
		auto& levelState = reg.ctx().get<wlLevelState>();
		const float mousePositionX = event->button.x - offset.value.x;
		const float mousePositionY = event->button.y - offset.value.y;

		const int32_t indexCellX = mousePositionX / levelState.offsetCell;
		const int32_t indexCellY = mousePositionY / levelState.offsetCell;

		const auto& sourceMap = levelState.GetSourceMap();
		if ( sourceMap.size() > indexCellY && sourceMap.at( indexCellY ).size() > indexCellX ) {
			if ( sourceMap[indexCellY][indexCellX] == cellType_t::WALL &&
				std::ranges::find( levelState.posTowers, std::pair{ indexCellY, indexCellX } ) == levelState.posTowers.end() ) {
				levelState.currentMap[indexCellY][indexCellX] = cellType_t::TOWER;

				const auto towerEnt = reg.create();
				reg.emplace<wlPosition>( towerEnt, wlVec2{	levelState.offsetCell * indexCellX + levelState.offsetCell / 4.0f,
															levelState.offsetCell * indexCellY + levelState.offsetCell / 4.0f } );
				reg.emplace<wlTower>( towerEnt, 0.2f );
				auto& sprite = reg.emplace<wlSprite>( towerEnt );
				sprite.texture = wlSprites::gameAtlas.texture;
				sprite.srcRect = wlSprites::gameAtlas.GetSpriteData( "base_tower" ).srcRect;
				sprite.isSimpleSprite = false;
				sprite.scale = 6.0f;

				levelState.posTowers.push_back( { indexCellY, indexCellX } );
			}
		}
	} else if ( event->type == SDL_EVENT_WINDOW_RESIZED ) {
		updateOffset( reg );

		UpdateDstUIElements( reg );
	}

	if ( HandleUIMouseEvent( reg, event ) ) {
		UpdateDstUIElements( reg );
	}
}

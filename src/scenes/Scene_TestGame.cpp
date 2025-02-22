#include "Scene_TestGame.hpp"

#include <SDL3/SDL_render.h>

#include "../Application.hpp"

#include "DataMaps.hpp"

#include "../components/GameComponents.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../helpers/SpawnHelper.hpp"
#include "../resources/Sprites.hpp"

#include "../systems/MovementSystems.hpp"
#include "../systems/TowerSystems.hpp"
#include "../systems/BulletSystems.hpp"
#include "../systems/Rendering.hpp"


constexpr float offsetCell = 150.0f;

std::vector<wlCellPos> posTowers;

void wlScene_TestGame::Start() {
	auto& levelState = reg.ctx().emplace<wlLevelState>();
	levelState.sourceMap = &game::level::TestLevel;
	levelState.currentMap = game::level::TestLevel;

	levelState.pathForEnemy = game::level::findPath( *levelState.sourceMap);

	for ( std::size_t i = 0; i < levelState.sourceMap->size(); i++ ) {
		for ( std::size_t j = 0; j < ( *levelState.sourceMap )[i].size(); j++ ) {
			const auto ent = reg.create();
			reg.emplace<wlPosition>( ent, wlVec2{ offsetCell * j, offsetCell * i } );
			auto& sprite = reg.emplace<wlSprite>( ent );
			sprite.texture = sprites::gameAtlas.texture;
			if ( ( *levelState.sourceMap )[i][j] == cellType_t::WALL ) {
				sprite.srcRect = sprites::gameAtlas.GetSpriteData( "place_for_tower_01" ).srcRect;
			} else if ( ( *levelState.sourceMap )[i][j] == cellType_t::FLOOR ) {
				sprite.srcRect = sprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
			} else if ( ( *levelState.sourceMap )[i][j] == cellType_t::START ) {
				sprite.srcRect = sprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
			} else {
				sprite.srcRect = sprites::gameAtlas.GetSpriteData( "ground_01" ).srcRect;
			}
			sprite.scale = offsetCell / sprite.srcRect.w;
			sprite.posZ = -1;
		}
	}
}

void wlScene_TestGame::OnUpdate( const double deltaTime ) {
	auto& levelState = reg.ctx().get<wlLevelState>();

	static double currentTimeForSpawn;
	currentTimeForSpawn += deltaTime;

	if (currentTimeForSpawn > 1.0) {
		currentTimeForSpawn = 0.0;
		spawnBaseEnemy( reg, wlVec2{
					levelState.pathForEnemy[0].second * offsetCell - 50.0f,
					levelState.pathForEnemy[0].first * offsetCell + 50.0f } );
	}

	updatePosition( reg, deltaTime );
	updatePathFollowing( reg );

	// game logic

	updateTowerTargetSelection( reg );

	updateTowerAttack( reg, deltaTime );

	updateTowerLookAtEnemy( reg );

	updateBulletTracking( reg );

	updateEnemyHealthOnHit( reg );

	updateDestroyBulletOnInvalidTarget( reg );

	// rendering

	updateDstRectSprites( reg );
	drawSprites( reg );
}

void wlScene_TestGame::OnEvent( SDL_Event* event ) {
	if ( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == 1) {
		auto& levelState = reg.ctx().get<wlLevelState>();
		const float mousePositionX = event->button.x;
		const float mousePositionY = event->button.y;

		const int32_t indexCellX = mousePositionX / offsetCell;
		const int32_t indexCellY = mousePositionY / offsetCell;

		if ( levelState.sourceMap->size() > indexCellY && levelState.sourceMap->at( indexCellY ).size() > indexCellX ) {
			if ( ( *levelState.sourceMap )[indexCellY][indexCellX] == cellType_t::WALL && std::ranges::find( posTowers, std::pair{ indexCellY, indexCellX } ) == posTowers.end() ) {
				SDL_SetRenderDrawColor( appState->renderer, 0, 200, 0, 255 );
				levelState.currentMap[indexCellY][indexCellX] = cellType_t::TOWER;

				const auto towerEnt = reg.create();
				reg.emplace<wlPosition>( towerEnt, wlVec2{ offsetCell * indexCellX + offsetCell / 4.0f, offsetCell * indexCellY + offsetCell / 4.0f } );
				reg.emplace<wlTower>( towerEnt, 0.2f );
				auto& sprite = reg.emplace<wlSprite>( towerEnt );
				sprite.texture = sprites::gameAtlas.texture;
				sprite.srcRect = sprites::gameAtlas.GetSpriteData( "base_tower" ).srcRect;
				sprite.isSimpleSprite = false;
				sprite.scale = 6.0f;
				posTowers.push_back( { indexCellY, indexCellX } );
			}
		}
	}
}

#include "Scene_TestGame.hpp"

#include <SDL3/SDL_render.h>

#include <iostream>

#include "../Application.hpp"

#include "DataMaps.hpp"

#include "../components/GameComponents.hpp"
#include "../components/GameContextComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/NavigationComponents.hpp"
#include "../helpers/PhysicsHelper.hpp"
#include "../helpers/SpawnHelper.hpp"

#include "../systems/MovementSystems.hpp"
#include "../systems/TowerSystems.hpp"
#include "../systems/BulletSystems.hpp"

wlAppState* appState;

constexpr float offsetCell = 150.0f;

std::vector<wlCellPos> posTowers;

void wlScene_TestGame::Start() {
	auto& levelState = reg.ctx().emplace<wlLevelState>();
	levelState.sourceMap = &game::level::TestLevel;
	levelState.currentMap = game::level::TestLevel;

	levelState.pathForEnemy = game::level::findPath( *levelState.sourceMap);
}

void wlScene_TestGame::OnUpdate( const double deltaTime ) {
	auto& levelState = reg.ctx().get<wlLevelState>();

	static double currentTimeForSpawn;
	currentTimeForSpawn += deltaTime;

	if (currentTimeForSpawn > 1.0) {
		currentTimeForSpawn = 0.0;
		const auto enemyEnt = reg.create();
		reg.emplace<wlEnemy>( enemyEnt, 25.0f );
		reg.emplace<wlHealth>( enemyEnt, 10.0f );
		reg.emplace<wlPosition>( enemyEnt, wlVec2{ levelState.pathForEnemy[0].second * offsetCell - 50.0f, levelState.pathForEnemy[0].first * offsetCell + 50.0f } );
		reg.emplace<wlVelocity>( enemyEnt, wlVec2{ 0.0f, 0.0f }, 100.0f );
		reg.emplace<wlPathFollower>( enemyEnt, levelState.pathForEnemy );
	}

	updatePosition( reg, deltaTime );
	updatePathFollowing( reg );


	// game logic

	updateTowerTargetSelection( reg );

	updateTowerAttack( reg, deltaTime );

	updateBulletTracking( reg );

	updateEnemyHealthOnHit( reg );

	updateDestroyBulletOnInvalidTarget( reg );

	// rendering

	for ( std::size_t i = 0; i < levelState.sourceMap->size(); i++ ) {
		for ( std::size_t j = 0; j < ( *levelState.sourceMap )[i].size(); j++ ) {
			if ( ( *levelState.sourceMap )[i][j] == cellType_t::WALL ) {
				SDL_SetRenderDrawColor( appState->renderer, 255, 0, 0, 255 );
			} else if ( ( *levelState.sourceMap )[i][j] == cellType_t::FLOOR ) {
				SDL_SetRenderDrawColor( appState->renderer, 0, 220, 0, 255 );
			} else if ( ( *levelState.sourceMap )[i][j] == cellType_t::START ) {
				SDL_SetRenderDrawColor( appState->renderer, 0, 0, 250, 255 );
			} else {
				SDL_SetRenderDrawColor( appState->renderer, 0, 0, 0, 255 );
			}
			SDL_FRect rect{ offsetCell * j, offsetCell * i, offsetCell, offsetCell };
			SDL_RenderFillRect( appState->renderer, &rect );
		}
	}

	SDL_SetRenderDrawColor( appState->renderer, 255, 0, 0, 255 );
	for ( auto&& [ent, pos, enemy] : reg.view<const wlPosition, const wlEnemy>().each() ) {
		SDL_FRect rect{ pos.value.x, pos.value.y, 50.0f, 50.0f };
		SDL_RenderFillRect( appState->renderer, &rect );
	}

	SDL_SetRenderDrawColor( appState->renderer, 0, 150, 0, 255 );
	for ( auto&& [ent, pos, tower] : reg.view<const wlPosition, const wlTower>().each() ) {
		SDL_FRect rect{ pos.value.x, pos.value.y, offsetCell / 2.0f, offsetCell / 2.0f };
		SDL_RenderFillRect( appState->renderer, &rect );
	}

	SDL_SetRenderDrawColor( appState->renderer, 200, 200, 200, 255 );
	for ( auto&& [ent, pos, bullet] : reg.view<const wlPosition, const wlBullet>().each() ) {
		SDL_FRect rect{ pos.value.x, pos.value.y, 10.0f, 10.0f };
		SDL_RenderFillRect( appState->renderer, &rect );
	}
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
				posTowers.push_back( { indexCellY, indexCellX } );
			}
		}
	}
}

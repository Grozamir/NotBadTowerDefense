#include "TowerSystems.hpp"

#include <corecrt_math_defines.h>

#include "../components/GameComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../components/RenderComponents.hpp"
#include "../helpers/PhysicsHelper.hpp"
#include "../helpers/SpawnHelper.hpp"

void UpdateTowerTargetSelection( entt::registry& reg ) {
	for ( auto&& [towerEnt, towerPos, tower] : reg.view<const wlPosition, wlTower>().each() ) {
		// check current targetEnemy
		if ( reg.valid( tower.targetEnemy ) && reg.all_of<wlPosition, wlEnemy>( tower.targetEnemy ) ) {
			const auto targetEnemyPos = reg.get<wlPosition>( tower.targetEnemy ).value;
			const auto targetEnemy_radiusCollision = reg.get<wlEnemy>( tower.targetEnemy ).radiusCollision;
			if ( checkCollision( targetEnemyPos, towerPos.value, targetEnemy_radiusCollision, tower.radiusAttack ) ) {
				continue;
			}
		}

		// update targetEnemy
		bool foundEnemy = false;
		for ( auto&& [enemyEnt, enemyPos, enemy] : reg.view<const wlPosition, const wlEnemy>().each() ) {
			if ( checkCollision( enemyPos.value, towerPos.value, enemy.radiusCollision, tower.radiusAttack ) ) {
				foundEnemy = true;
				tower.targetEnemy = enemyEnt;
				break;
			}
		}

		if ( !foundEnemy ) {
			tower.targetEnemy = entt::null;
		}
	}
}

void UpdateTowerLookAtEnemy( entt::registry& reg ) {
	for ( auto&& [ent, pos, tower, sprite] : reg.view<const wlPosition, const wlTower, wlSprite>().each() ) {
		if ( reg.valid( tower.targetEnemy ) ) {
			const auto& posEnemy = reg.get<wlPosition>( tower.targetEnemy );
			const float dx = pos.value.x - posEnemy.value.x;
			const float dy = pos.value.y - posEnemy.value.y;

			sprite.angle = std::atan2( dy, dx ) * 180.0 / M_PI + 90.0;
		}
	}
}

void UpdateTowerAttack( entt::registry& reg, const double deltaTime ) {
	for ( auto&& [ent, pos, tower] : reg.view<const wlPosition, wlTower>().each() ) {
		tower.currentTimeForFire += deltaTime;

		if ( tower.currentTimeForFire > tower.fireRate ) {
			tower.currentTimeForFire = 0.0f;
			if ( reg.valid( tower.targetEnemy ) && reg.all_of<wlPosition, wlEnemy>( tower.targetEnemy ) ) {
				spawnBaseBullet( reg, tower.targetEnemy, pos.value + wlVec2{ 25.0f, 25.0f }, 1.0f, 5.0f, 800.0f );
			}
		}
	}
}

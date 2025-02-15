#include "TowerSystems.hpp"

#include "../components/GameComponents.hpp"
#include "../components/PhysicsComponents.hpp"
#include "../helpers/PhysicsHelper.hpp"
#include "../helpers/SpawnHelper.hpp"

void updateTowerTargetSelection( entt::registry& reg ) {
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

void updateTowerAttack( entt::registry& reg, const double deltaTime ) {
	for ( auto&& [ent, pos, tower] : reg.view<const wlPosition, wlTower>().each() ) {
		tower.currentTimeForFire += deltaTime;

		if ( tower.currentTimeForFire > tower.fireRate ) {
			tower.currentTimeForFire = 0.0f;
			if ( reg.valid( tower.targetEnemy ) && reg.all_of<wlPosition, wlEnemy>( tower.targetEnemy ) ) {
				spawnBaseBullet( reg, tower.targetEnemy, pos.value, 1.0f, 5.0f, 800.0f );
			}
		}
	}
}

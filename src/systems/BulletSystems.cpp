#include "BulletSystems.hpp"

#include "../components/GameComponents.hpp"
#include "../components/PhysicsComponents.hpp"

#include "../helpers/PhysicsHelper.hpp"

void updateBulletTracking( entt::registry& reg ) {
	for ( auto&& [ent, pos, vel, bullet] : reg.view<wlPosition, wlVelocity, wlBullet>().each() ) {
		if ( reg.valid( bullet.targetEnemy ) && reg.all_of<wlPosition, wlEnemy>( bullet.targetEnemy ) ) {
			auto targetEnemyPos = reg.get<wlPosition>( bullet.targetEnemy ).value;
			wlVec2 newDir = targetEnemyPos - pos.value;
			newDir.Normalize();
			vel.direction = newDir;

		} else {
			reg.destroy( ent );
		}
	}
}

void updateEnemyHealthOnHit( entt::registry& reg ) {
	for ( auto&& [enemyEnt, enemyPos, enemy, enemyHealth] : reg.view<const wlPosition, const wlEnemy, wlHealth>().each() ) {
		for ( auto&& [bulletEnt, bulletPos, bullet] : reg.view<const wlPosition, const wlBullet>().each() ) {
			if ( checkCollision( enemyPos.value, bulletPos.value, enemy.radiusCollision, bullet.radiusCollision ) ) {
				enemyHealth.currentHealth = std::max( enemyHealth.currentHealth - bullet.damage, 0.0f );
				if ( enemyHealth.currentHealth <= 0.01f ) {
					reg.destroy( enemyEnt );
				}

				reg.destroy( bulletEnt );

				break;
			}
		}
	}
}

void updateDestroyBulletOnInvalidTarget( entt::registry& reg ) {
	for ( auto&& [ent, pos, vel, bullet] : reg.view<wlPosition, wlVelocity, wlBullet>().each() ) {
		if ( !reg.valid( bullet.targetEnemy ) || !reg.all_of<wlPosition, wlEnemy>( bullet.targetEnemy ) ) {
			reg.destroy( ent );
		}
	}
}

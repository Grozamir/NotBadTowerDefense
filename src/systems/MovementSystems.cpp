#include "MovementSystems.hpp"

#include "../components/NavigationComponents.hpp"
#include "../components/PhysicsComponents.hpp"

void UpdatePosition( entt::registry& reg, const double deltaTime ) {
	for ( auto&& [ent, pos, vel] : reg.view<wlPosition, const wlVelocity>().each() ) {
		pos.value = pos.value + vel.direction * vel.speed * deltaTime;
	}
}

void UpdatePathFollowing( entt::registry& reg ) {
	for ( auto&& [ent, pos, vel, moveByPath] : reg.view<const wlPosition, wlVelocity, wlPathFollower>().each() ) {
		const auto& [targetColumn, targetRow] = moveByPath.path[moveByPath.currentTargetPointIndex];
		wlVec2 cellTargetPos{ targetRow * 150.0f + 50.0f, targetColumn * 150.0f + 50.0f };
		wlVec2 direction = cellTargetPos - pos.value;

		if ( direction.Length() > 0 ) {
			direction.Normalize();
			vel.direction = direction;
		}

		if ( ( pos.value - cellTargetPos ).Length() < 2.0f ) {
			if (!moveByPath.NextPoint()) {
				reg.destroy( ent );
				auto& gameState = reg.ctx().get<wlGameState>();
				gameState.TakeDamage( reg, 1 );
				continue;
			}

			if ( moveByPath.currentTargetPointIndex < moveByPath.path.size() ) {
				const auto& [targetColumn, targetRow] = moveByPath.path[moveByPath.currentTargetPointIndex];
				wlVec2 newTargetPos{ targetRow * 150.0f + 50.0f, targetColumn * 150.0f + 50.0f };
				wlVec2 newDirection = newTargetPos - pos.value;
				if ( newDirection.Length() > 0 ) {
					newDirection.Normalize();
				}
				vel.direction = newDirection;
			}
		}
	}
}

#include "SpawnHelper.hpp"

#include "../math/Vector.hpp"

#include "../components/GameComponents.hpp"
#include "../components/PhysicsComponents.hpp"


entt::entity spawnBaseBullet( entt::registry& reg, entt::entity targetEnemy, wlVec2 pos, float damage, float radiusCollision, float speed ) {
	const auto bulletEnt = reg.create();
	reg.emplace<wlBullet>( bulletEnt, damage, radiusCollision, targetEnemy );
	reg.emplace<wlPosition>( bulletEnt, pos );
	reg.emplace<wlVelocity>( bulletEnt, wlVec2{ 0.0f, 0.0f }, speed );
	return bulletEnt;
}

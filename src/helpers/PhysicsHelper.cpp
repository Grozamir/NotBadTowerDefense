#include "PhysicsHelper.hpp"

#include "../math/Vector.hpp"

bool CheckCollision( const wlVec2 posA, const wlVec2 posB, const float radiusA, const float radiusB ) {
	const float dx = posA.x - posB.x;
	const float dy = posA.y - posB.y;
	const float distSq = dx * dx + dy * dy;

	const float collisionDist = radiusA + radiusB;
	const float collisionDistSq = collisionDist * collisionDist;

	return distSq <= collisionDistSq;
}

#include "PhysicsHelper.hpp"

#include "../math/Vector.hpp"

bool checkCollision( wlVec2 posA, wlVec2 posB, float radiusA, float radiusB ) {
	const float dx = posA.x - posB.x;
	const float dy = posA.y - posB.y;
	const float distSq = dx * dx + dy * dy;

	const float collisionDist = radiusA + radiusB;
	const float collisionDistSq = collisionDist * collisionDist;

	return distSq <= collisionDistSq;
}
